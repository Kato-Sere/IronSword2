#ifndef HP_GAUGE_H_
#define HP_GAUGE_H_

#include"Actor/Character/Character.h"
#include <GStype.h>

// 体力ゲージ
class HPGauge {
public:
    // コンストラクタ
    HPGauge(Character* target = nullptr, const GScolor& color = GScolor{ 1.0f,1.0f,1.0f,1.0f });
    // デストラクタ
    ~HPGauge();

    // 描画
    void draw() const;

private:
    // 表示対象
    Character* target_;
    // ゲージの色
    GScolor color_;
};

#endif // !HP_GAUGE_H_
