#ifndef ENEMY_SE_MANAGER_H_
#define ENEMY_SE_MANAGER_H_

#include "Actor/Actor.h"

// 敵SE再生管理クラス
class EnemySEManager : public Actor {
public:
    // コンストラクタ
    EnemySEManager();

    // 更新
    void update(float delta_time) override;

    // ダメージボイス再生
    void play_damage();
    // 死亡ボイス再生
    void play_death();

private:
    // 再生中のダメージボイス番号
    int playing_damage_{ -1 };
    // 同一フレーム内で死亡ボイスを再生したか？
    bool is_played_death_{ false };
};

#endif // !ENEMY_VOICE_MANAGER_H_
