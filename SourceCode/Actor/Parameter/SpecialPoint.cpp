#include "SpecialPoint.h"

// コンストラクタ
SpecialPoint::SpecialPoint() {
    reset();
}

// 現在値の取得
int SpecialPoint::get() const {
    return current_point_;
}

// 最大値の取得
int SpecialPoint::max() const {
    return max_point_;
}

// 現在値の増加
void SpecialPoint::add(int value) {
    // 最大値を超えないように制限
    current_point_ = std::min(current_point_ + value, max_point_);
}

// 現在値のリセット
void SpecialPoint::reset() {
    current_point_ = 0;
}

// ゲージは満タンか？
bool SpecialPoint::is_full() const {
    return current_point_ == max_point_;
}
