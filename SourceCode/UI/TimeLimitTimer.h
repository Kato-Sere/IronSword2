#ifndef TIME_LIMIT_TIMER_H_
#define TIME_LIMIT_TIMER_H_

#include <GStype.h>

// 残り時間管理クラス
class TimeLimitTimer {
public:
    // コンストラクタ
    TimeLimitTimer();
    // デストラクタ
    ~TimeLimitTimer();

    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;

    // タイマーセット
    void set(float time);
    // タイマー開始
    void start();
    // タイマー停止
    void stop();
    // 初期化
    void reset();

    // タイマー増加
    void add(float time);
    // タイマー減少
    void sub(float time);

    // 現在のタイマー取得
    float get_current() const;
    // 累計のタイマーを取得
    float get_elapsed() const;

    // タイムオーバーか？
    bool is_time_over() const;

    // デバッグ用更新
    void debug_update(float delta_time);

private:
    // タイマー動作フラグ
    bool run_timer_{ false };
    // 残り時間の最大値
    float max_limit_time_{ 5.0f };
    // 残り時間
    float limit_time_{ 0.0f };
    // 累計時間
    float elapsed_time_{ 0.0f };
};

#endif // !TIME_LIMIT_TIMER_H_
