#ifndef WOOD_BOX_H_
#define WOOD_BOX_H_

#include "Actor/FieldActor.h"

// 障害物の木箱クラス
class WoodBox : public FieldActor {
public:
    // コンストラクタ
    WoodBox(IWorld* world, const GSvector3& position);

    // 更新
    void update(float delta_time) override;
    // 衝突判定
    void react(Actor& other) override;

    // 個別の削除処理
    void this_prepare_erase() override;

private:
    // デバッグ用更新処理
    void debug_update(float delta_time);

private:
    // 体力
    int hp_{ 0 };
};

#endif // !WOOD_BOX_H_
