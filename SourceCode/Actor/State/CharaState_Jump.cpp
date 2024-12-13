#include "CharaState_Jump.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "Actor/Character/CharacterConstantValue.h"
#include <imgui/imgui.h>

// コンストラクタ
CharaState_Jump::CharaState_Jump(Character* chara) : chara_{ chara } {}

// 開始
void CharaState_Jump::start() {
    if (chara_->current_motion() == Motion_Jump_In) chara_->velocity_.y = parameters().jump_power();
}

// 更新
void CharaState_Jump::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_Jump::end() {}

// 衝突処理
void CharaState_Jump::react(Actor& other) {
    // 攻撃を受けるタグなら被ダメージ処理
    if (other.tag() == chara_->damage_tag()) chara_->take_damage(other);
}

// 移動・モーション系更新処理
void CharaState_Jump::motion_update(float delta_time) {
    // 着地中は移動しない
    if (chara_->current_motion() == Motion_Jump_Out) return;
    chara_->translate_jump(delta_time, parameters().move_speed().speed());
}

// 状態遷移系更新処理
void CharaState_Jump::select_state() {
    // 移動の入力がある場合はモーションキャンセルを行う
    float motion_end_timing = chara_->state_trigger().Move ?
        Rigit_Cancell_Timing : 1.0f;

    switch (chara_->current_motion()) {
    case Motion_Jump_In:
        if (chara_->ratio_motion_time() >= 0.9f) chara_->change_state(State_Jump, Motion_Jump_Idle);
        // モーションの後半で攻撃入力があった場合はジャンプ攻撃に状態遷移
        if (chara_->ratio_motion_time() >= JumpAttack_Timing && chara_->state_trigger().AttackC) chara_->change_state(State_JumpAttack, Motion_JumpC_In, false);
        break;
    case Motion_Jump_Idle:
        // 接地したら着地
        if (chara_->is_land()) chara_->change_state(State_Jump, Motion_Jump_Out, false);
        // 攻撃入力があった場合はジャンプ攻撃に状態遷移
        if (chara_->state_trigger().AttackC) chara_->change_state(State_JumpAttack, Motion_JumpC_In, false);
        break;
    case Motion_Jump_Out:
        if (chara_->ratio_motion_time() >= motion_end_timing) chara_->change_state(State_Move, Motion_Idle);
        break;
    default:
        break;
    }
}

// パラメーター取得
CharacterParameters& CharaState_Jump::parameters() {
    return chara_->parameters();
}
