#include "Actor/ActorEnum.h"
#include "CharacterParameters.h"

#include <stdexcept>

// コンストラクタ
CharacterParameters::CharacterParameters(const ColliderParam& collider, const HitPoint& hp,
    const MoveSpeed& speed, const BlockPower& block, const AttackRate& rate, float jump,
    const std::vector<ActionData>& action_n, const std::vector<ActionData>& action_c,
    const ActionData& action_jc, const ActionData& action_s, const std::vector<float>& knock)
    : collider_{ collider }, hp_{ hp }, move_speed_{ speed }, block_{ block },
attack_rate_{ rate }, jump_power_{ jump }, action_normal_{ action_n }, action_charge_{ action_c },
action_jumpc_{ action_jc }, action_special_{ action_s }, knockback_speed_{ knock } {}

// 衝突判定の取得
ColliderParam& CharacterParameters::collider() {
    return collider_;
}

// 体力の取得
HitPoint& CharacterParameters::hp() {
    return hp_;
}

// 体力の取得(const)
const HitPoint& CharacterParameters::hp() const {
    return hp_;
}

// 特殊技を取得
SpecialPoint& CharacterParameters::sp() {
    return sp_;
}

// 移動速度の取得
MoveSpeed& CharacterParameters::move_speed() {
    return move_speed_;
}

// ガード耐久力の取得
BlockPower& CharacterParameters::block() {
    return block_;
}

// 攻撃倍率の取得
AttackRate& CharacterParameters::attack_rate() {
    return attack_rate_;
}

// ジャンプ力を取得
float CharacterParameters::jump_power() {
    return jump_power_;
}

// 弱攻撃データを取得
const ActionData& CharacterParameters::action_normal(GSuint value) const {
    GSuint num = value - AtkN_first;
    return action_normal_.at(num);
}

// 強攻撃データを取得
const ActionData& CharacterParameters::action_charge(GSuint value) const {
    GSuint num = value - AtkC_first;
    return action_charge_.at(num);
}

const ActionData& CharacterParameters::action_jumpc() const {
    return action_jumpc_;
}

// 特殊技データを取得
const ActionData& CharacterParameters::action_special() const {
    return action_special_;
}

// ノックバック速度を取得
float CharacterParameters::knockback_speed(int value) {
    return knockback_speed_.at(value);
}

// NULL
CharacterParameters CharacterParameters::null() {
    return CharacterParameters{
        ColliderParam{0, 0}, HitPoint{0}, MoveSpeed{0}, BlockPower{0},
        AttackRate{0}, 0, std::vector<ActionData>{}, std::vector<ActionData>{},
        ActionData{}, ActionData{}, std::vector<float>{0, 0}
    };
}
