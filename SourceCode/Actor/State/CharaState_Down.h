#ifndef CHARA_STATE_DOWN_H_
#define CHARA_STATE_DOWN_H_

#include "ICharaState.h"
#include "Actor/Character/Character.h"

// 状態・ダウン
class CharaState_Down : public ICharaState {
public:
    // コンストラクタ
    CharaState_Down(Character* chara, int dead_timer = 0.0f);

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

private:
    // 対象
    Character* chara_;
    // 消滅タイマー
    float dead_timer_{ 0.0f };
    // 起き上がりタイマー
    float react_timer_{ 0.0f };
};

#endif // ! CHARA_STATE_DOWN_H_
