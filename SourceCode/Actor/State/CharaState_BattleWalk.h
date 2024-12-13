#ifndef CHARA_STATE_SIDE_WALK_H_
#define CHARA_STATE_SIDE_WALK_H_

#include "ICharaState.h"
#include "Actor/Character/Character.h"
#include "Actor/Character/Enemy.h"

// 状態・戦闘時移動 敵専用
class CharaState_BattleWalk : public ICharaState {
    enum Move_Direction {
        None,
        Left,
        Right,
        Back,
        Forward,
    };
public:
    // コンストラクタ
    CharaState_BattleWalk(Enemy* chara);

    // 開始
    void start() override;
    // 更新
    void update(float delta_time) override;
    // 終了
    void end() override;
    // 衝突処理
    void react(Actor& other) override;
    // 終了しているか？
    bool is_end() const override { return is_end_; }

private:
    // モーション系更新処理
    void motion_update(float delta_time);
    // 状態遷移系更新処理
    void select_state();
    // パラメーター取得
    CharacterParameters& parameters();

    // 目標が近すぎるか？
    bool is_target_too_close();
    // 目標が遠すぎるか？
    bool is_target_too_far();

private:
    // 対象
    Enemy* chara_;
    // 移動タイマー
    float move_timer_{ 0.0f };
    // 移動方向
    int move_direction_{ 0 };
};

#endif // !CHARA_STATE_SIDE_WALK_H_
