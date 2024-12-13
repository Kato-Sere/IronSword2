#include "Assets.h"
#include "HPGauge.h"

// 1メモリ当たりのゲージの幅
const int Gauge_Width_Rate{ 8 };
// ゲージの高さ
const int Gauge_Height{ 6 };
// ゲージ用パラメーターの倍率
const float Gauge_Rate{ 0.1f };

// 背景の色
const GScolor BG_Color{ 0.0f,0.0f,0.0f,1.0f };
// 体力ゲージを描画する位置の補正値
const GSvector3 HPGauge_Offset{ 0.0f,1.0f,0.0f };
// 体力ゲージの最大の長さ
const int Max_HPGauge_Width{ 160 };

// ゲージ部分の補正値
const int Gauge_Offset_LR{ 1 }; // 左右
const int Gauge_Offset_TB{ 1 }; // 上下

// コンストラクタ
HPGauge::HPGauge(Character* target, const GScolor& color) : color_{ color } {
    target_ = target;
}

// デストラクタ
HPGauge::~HPGauge() {
    target_ = nullptr;
}

// 描画
void HPGauge::draw() const {
    //OpenGLの各種設定を退避
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //ライティング無効
    glDisable(GL_LIGHTING);
    //Zバッファに書き込みを行わない
    glDepthMask(GL_FALSE);

    // 体力を取得
    HitPoint hp = target_->parameters().hp();
    // 体力ゲージが長くなりすぎないように調整
    int hp_gauge_width = MIN(hp.max() * Gauge_Rate * Gauge_Width_Rate, Max_HPGauge_Width);
    // 体力ゲージの中心位置を計算
    GSvector3 position = target_->transform().position() + HPGauge_Offset;
    position.y += target_->collider().center().y;

    // ワールド座標からスクリーン座標に変換
    GSvector2 bg_pos;
    gsCalculateScreen(&bg_pos, &position);
    // 背景の描画
    GSrect bg_rect = { bg_pos.x - hp_gauge_width / 2, bg_pos.y - Gauge_Height / 2,
                       bg_pos.x + hp_gauge_width / 2, bg_pos.y + Gauge_Height / 2 };
    gsDrawRectangle2D(&bg_rect, &BG_Color);

    // 体力の割合を計算
    float rate = static_cast<float>(hp.current()) / static_cast<float>(hp.max());
    // ゲージ用矩形・左上と右下のx座標を計算
    float gauge_width = bg_rect.right - bg_rect.left - Gauge_Offset_LR * 2;
    float gauge_left = bg_rect.left + Gauge_Offset_LR;
    float gauge_right = gauge_left + gauge_width * rate;
    // 体力ゲージの描画
    GSrect gauge_rect = { gauge_left, bg_rect.top + Gauge_Offset_TB,
                          gauge_right, bg_rect.bottom - Gauge_Offset_TB};
    gsDrawRectangle2D(&gauge_rect, &color_);

    //OpenGLの各種設定を復帰
    glPopAttrib();
}
