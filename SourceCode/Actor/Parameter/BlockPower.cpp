#include "BlockPower.h"

// コンストラクタ
BlockPower::BlockPower(int max_block) {
    max_block_ = max_block;
    reset();
}

// 耐久力の取得
int BlockPower::block() const {
    return block_;
}

// 最大耐久力の増加
void BlockPower::add_max(int value) {
    max_block_ += value;
    block_ += value;
}

// 耐久力の減少
void BlockPower::sub(int value) {
    block_ = std::max(0, block_ - value);
}

// 耐久力のリセット
void BlockPower::reset() {
    block_ = max_block_;
}
