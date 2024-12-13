#include "CharaState_JumpAttack.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "Actor/Character/CharacterConstantValue.h"

// コンストラクタ
CharaState_JumpAttack::CharaState_JumpAttack(Character* chara) : chara_{ chara } {}

// 開始
void CharaState_JumpAttack::start() {
    // ジャンプ攻撃開始時にy軸移動速度を0にする
    if (chara_->current_motion() == Motion_JumpC_In) chara_->velocity_.y = 0;
}

// 更新
void CharaState_JumpAttack::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_JumpAttack::end() {}

// 衝突処理
void CharaState_JumpAttack::react(Actor& other) {
    // 攻撃を受けるタグなら被ダメージ処理
    if (other.tag() == chara_->damage_tag()) chara_->take_damage(other);
}

// 移動・モーション系更新処理
void CharaState_JumpAttack::motion_update(float delta_time) {
    // 着地中は移動しない
    if (chara_->current_motion() == Motion_JumpC_Out) return;
    chara_->translate_jump(delta_time, parameters().move_speed().speed());
}

// 状態遷移系更新処理
void CharaState_JumpAttack::select_state() {
    // 移動の入力がある場合はモーションキャンセルを行う
    float motion_end_timing = chara_->state_trigger().Move ?
        Rigit_Cancell_Timing : 1.0f;
    switch (chara_->current_motion()) {
    case Motion_JumpC_In:
        // 接地したら着地
        if (chara_->is_land()) chara_->change_state(State_JumpAttack, Motion_JumpC_Out, false);
        break;
    case Motion_JumpC_Out:
        if (chara_->ratio_motion_time() >= motion_end_timing) chara_->change_state(State_Move, Motion_Idle);
        break;
    default:
        break;
    }
}

// パラメーター取得
CharacterParameters& CharaState_JumpAttack::parameters() {
    return chara_->parameters();
}
