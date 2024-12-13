#include "AttackRate.h"

// コンストラクタ
AttackRate::AttackRate(float rate) {
    attack_rate_ = rate;
}

// 攻撃倍率の取得
float AttackRate::rate() const {
    return attack_rate_;
}

// 攻撃倍率の変更
void AttackRate::change(float value) {
    attack_rate_ = value;
}

// 攻撃倍率のリセット
void AttackRate::reset() {
    attack_rate_ = 1.0f;
}
