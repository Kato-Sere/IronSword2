#include "ItemPotSpawnerManager.h"
#include "Actor/ActorEnum.h"

#include <algorithm>
#include <iterator>

// アイテム壺生成地点取得
void ItemPotSpawnerManager::get_point(IWorld* world) {
    // ワールド内のアイテム壺生成地点を配列に格納
    spawners_.reserve(world->count_actor_with_tag(Tag_ItemSpawner));
    std::vector<Actor*> spawners = world->find_actor_with_tag(Tag_ItemSpawner);
    std::transform(spawners.begin(), spawners.end(), std::back_inserter(spawners_), [](Actor* spawner) { return dynamic_cast<ItemPotSpawner*>(spawner); });
}

// アイテム壺生成
void ItemPotSpawnerManager::generate(int count) {
    // 引数分ランダムにアイテム壺生成
    for (int i = 0; i < count;) {
        // アイテム壺が生成されていない地点を取得
        std::vector<ItemPotSpawner*> spawners;
        std::remove_copy_if(spawners_.begin(), spawners_.end(), std::back_inserter(spawners), [](ItemPotSpawner* spawner) { return spawner->is_generated(); });
        if (spawners.size() == 0) break;
        // アイテム壺生成
        int num = gsRand(0, spawners.size() - 1);
        spawners[num]->generate_item_pot();
        ++i;
    }
}

// 全地点にアイテム壺生成
void ItemPotSpawnerManager::generate_all() {
    for (auto spawner : spawners_) {
        if (!spawner->is_generated()) spawner->generate_item_pot();
    }
}

// 初期化
void ItemPotSpawnerManager::clear() {
    world_ = nullptr;
    for (auto spawner : spawners_) {
        spawner = nullptr;
        delete spawner;
    }
    spawners_.clear();
}
