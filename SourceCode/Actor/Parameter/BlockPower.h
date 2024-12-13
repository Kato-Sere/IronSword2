#ifndef BLOCK_POWER_H_
#define BLOCK_POWER_H_

#include <algorithm>

// ガード耐久力管理クラス
class BlockPower {
public:
    // コンストラクタ
    BlockPower(int max_block);

    // 耐久力の取得
    int block() const;
    // 最大耐久力の増加
    void add_max(int value);
    // 耐久力の減少
    void sub(int value);
    // 耐久力のリセット
    void reset();

private:
    // 現在の耐久力
    int block_{ 1 };
    // 最大の耐久力
    int max_block_{ 1 };
};

#endif // !BLOCK_POWER_H_
