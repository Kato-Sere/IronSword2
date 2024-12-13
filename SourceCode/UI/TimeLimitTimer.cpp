#include "Assets.h"
#include "TimeLimitTimer.h"
#include "NumberTexture.h"

#include <gslib.h>
#include <sstream>
#include <iomanip>
#include <imgui/imgui.h>

// 時間の表示座標
const GSvector2 Time_Draw_Pos{ 563.0f, 50.0f };
// 時間表示の拡大率
const GSvector2 Time_Scale{ 0.7f, 0.7f };

// コンストラクタ
TimeLimitTimer::TimeLimitTimer() {}

// デストラクタ
TimeLimitTimer::~TimeLimitTimer() {
    run_timer_ = false;
}

// 更新
void TimeLimitTimer::update(float delta_time) {
    if (run_timer_) {
        // 残り時間が0以下にならないように制限する
        limit_time_ = MAX(limit_time_ - (delta_time / 60.0f), 0.0f);
    }
#if _DEBUG
    debug_update(delta_time);
#endif // !_DEBUG
}

// 描画
void TimeLimitTimer::draw() const {
    // 現在の残り時間を取得
    float timer = get_current();
    // 分の計算
    int min = (int)limit_time_ / 60;
    timer -= min * 60;
    // タイマーの書式設定
    std::stringstream ss;
    ss << min << ':' << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << timer;
    // 数値の更新
    static const NumberTexture number{ Texture_Number, 32, 64 };
    number.draw(Time_Draw_Pos, ss.str(), Time_Scale);
}

// タイマーセット
void TimeLimitTimer::set(float time) {
    max_limit_time_ = time;
    reset();
}

// タイマー開始
void TimeLimitTimer::start() {
    run_timer_ = true;
}

// タイマー停止
void TimeLimitTimer::stop() {
    run_timer_ = false;
}

// 初期化
void TimeLimitTimer::reset() {
    run_timer_ = false;
    limit_time_ = max_limit_time_;
    elapsed_time_ = max_limit_time_;
}

// タイマー増加
void TimeLimitTimer::add(float time) {
    limit_time_ += time;
    elapsed_time_ += time;
}

// タイマー減少
void TimeLimitTimer::sub(float time) {
    if (limit_time_ >= time) {
        elapsed_time_ -= time;
        limit_time_ -= time;
    } else {
        elapsed_time_ -= limit_time_;
        limit_time_ = 0;
    }
}

// 現在のタイマー取得
float TimeLimitTimer::get_current() const {
    return limit_time_;
}

// 累計のタイマーを取得
float TimeLimitTimer::get_elapsed() const {
    return elapsed_time_;
}

// タイムオーバーか？
bool TimeLimitTimer::is_time_over() const {
    return limit_time_ <= 0.0f;
}

// デバッグ用更新
void TimeLimitTimer::debug_update(float delta_time) {
    ImGui::Begin("timer");
    if (ImGui::Button("add->5s")) add(5);
    if (ImGui::Button("sub->5s")) sub(5);
    if (ImGui::Button("start")) start();
    if (ImGui::Button("stop")) stop();
    if (ImGui::Button("reset")) reset();
    ImGui::End();
}
