#ifndef HIT_POINT_H_
#define HIT_POINT_H_

#include <algorithm>

// 体力管理クラス
class HitPoint {
public:
    // コンストラクタ
    HitPoint(int max_hp);

    // 現在体力の取得
    float current() const;
    // 最大体力の取得
    float max() const;
    // 現在HPの増加
    void add(int value);
    // 現在HPの減少
    void sub(int value);
    // 最大HPの増加
    void add_max(int value);
    // 現在の体力の割合を取得
    float rate();
    // 現在の被ダメージ量を取得
    int damage() const;

private:
    // 現在HP
    float hp_{ 1 };
    // 最大HP
    float max_hp_{ 1 };
    // 被ダメージ量
    int damage_{ 0 };
};

#endif // !HIT_POINT_H_
