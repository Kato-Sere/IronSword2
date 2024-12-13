#include "CharaState_Damage.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "Actor/Character/CharacterConstantValue.h"

// コンストラクタ
CharaState_Damage::CharaState_Damage(Character* chara) : chara_{ chara } {}

// 開始
void CharaState_Damage::start() {}

// 更新
void CharaState_Damage::update(float delta_time) {
    moiton_update(delta_time);
    select_state();
}

// 終了
void CharaState_Damage::end() {}

// 衝突処理
void CharaState_Damage::react(Actor& other) {
    if (other.tag() == chara_->damage_tag()) chara_->take_damage(other);
}

// モーション系更新処理
void CharaState_Damage::moiton_update(float delta_time) {
    // アクターの速度を取得
    GSvector3 velocity = chara_->velocity_;
    // ノックバックさせる
    chara_->transform_.translate(velocity * delta_time, GStransform::Space::World);
    chara_->velocity_ -= GSvector3{ velocity.x, 0.0f, velocity.z } * Damage_KnockBack_Rate * delta_time;
}

// 状態遷移系更新処理
void CharaState_Damage::select_state() {
    // 特殊技
    if (chara_->state_trigger().Special) chara_->change_state(State_Special, Motion_AttackSpecial_Prepare, false);
    // モーション終了後移動状態へ
    if (chara_->ratio_motion_time() >= 0.9f) chara_->change_state(State_Move, Motion_Idle);
}

// パラメーター取得
CharacterParameters& CharaState_Damage::parameters() {
    return chara_->parameters();
}
