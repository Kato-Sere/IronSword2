#ifndef MOVE_SPEED_H_
#define MOVE_SPEED_H_

// 移動速度管理クラス
class MoveSpeed {
public:
    // コンストラクタ
    MoveSpeed(float speed);
    // 移動速度の取得
    float speed();
    // 移動倍率の取得
    float rate();
    // 移動倍率の設定
    void change_rate(float value);
    // 移動倍率のリセット
    void reset_rate();

private:
    // 移動速度
    float speed_{ 0.1f };
    // 移動倍率
    float rate_{ 1.0f };
};

#endif // !MOVE_SPEED_H_



