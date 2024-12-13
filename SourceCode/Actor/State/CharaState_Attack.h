#ifndef CHARA_STATE_ATTACK_H_
#define CHARA_STATE_ATTACK_H_

#include "ICharaState.h"
#include "Actor/Character/Character.h"
#include "Actor/Parameter/ActionData.h"

// 状態・攻撃
class CharaState_Attack : public ICharaState {
public:
    // コンストラクタ
    CharaState_Attack(Character* chara, int combo);

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
    // 移動・モーション系更新処理
    void motion_update(float delta_time);
    // 状態遷移系更新処理
    void select_state();
    // 次の攻撃のデータを取得
    void next_data(GSuint motion);
    // パラメーター取得
    CharacterParameters& parameters();

    // デバッグ用更新処理
    void debug_update(float delta_time);

private:
    // 対象
    Character* chara_;
    // 連撃フラグ
    bool combo_normal_{ false };
    bool combo_charge_{ false };
    // 次のモーションデータ
    ActionData data_{};
    // 次のモーションデータ候補
    ActionData next_normal_{};
    ActionData next_charge_{};
    // 使用しているデータ
    ActionData use_data_;
    // 連続攻撃回数
    int combo_count_{ 0 };
};

#endif // !CHARA_STATE_ATTACK_H_
