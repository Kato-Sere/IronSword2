#ifndef INVISIBLE_WALL_H_
#define INVISIBLE_WALL_H_

#include "Actor/Actor.h"

// 透明な壁
class InvisibleWall : public Actor {
public:
    // コンストラクタ
    InvisibleWall(IWorld* world, const GSvector3& position, GSuint jamming_tag);

    // 妨害タグを取得
    const GSuint jamming_tag() const;

private:
    // 妨害するタグ
    GSuint jamming_tag_{ 0 };
};

#endif // !INVISIBLE_WALL_H_
