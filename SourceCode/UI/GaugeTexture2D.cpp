#include"GaugeTexture2D.h"

// 背景の拡大率
const GSvector2 BG_Scale{ 1.0f, 1.0f };
// ゲージの拡大補正率
const GSvector2 Gauge_Default_Scale{ 0.9875f, 0.8f };
// ゲージ表示位置の補正値
const int Gauge_Correction_Value{ 2 };

// コンストラクタ(ゲージ用、背景用、幅、高さ)
GaugeTexture2D::GaugeTexture2D(GSuint gauge_texture, GSuint background_texture, int texture_width, int texture_height) {
    gauge_texture_  = gauge_texture;
    bg_texture_     = background_texture;
    texture_width_  = texture_width;
    texture_height_ = texture_height;
}

// 描画・背景の描画倍率固定
void GaugeTexture2D::draw_const_bg(const GSvector2& position, int width, int height, int value,
    int max_value, const GScolor& gauge_color, const GScolor& bg_color) const {
    // 最大値を超えていたら超えないように補正
    if (value > max_value) value = max_value;
    // 背景の部分を表示
    gsDrawSprite2D(bg_texture_, &position, NULL, NULL, &bg_color, &BG_Scale, 0.0f);

    // ゲージの割合を求める
    float rate = value / static_cast<float>(max_value);
    // ゲージの拡大率を求める
    GSvector2 gauge_scaling;
    gauge_scaling.x = (width / static_cast<float>(texture_width_))
        * Gauge_Default_Scale.x * rate; //横方向
    gauge_scaling.y = height / static_cast<float>(texture_height_)
        * Gauge_Default_Scale.y;        //縦方向
    // ゲージを表示する座標を調整
    GSvector2 gauge_pos;
    gauge_pos.x = position.x + Gauge_Correction_Value;
    gauge_pos.y = position.y + texture_height_ * (1 - Gauge_Default_Scale.y) / 2;
    // ゲージの表示
    gsDrawSprite2D(gauge_texture_, &gauge_pos, NULL, NULL, &gauge_color, &gauge_scaling, 0.0f);
}

// 描画・背景の描画倍率可変
void GaugeTexture2D::draw(const GSvector2& position, int width, int height, int value,
    int max_value, const GScolor& gauge_color, const GScolor& bg_color) const {
    // ゲージの割合を求める
    float rate = value / static_cast<float>(max_value);

    // 背景の位置を計算
    GSvector2 bg_position;
    bg_position.x = position.x + width * rate;
    bg_position.y = position.y;
    // 背景の拡大率を計算
    GSvector2 bg_scaling;
    bg_scaling.x = width / static_cast<float>(texture_width_);
    bg_scaling.y = height / static_cast<float>(texture_height_);
    // 背景の表示
    gsDrawSprite2D(bg_texture_, &position, NULL, NULL, &bg_color, &bg_scaling, 0.0f);

    // ゲージの拡大率を求める
    GSvector2 gauge_scaling;
    gauge_scaling.x = bg_scaling.x * rate; //横方向
    gauge_scaling.y = bg_scaling.y;        //縦方向

    // ゲージの表示
    gsDrawSprite2D(gauge_texture_, &position, NULL, NULL, &gauge_color, &gauge_scaling, 0.0f);

}
