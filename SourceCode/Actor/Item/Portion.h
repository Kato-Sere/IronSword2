#ifndef PORTION_H_
#define PORTION_H_

#include "Actor/Actor.h"

// 回復薬クラス
class Portion : public Actor {
public:
    // コンストラクタ
    Portion(IWorld* world, const GSvector3& position);

    // 描画
    void draw() const override;
    // 衝突判定
    void react(Actor& other) override;

    // 個別の削除処理
    void this_prepare_erase() override;
};

#endif // !PORTION_H_
