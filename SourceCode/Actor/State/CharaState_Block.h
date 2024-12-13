#ifndef CHARA_STATE_BLOCK_H_
#define CHARA_STATE_BLOCK_H_

#include "InputManager.h"
#include "Actor/Character/Character.h"
#include "ICharaState.h"

// 状態・防御
class CharaState_Block : public ICharaState {
public:
    // コンストラクタ
    CharaState_Block(Character* chara);
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
    // パラメーター取得
    CharacterParameters& parameters();

private:
    // 対象
    Character* chara_;
};


#endif // !CHARA_STATE_BLOCK_H_
