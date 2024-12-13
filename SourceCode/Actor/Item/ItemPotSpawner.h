#ifndef ITEM_POT_SPAWNER_H_
#define ITEM_POT_SPAWNER_H_

#include "Actor/Actor.h"

// アイテム壺生成用アクター
class ItemPotSpawner : public Actor {
public:
    // コンストラクタ
    ItemPotSpawner(IWorld* world, const GSvector3& position);

    // アイテム壺生成
    void generate_item_pot();
    // アイテム壺を生成済みか？
    bool is_generated();

    // メッセージの受信
    virtual void handle_massage(const std::string& message, void* param) override;

private:
    // 壺を生成済みか？
    bool is_generated_{ false };
};

#endif // !ITEM_POT_SPAWNER_H_
