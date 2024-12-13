#ifndef RANKS_H_
#define RANKS_H_

#include "Captain.h"
#include "Enemy.h"
#include "UI/HPGauge.h"

// 兵卒クラス
class Ranks : public Enemy {
public:
    // コンストラクタ
    Ranks(IWorld* world = nullptr, const GSvector3& position = GSvector3::zero(), Captain* leader = nullptr);

    // 更新
    void update(float delta_time) override;
    // 各状態への遷移条件
    void update_state_trigger(float delta_time) override;

    // 体力が0になった瞬間の処理
    void down() override;

    // 撤退準備を行う
    void set_reterat();
    // 自身の隊長が生存しているか？
    bool is_arive_leader() const;

protected:
    // 攻撃状態への遷移判定・移動時
    void judge_attack_moving(float delta_time);
    // 攻撃状態への遷移判定・攻撃時
    void judge_attack_attacking(float delta_time);
    // デバッグ用更新処理
    void debug_update(float delta_time) override;

private:
    // 状態の追加
    void add_states(IWorld* world) override;

    // 移動状態への遷移条件定義
    bool judge_move();
    // 攻撃状態への遷移条件定義
    void judge_attack(float delta_time);

    // 移動目標を決定
    void set_target();

private:
    // 自分の隊長
    Captain* leader_{ nullptr };
    // 移動目標設定用タイマー
    int timer_{ 0 };
};


#endif // !RANKS_H_
