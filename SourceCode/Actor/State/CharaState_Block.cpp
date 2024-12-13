#include "CharaState_Block.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "Actor/Character/CharacterConstantValue.h"

// コンストラクタ
CharaState_Block::CharaState_Block(Character* chara) : chara_{ chara } {}

// 開始
void CharaState_Block::start() {
    // 移動倍率を減少させる
    parameters().move_speed().change_rate(Block_Move_Speed);
}

// 更新
void CharaState_Block::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_Block::end() {
    // 移動速度を元に戻す
    parameters().move_speed().reset_rate();
}

// 衝突処理
void CharaState_Block::react(Actor& other) {
    // 衝突相手のタグは自分が攻撃を受けるタグか？
    if (other.tag() == chara_->damage_tag()) {
        // ターゲット方向のベクトルを求める
        GSvector3 to_target = other.transform().position() - chara_->transform().position();
        to_target.y = 0.0f;
        // ガードブレイク中に攻撃を受けた場合は被撃状態に移行する
        if (chara_->current_motion() == Motion_Block_Break) {
            chara_->take_damage(other);
            parameters().block().reset();
            return;
        }
        // ガード強度を減少させる
        parameters().block().sub(other.attack());
        // ガード強度がマイナスになったらガード強制解除
        if (parameters().block().block() == 0) {
            chara_->change_state(State_Block, Motion_Block_Break, false);
            return;
        }
        // 速度の代入とモーション変更
        chara_->velocity_ = parameters().knockback_speed(0) * Down_KnockBack_Rate * to_target;
        chara_->change_state(State_Block, Motion_Block_Hit, false, 2.0f);
    }
}

// 移動・モーション系更新処理
void CharaState_Block::motion_update(float delta_time) {
    chara_->translate_block(delta_time, parameters().move_speed().speed());
}

// 状態遷移系更新処理
void CharaState_Block::select_state() {
    switch (chara_->current_motion()) {
    case Motion_Block_In:
    case Motion_Block_Hit:
        if (chara_->ratio_motion_time() >= 1.0f) chara_->change_state(State_Block, Motion_Block_Idle);
        break;
    case Motion_Block_Idle:
    case Motion_Block_WalkF: case Motion_Block_WalkB:
    case Motion_Block_WalkL: case Motion_Block_WalkR:
        // 構え解除
        if (!chara_->state_trigger().Block) chara_->change_state(State_Block, Motion_Block_Out, false, 2.0f);
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
        break;
    case Motion_Block_Out:
    case Motion_Block_Break:
        if (chara_->ratio_motion_time() >= 1.0f) chara_->change_state(State_Move, Motion_Idle);
        break;
    default:
        return;
    }
}

// パラメーター取得
CharacterParameters& CharaState_Block::parameters() {
    return chara_->parameters();
}
