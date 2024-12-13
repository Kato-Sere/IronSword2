#ifndef ENEMY_SPAWNER_H_
#define ENEMY_SPAWNER_H_

#include "Actor/Actor.h"

// 敵生成用アクター
class EnemySpawner : public Actor {
public:
    // コンストラクタ
    EnemySpawner(IWorld* world, const GSvector3& position);

    // 敵アクター生成
    void generate_enemy();
};

#endif // !ENEMY_SPAWNER_H_
