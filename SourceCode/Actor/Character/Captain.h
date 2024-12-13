#ifndef CAPTAIN_H_
#define CAPTAIN_H_

#include "Enemy.h"
#include "UI/HPGauge.h"

class Ranks;

// 兵長クラス
class Captain : public Enemy {
public:
    // コンストラクタ
    Captain(IWorld* world = nullptr, const GSvector3& position = GSvector3::zero());

    // 更新
    void update(float delta_time) override;

    // 各状態への遷移条件
    void update_state_trigger(float delta_time) override;

    // 体力が0になった瞬間の処理
    void down() override;
    // 部下をリストから削除
    void erase_soldier(Ranks& ranks);

private:
    // 状態の追加
    void add_states(IWorld* world) override;

    // 部下リストに追加
    void add_soldiers(int value);
    // 自分の部隊の攻撃処理
    void unit_attack();

    // 移動状態への遷移条件定義
    bool judge_move();
    // 攻撃状態への遷移条件定義
    void judge_attack(float delta_time);
    // 移動目標を決定
    void set_target();

    // 自身を含めた攻撃できるキャラクターを取得
    std::vector<Enemy*> ready_is_attack();
    // 自身を含めた攻撃予定のキャラクターの数を取得
    const int attack_plan_addself() const;
    // デバッグ用更新処理
    void debug_update(float delta_time) override;

public:
    // 自身の部下リスト
    std::vector<Ranks*> ranks_;
private:
    int timer_{ 0 };
};

#endif // !CAPTAIN_H_
