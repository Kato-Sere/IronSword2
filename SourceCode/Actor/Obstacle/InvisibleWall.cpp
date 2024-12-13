#include "InvisibleWall.h"
#include "Actor/ActorEnum.h"

// コンストラクタ
InvisibleWall::InvisibleWall(IWorld* world, const GSvector3& position, GSuint jamming_tag) {
    world_ = world;
    const GSvector3 center{ 0.0f, 1.0f, 0.0f };
    collider_ = BoundingSphere{ 1.0f, center };
    name_ = "InvisibleWall";
    tag_ = Tag_InvisibleWall;
    jamming_tag_ = jamming_tag;
    transform_.position(position);
    const GSvector3 scale{ 1.0f, 1.0f, 1.0f };
    transform_.localScale(scale);
    transform_.translate(velocity_, GStransform::Space::World);
}

// 妨害タグを取得
const GSuint InvisibleWall::jamming_tag() const {
    return jamming_tag_;
}
