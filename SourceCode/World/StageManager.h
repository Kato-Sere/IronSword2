#ifndef STAGE_MANAGER_H_
#define STAGE_MANAGER_H_

#include "IWorld.h"
#include "Actor/EnemyControll/EnemySpawnManager.h"
#include "Actor/Item/ItemPotSpawnerManager.h"

// ステージ管理クラス
class StageManager {
public:
    //使用準備
    void prepare(IWorld* world);

    // 更新
    void update(float delta_time);
    // 初期化
    void clear();

private:
    // ワールド
    IWorld* world_{ nullptr };
    // 敵生成アクター管理用
    EnemySpawnManager enemy_spawners_;
    // アイテム壺生成アクター管理用
    ItemPotSpawnerManager item_pot_spawners_;
    // 次にアイテム壺を生成する撃破数
    int next_generate_item_pot_{ 0 };
};

#endif // !STAGE_MANAGER_H_
