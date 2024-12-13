#include "Actor/Actor.h"
#include "Actor/ActorEnum.h"
#include "Collider/ActionCollider.h"
#include "World/Field.h"
#include "World/IWorld.h"
#include "World/Line.h"

// 更新
void Actor::update(float delta_time) {}

// 遅延更新
void Actor::late_update(float delta_time) {}

// 描画
void Actor::draw() const {}

// 半透明オブジェクトの描画
void Actor::draw_transparent() const {}

// GUIの描画
void Actor::draw_gui() const {}

// 衝突処理
void Actor::react(Actor&) {}

// 間隔拡張処理
void Actor::react_spaceing(Actor& other) {}

// メッセージ処理
void Actor::handle_massage(const std::string& message, void* param) {}

// アクターとの衝突判定
void Actor::collide(Actor& other) {
    // どちらのアクターも衝突判定が有効か？
    if (!enable_collider_ || !other.enable_collider_) return;
    // 衝突判定をする
    if (is_collide(other)) {
        // 衝突した場合は、互いに衝突処理をする
        react(other);
        other.react(*this);
    }
    // どちらのアクターも間隔拡張を行うか？
    if (!use_spaceing_ || !other.use_spaceing_) return;
    // 間隔拡張用衝突判定をする
    if (is_spaceing_collide(other)) {
        // 互いに間隔を広げる
        react_spaceing(other);
        other.react_spaceing(*this);
    }
}

// 共通の削除準備
void Actor::prepare_erase() {
    this_prepare_erase();
    erase_ = true;
}

// 衝突しているか？
bool Actor::is_collide(const Actor& other) const {
    return collider().intersects(other.collider());
}

// 間隔拡張用衝突判定で衝突したか？
bool Actor::is_spaceing_collide(const Actor& other) const {
    return spaceing_collider().intersects(other.spaceing_collider());
}

// 削除するか？
bool Actor::is_erase() const {
    return erase_;
}

// 名前を取得
const std::string& Actor::name() const {
    return name_;
}

// タグ名を取得
const GSuint& Actor::tag() const {
    return tag_;
}

// 攻撃力を取得
const int& Actor::attack() const {
    return attack_;
}

// Transformを取得
GStransform& Actor::transform() {
    return transform_;
}

// Transformを取得（const）
const GStransform& Actor::transform() const {
    return transform_;
}

// 移動量を取得
GSvector3 Actor::velocity() const {
    return velocity_;
}

// ノックバック強度を取得
const std::bitset<8>& Actor::sp_effect() const {
    return sp_effect_;
}

// 衝突判定データを取得
BoundingSphere Actor::collider() const {
    return collider_.transform(transform_.localToWorldMatrix());
}

// 間隔拡張用衝突判定データの取得
BoundingSphere Actor::spaceing_collider() const {
    return spaceing_collider_.transform(transform_.localToWorldMatrix());
}

// 親アクターを取得
Actor* Actor::owner() {
    return owner_;
}

// アクターとの衝突判定
void Actor::collide_actor(Actor& other, float length) {
    // y座標を除く座標を求める
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // 相手との距離
    float distance = GSvector3::distance(position, target);
    // 衝突判定球の重なっている長さを求める
    float overlap = length - distance;
    // 重なっている部分の距離だけ離れる移動量を求める
    GSvector3 v = (position - target).getNormalized() * overlap;
    transform_.translate(v, GStransform::Space::World);
    // フィールドとの衝突判定
    collide_field();
}

// フィールドとの衝突判定
void Actor::collide_field() {
    // 壁との衝突判定(球との判定)
    GSvector3 center; // 衝突後の球の中心座標
    if (world_->field()->is_collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        //補正後の座標に変更する
        transform_.position(center);
    }

    // 地面との衝突判定(線分との交差判定)
    GSvector3 position = transform_.position();
    Line line;
    line.start_ = position + collider_.center_;
    line.end_ = position + GSvector3{ 0.0f, -0.08f, 0.0f };
    //line.draw();
    GSvector3 intersect; // 地面との交点
    // 衝突したフィールド用アクター
    Actor* field_actor{ nullptr };
    // 親をリセットしておく
    transform_.parent(nullptr);
    if (world_->field()->is_collide(line, &intersect, nullptr, &field_actor)) {
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
        // フィールド用のアクタークラスと衝突したか？
        if (field_actor != nullptr) {
            // 衝突したフィール用のアクターを親のトランスフォームクラスとして設定
            transform_.parent(&field_actor->transform());
        }
    }
}

// 視錐台に存在するか？
bool Actor::is_inside_frustum(const float rate) const {
    const GSvector3* center = new GSvector3{ collider().center() };
    bool result = gsIsSphereInsideFrustum(center, collider().radius() * rate);
    delete center;
    return result;
}
