#ifndef ITEM_POT_H_
#define ITEM_POT_H_

#include "ItemPotSpawner.h"
#include "Actor/Actor.h"
#include "Sound/SEManager.h"

// アイテム壺クラス
class ItemPot : public Actor {
public:
    // コンストラクタ
    ItemPot(IWorld* world, const GSvector3& position, ItemPotSpawner* spawner);
    // デストラクタ
    ~ItemPot() override;

    // 更新
    void update(float delta_time) override;
    // 描画
    void draw() const override;
    // 衝突判定
    void react(Actor& other) override;

    // 個別の消滅準備
    void this_prepare_erase() override;

private:
    // アイテムドロップ
    void drop_item();

private:
    // 体力
    int hp_{ 0 };
    // このアイテム壺を生成したスポナー
    ItemPotSpawner* spawner_{ nullptr };
    // エフェクトハンドル
    GSuint effect_handle_;
};

#endif // !ITEM_POT_H_
