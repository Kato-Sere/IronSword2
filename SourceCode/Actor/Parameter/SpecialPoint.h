#ifndef SPECIAL_POINT_H_
#define SPECIAL_POINT_H_

#include <algorithm>

// 特殊技管理クラス
class SpecialPoint {
public:
    // コンストラクタ
    SpecialPoint();

    // 現在値の取得
    int get() const;
    // 最大値の取得
    int max() const;
    // 現在値の増加
    void add(int value);
    // 現在値のリセット
    void reset();
    // ゲージは満タンか？
    bool is_full() const;

private:
    // 現在値
    int current_point_{ 0 };
    // 最大値
    int max_point_{ 200 };
};

#endif // !SPECIAL_POINT_H_
