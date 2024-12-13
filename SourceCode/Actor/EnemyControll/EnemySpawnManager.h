#ifndef ENEMY_SPAWN_MANAGER_H_
#define ENEMY_SPAWN_MANAGER_H_

#include "EnemySpawner.h"
#include "World/IWorld.h"

// 敵生成管理クラス
class EnemySpawnManager {
public:
    // 敵生成地点取得
    void get_point(IWorld* world);
    // 敵生成
    void generate();
    // 全地点に敵生成
    void generate_all();

    // リセット
    void clear();

private:
    // 全地点敵生成・スレッド用
    void th_generate_all();

private:
    // ワールド
    IWorld* world_{ nullptr };
    // 敵生成位置の配列
    std::vector<EnemySpawner*> spawners_;
};

#endif // !ENEMY_SPAWN_MANAGER_H_
