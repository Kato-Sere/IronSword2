#ifndef ITEM_POT_SPAWNER_MANAGER_H_
#define ITEM_POT_SPAWNER_MANAGER_H_

#include "ItemPotSpawner.h"
#include "World/IWorld.h"

// アイテム壺生成管理クラス
class ItemPotSpawnerManager {
public:
    // アイテム壺生成地点取得
    void get_point(IWorld* world);
    // アイテム壺生成
    void generate(int count);

    // 全地点にアイテム壺生成
    void generate_all();

    // 初期化
    void clear();

private:
    // ワールド
    IWorld* world_{ nullptr };
    // アイテム壺生成位置の配列
    std::vector<ItemPotSpawner*> spawners_;
};

#endif // !ITEM_POT_SPAWNER_MANAGER_H_
