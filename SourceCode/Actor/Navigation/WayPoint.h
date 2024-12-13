#ifndef WAY_POINT_H_
#define WAY_POINT_H_

#include "Actor/Actor.h"

// ウェイポイント
class WayPoint : public Actor {
public:
    // コンストラクタ
    WayPoint(IWorld* world, const GSvector3& position);
};

#endif // !WAY_POINT_H_
