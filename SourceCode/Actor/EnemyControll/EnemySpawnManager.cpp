#include "EnemySpawnManager.h"
#include "Actor/ActorEnum.h"

#include <thread>

// 敵生成地点取得
void EnemySpawnManager::get_point(IWorld* world) {
    // ワールド内の敵生成地点を配列に格納
    spawners_.reserve(world->count_actor_with_tag(Tag_EnemySpawner));
    const std::vector<Actor*>& spawners = world->find_actor_with_tag(Tag_EnemySpawner);
    for (auto spawner : spawners) {
        spawners_.emplace_back(static_cast<EnemySpawner*>(spawner));
    }
}

// 敵生成
void EnemySpawnManager::generate() {
    int num = gsRand(0, spawners_.size() - 1);
    std::thread th(&EnemySpawner::generate_enemy, spawners_[num]);
    th.detach();
}

// 全地点に敵生成
void EnemySpawnManager::generate_all() {
    std::thread th(&EnemySpawnManager::th_generate_all, this);
    th.detach();
}

// リセット
void EnemySpawnManager::clear() {
    world_ = nullptr;
    spawners_.clear();
}

// 全地点敵生成・スレッド用
void EnemySpawnManager::th_generate_all() {
    for (auto spawner : spawners_) {
        spawner->generate_enemy();
    }
}
