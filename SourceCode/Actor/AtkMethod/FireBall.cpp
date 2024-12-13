#include "Assets.h"
#include "FireBall.h"
#include "Actor/ActorEnum.h"
#include "Collider/ActionCollider.h"
#include "Sound/SEManager.h"
#include "World/Field.h"
#include "World/IWorld.h"
#include "World/Line.h"

#include <GSeffect.h>

// コンストラクタ
FireBall::FireBall(IWorld* world, Actor* owner, const BoundingSphere& collider, float lifespan,
    float attack, const GSvector3& velocity, const EffectParam& effect, const std::bitset<8>& sp_effect) : owner_name_{ owner->name() } {
    world_ = world;
    collider_ = BoundingSphere{ collider.radius_ };
    owner_ = owner;
    tag_ = Tag_PlayerAttack;
    name_ = "FireBall";
    attack_ = attack;
    velocity_ = velocity;
    lifespan_ = lifespan;
    sp_effect_ = sp_effect;
    transform_.position(collider.center_);
    transform_.rotation(GSquaternion::lookRotation(velocity));
    // 指定した座標・回転角・拡大率の行列を作成する
    GSmatrix4 localMat = GSmatrix4::TRS(effect.local_position, GSquaternion::euler(effect.local_rotation), effect.local_scale);
    // ワールド座標に変換する
    GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
    // エフェクト再生
    effect_handle_ = gsPlayEffectEx(effect.effect_id, &worldMat);
}

// デストラクタ
FireBall::~FireBall() {
    // SE再生
    SEManager::get_instance().play_se(SE_Fire_Explode);
    // 火球のエフェクトを停止し爆発エフェクトを再生する
    gsStopEffect(effect_handle_);
    const GSvector3 position = transform_.position();
    effect_handle_ = gsPlayEffect(Effect_Explosion, &position);
}

// 更新
void FireBall::update(float delta_time) {
    // 寿命が尽きた場合爆発
    if (lifespan_ < 0) prepare_erase();

    // 寿命の更新
    lifespan_ -= delta_time;
    // フィールドとの衝突判定
    collide_field();
    // 移動する
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

// 描画
void FireBall::draw() const {
    // 自身の座標に描画する
    const GSvector3 position = transform_.position();
    gsSetEffectPosition(effect_handle_, &position);
#if _DEBUG
    // デバッグ用衝突判定の描画
    // collider().draw();
#endif // _DEBUG
}

// 衝突リアクション
void FireBall::react(Actor& other) {
    // 自分と同じ名前か親の名前なら衝突しない
    if (other.name() == owner_name_ || other.name() == name()) return;
    // 衝突したら移動しないようにする
    velocity_ = GSvector3::zero();
    // 以後の衝突判定を行わない
    enable_collider_ = false;
    // 爆発する
    exploded_ = true;
    // 爆発用衝突判定の生成
    BoundingSphere new_col{ collider().radius() * 2, transform().position() };
    world_->add_actor(new ActionCollider{ world_, owner_, new_col, name(), tag_, 3.0f, attack_ * 2, 0.0f, GSvector3::zero(), sp_effect_ });
    // 死亡する
    prepare_erase();
}

// フィールドとの衝突判定
void FireBall::collide_field() {
    Line line;
    line.start_ = transform_.position();
    line.end_ = transform_.position() + velocity_;
    GSvector3 intersect;
    if (world_->field()->is_collide(line, &intersect)) {
        // 交点の座標に補正
        transform_.position(intersect);
        // 衝突したら移動しないようにする
        velocity_ = GSvector3::zero();
        // 死亡する
        prepare_erase();
        return;
    }
}
