#include "Assets.h"
#include "FixedHPGauge.h"
#include "GaugeTexture2D.h"

const GScolor Danger_Color{1.0f,0.1f,0.1f,1.0f};

// コンストラクタ
FixedHPGauge::FixedHPGauge(Character* chara, GSvector2 position, int width, int height, const GScolor& gauge) : target_{ chara }, position_{ position }, width_{ width }, height_{ height }, color_gauge_{ gauge } {}

// デストラクタ
FixedHPGauge::~FixedHPGauge() {
    target_ = nullptr;
}

// 描画
void FixedHPGauge::draw() const {
    // ゲージの幅と高さ
    static const GaugeTexture2D gauge{ Texture_Gauge_White, Texture_HPGauge, 16, 16 };
    // 残り体力が最大体力の25%以下なら色を変えて描画
    bool is_danger = target_->parameters().hp().current() <= target_->parameters().hp().max() / 4;
    GScolor color = is_danger ? Danger_Color : color_gauge_;
    HitPoint hp = target_->parameters().hp();
    gauge.draw_const_bg(position_, width_, height_, hp.current(), hp.max(), color, color_back_);
}
