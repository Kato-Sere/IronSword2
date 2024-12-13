#include "ExitPoint.h"
#include "Actor/ActorEnum.h"
#include "World/Line.h"
#include "World/Field.h"
#include "World/IWorld.h"

// コンストラクタ
ExitPoint::ExitPoint(IWorld* world, const GSvector3& position) {
    world_ = world;
    const GSvector3 center{ 0.0f, 0.5f, 0.0f };
    collider_ = BoundingSphere{ 2.5f, center };
    name_ = "ExitPoint";
    transform_.position(position);
    transform_.translate(velocity_, GStransform::Space::World);
}

// 更新
void ExitPoint::update(float delta_time) {
    // フィールドとの衝突判定
    collide_field();
}

// 描画
void ExitPoint::draw() const {
}

// 衝突判定
void ExitPoint::react(Actor& other) {
    // 衝突した相手を消去する(念のためプレイヤーは消去しない)
    if(other.tag() != Tag_Player) other.prepare_erase();
}

// フィールドとの衝突判定
void ExitPoint::collide_field() {
    // フィールドとの衝突判定
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
    if (world_->field()->is_collide(line, &intersect)) {
        // 交点の位置からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
}
