#ifndef FADE_SWITCHER_H_
#define FADE_SWITCHER_H_

#include <GStype.h>

// フェードイン・フェードアウト
class FadeSwitcher {
public:
    // コンストラクタ
    FadeSwitcher();

    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;

    // フェードイン開始
    void start_in();
    // フェードアウト開始
    void start_out();

    // α値を取得
    float alpha() const;

    // フェードインが終了しているか？
    bool is_end_fade_in() const;
    // フェードアウトが終了しているか？
    bool is_end_fade_out() const;

    // 初期化
    void clear();

private:
    // フェードイン
    void fade_in(float delta_time);
    // フェードアウト
    void fade_out(float delta_time);

private:
    // フェードイン中か？
    bool is_fading_in_{ false };
    // フェードインは終了したか？
    bool is_end_fade_in_{ true };
    // フェードアウト中か？
    bool is_fading_out_{ false };
    // フェードアウトは終了したか？
    bool is_end_fade_out_{ false };
    // 矩形のα値
    float rect_alpha_{ 1.0f };
};

#endif // !FADE_SWITCHER_H_
