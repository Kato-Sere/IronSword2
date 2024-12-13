#include "CharaState_BlowOff.h"
#include "Actor/ActorEnum.h"
#include "Actor/Character/CharacterConstantValue.h"

// コンストラクタ
CharaState_BlowOff::CharaState_BlowOff(Character* chara) : chara_{ chara } {}

// 開始
void CharaState_BlowOff::start() {
    chara_->velocity_.y = BlowOff_Velocity_Y;
}

// 更新
void CharaState_BlowOff::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_BlowOff::end() {}

// 衝突処理
void CharaState_BlowOff::react(Actor& other) {}

// 移動・モーション系更新処理
void CharaState_BlowOff::motion_update(float delta_time) {
    chara_->transform().translate(chara_->velocity(), GStransform::Space::World);
}

// 状態遷移系更新処理
void CharaState_BlowOff::select_state() {
    if (chara_->ratio_motion_time() >= 0.9f) chara_->change_state(State_Down, Motion_Damage_Air);
}

// パラメーター取得
CharacterParameters& CharaState_BlowOff::parameters() {
    return chara_->parameters();
}


