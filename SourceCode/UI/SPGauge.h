#ifndef SP_GAUGE_H_
#define SP_GAUGE_H_

#include "Actor/Character/Character.h"
#include <GStype.h>

// 必殺技ゲージ
class SPGauge {
public:
    // コンストラクタ
    SPGauge(Character* target = nullptr, GSvector2 position = GSvector2::zero(),
        int width = 0, int height = 0, const GScolor& gauge = GScolor{ 1.0f,1.0f,1.0f,1.0f });
    // デストラクタ
    ~SPGauge();

    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;

private:
    // ゲージの色の設定
    GScolor color() const;

private:
    // 表示対象
    Character* target_;
    // 出現させる座標
    GSvector2 position_;
    // ゲージの幅
    int width_;
    // ゲージの高さ
    int height_;
    // ゲージの色
    GScolor color_gauge_;
    // 背景の色
    GScolor color_back_{ 1.0f, 1.0f, 1.0f, 1.0f };
    // タイマー
    int timer_{ 0 };
};

#endif // !SP_GAUGE_H_
