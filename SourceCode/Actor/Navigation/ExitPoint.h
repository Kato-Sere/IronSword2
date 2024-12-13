#ifndef EXIT_POINT_H_
#define EXIT_POINT_H_

#include "Actor/Actor.h"

// 敵兵士の退却地点
class ExitPoint : public Actor {
public:
    // コンストラクタ
    ExitPoint(IWorld* world, const GSvector3& position);

    // 更新
    void update(float delta_time) override;
    // 描画
    void draw() const override;
    // 衝突判定
    void react(Actor& other) override;

private:
    // フィールドとの衝突判定
    void collide_field();
};

#endif // !EXIT_POINT_H
