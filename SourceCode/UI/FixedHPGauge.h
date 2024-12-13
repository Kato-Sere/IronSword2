#ifndef FIXED_HP_GAUGE_H_
#define FIXED_HP_GAUGE_H_

#include "Actor/Character/Character.h"
#include <GStype.h>

// UI用体力ゲージ
class FixedHPGauge {
public:
    // コンストラクタ
    FixedHPGauge(Character* chara = nullptr, GSvector2 position = GSvector2::zero(),
        int width = 0, int height = 0, const GScolor& gauge = GScolor{ 1.0f,1.0f,1.0f,1.0f });
    // デストラクタ
    ~FixedHPGauge();

    // 描画
    void draw() const;

private:
    // 対象
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
    GScolor color_back_{ 1.0f,1.0f, 1.0f, 1.0f };
};

#endif // !FIXED_HP_GAUGE_H_
