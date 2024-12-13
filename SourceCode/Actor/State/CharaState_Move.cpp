#include "CharaState_Move.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"

// コンストラクタ
CharaState_Move::CharaState_Move(Character* chara) : chara_{ chara } {}

// 開始
void CharaState_Move::start() {}

// 更新
void CharaState_Move::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_Move::end() {}

// 衝突処理
void CharaState_Move::react(Actor& other) {
    // 攻撃を受けるタグなら被ダメージ処理
    if (other.tag() == chara_->damage_tag()) chara_->take_damage(other);
}

// 移動・モーション系更新処理
void CharaState_Move::motion_update(float delta_time) {
    chara_->translate_move(delta_time, parameters().move_speed().speed() * parameters().move_speed().rate());
}

// 状態遷移系更新処理
void CharaState_Move::select_state() {
    // 特殊技
    if (chara_->state_trigger().Special) chara_->change_state(State_Special, Motion_AttackSpecial_Prepare, false);
    // 弱攻撃
    if (chara_->state_trigger().AttackN) {
        chara_->change_state(State_Attack, Motion_AttackNormal1, false,
            parameters().action_normal(Motion_AttackNormal1).animation_speed);
    }
    // 強攻撃
    if (chara_->state_trigger().AttackC) {
        chara_->change_state(State_Attack, Motion_AttackCharge1, false,
            parameters().action_charge(Motion_AttackCharge1).animation_speed);
    }
    // 防御
    if (chara_->state_trigger().Block) chara_->change_state(State_Block, Motion_Block_In, false, 2.0f);
    // ジャンプ
    if (chara_->state_trigger().Jump) chara_->change_state(State_Jump, Motion_Jump_In, false);
    // 落下時
    if (!chara_->is_land()) chara_->change_state(State_Jump, Motion_Jump_Idle);
    // 戦闘時移動
    if (chara_->state_trigger().BattleWalk) chara_->change_state(State_BattleWalk, Motion_Walk_L);
}

// パラメーター取得
CharacterParameters& CharaState_Move::parameters() {
    return chara_->parameters();
}
