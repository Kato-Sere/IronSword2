#include "FadeSwitcher.h"
#include "Assets.h"
#include <GSgraphics_util.h>

// フェード処理速度
const float Fade_Speed{ 2.5f };

// コンストラクタ
FadeSwitcher::FadeSwitcher() {}

// 更新
void FadeSwitcher::update(float delta_time) {
    // フェードイン判定
    if (is_fading_in_) fade_in(delta_time);
    // フェードアウト判定
    if (is_fading_out_) fade_out(delta_time);
}

// 描画
void FadeSwitcher::draw() const {
    GSrect rect{ 0, 0, Screen_Size.x, Screen_Size.y };
    const GScolor col{ 0.0f, 0.0f, 0.0f, rect_alpha_ };
    GSvector2 pos = GSvector2::zero();
    gsDrawSprite2D(Texture_Fade, &pos, &rect, NULL, &col, NULL, 0);
}

// フェードイン開始
void FadeSwitcher::start_in() {
    if (is_end_fade_out_) {
        is_fading_in_ = true;
        is_end_fade_in_ = false;
    }
}

// フェードアウト開始
void FadeSwitcher::start_out() {
    if (is_end_fade_in_) {
        is_fading_out_ = true;
        is_end_fade_out_ = false;
    }
}

// α値を取得
float FadeSwitcher::alpha() const {
    return rect_alpha_;
}

// フェードインが終了しているか？
bool FadeSwitcher::is_end_fade_in() const {
    return rect_alpha_ > 1.0f;
}

// フェードアウトが終了しているか？
bool FadeSwitcher::is_end_fade_out() const {
    return rect_alpha_ < 0.0f;
}

// リセット
void FadeSwitcher::clear() {
    is_fading_in_ = false;
    is_end_fade_in_ = true;
    is_fading_out_ = false;
    is_end_fade_out_ = false;
    rect_alpha_ = 1.0f;
}

// フェードイン
void FadeSwitcher::fade_in(float delta_time) {
    // α値が1.0以上になるまで増加
    if (rect_alpha_ <= 1.0f) rect_alpha_ += (Fade_Speed / 60.0f) * delta_time;
    else {
        is_fading_in_ = false;
        is_end_fade_in_ = true;
    }
}

//フェードアウト
void FadeSwitcher::fade_out(float delta_time) {
    // α値が0.0以下になるまで減少
    if (rect_alpha_ >= 0.0f)  rect_alpha_ -= (Fade_Speed / 60.0f) * delta_time;
    else {
        is_fading_out_ = false;
        is_end_fade_out_ = true;
    }
}
