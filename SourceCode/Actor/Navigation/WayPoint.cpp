#include "WayPoint.h"
#include "Actor/ActorEnum.h"

#include <imgui/imgui.h>

// コンストラクタ
WayPoint::WayPoint(IWorld* world, const GSvector3& position) {
    world_ = world;
    name_ = "WayPoint";
    tag_ = Tag_WayPoint;
    // 衝突判定球(デバッグ用)
    collider_ = BoundingSphere{ 0.1f, position };
    // 衝突判定は無効化
    enable_collider_ = false;
    // transformの初期化
    transform_.position(position);
}
