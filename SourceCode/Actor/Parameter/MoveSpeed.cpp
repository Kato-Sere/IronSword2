#include "MoveSpeed.h"

// コンストラクタ
MoveSpeed::MoveSpeed(float speed) {
    speed_ = speed;
    reset_rate();
}

// 移動速度の取得
float MoveSpeed::speed() {
    return speed_;
}

// 移動倍率の取得
float MoveSpeed::rate() {
    return rate_;
}

// 移動倍率の設定
void MoveSpeed::change_rate(float value) {
    rate_ = value;
}

// 移動倍率のリセット
void MoveSpeed::reset_rate() {
    rate_ = 1.0f;
}
