#include "HitPoint.h"

// コンストラクタ
HitPoint::HitPoint(int max_hp) {
    max_hp_ = max_hp;
    hp_ = max_hp_;
    damage_ = 0;
}

// 現在体力取得
float HitPoint::current() const {
    return hp_;
}

// 最大体力の取得
float HitPoint::max() const {
    return max_hp_;
}

// 現在HPの増加
void HitPoint::add(int value) {
    hp_ = std::min(max_hp_, hp_ + value);
}

// 現在HPの減少
void HitPoint::sub(int value) {
    int prev_hp_ = hp_; // 減少前の体力を保存
    hp_ = std::max(0.0f, hp_ - value);
    damage_ += prev_hp_ - hp_;
}

// 最大HPの増加
void HitPoint::add_max(int value) {
    max_hp_ += value;
    hp_ += value;
}

// 現在の体力の割合を取得
float HitPoint::rate() {
    return hp_ / max_hp_ * 100;
}

// 現在の被ダメージ量を取得
int HitPoint::damage() const {
    return damage_;
}
