#include "Assets.h"
#include "GaugeTexture2D.h"
#include "SPGauge.h"

// ゲージ満タン時の点滅色
const GScolor SPGauge_Full_Color{ 1.0f,0.0f,1.0f,1.0f };
// 点滅速度
const int Flash_Speed{ 6 };

// コンストラクタ
SPGauge::SPGauge(Character* target, GSvector2 position, int width, int height, const GScolor& gauge) : position_{ position }, color_gauge_{ gauge } {
    target_ = target;
    width_ = width;
    height_ = height;
}

// デストラクタ
SPGauge::~SPGauge() {
    target_ = nullptr;
}

// 更新
void SPGauge::update(float delta_time) {
    if (target_->parameters().sp().is_full()) timer_++;;
}

// 描画
void SPGauge::draw() const {
    static const GaugeTexture2D gauge{ Texture_Gauge_White, Texture_SPGauge, 16, 16 };
    // ゲージの幅と高さ
    SpecialPoint sp = target_->parameters().sp();
    gauge.draw_const_bg(position_, width_, height_, sp.get(), sp.max(), color(), color_back_);
}

// ゲージの色の決定
GScolor SPGauge::color() const {
    // ゲージ満タン時は点滅させる
    if (target_->parameters().sp().is_full()) {
        if (timer_ % Flash_Speed >= Flash_Speed / 2) return color_gauge_;
        else return SPGauge_Full_Color;
    } else {
        return color_gauge_;
    }
}
