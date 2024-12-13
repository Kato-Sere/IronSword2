#ifndef ATTACK_RATE_H_
#define ATTACK_RATE_H_

// 攻撃倍率管理クラス
class AttackRate {
public:
    // コンストラクタ
    AttackRate(float rate);

    // 攻撃倍率の取得
    float rate() const;
    // 攻撃倍率の変更
    void change(float value);
    // 攻撃倍率のリセット
    void reset();

private:
    // 攻撃倍率
    float attack_rate_{ 1.0f };
};

#endif // !ATTACK_RATE_H_

