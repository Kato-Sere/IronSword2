#include "Assets.h"
#include "KOCount.h"
#include "NumberTexture.h"

#include <imgui/imgui.h>

// 数字の描画場所
const GSvector2 Count_Position{ 1102.0f, 600.0f };
// 文字の描画場所
const GSvector2 Text_Position{ 1134.0f, 660.0f };
// カンスト値
const int Count_Stop{ 9999 };

// コンストラクタ
KOCount::KOCount() {}

// デストラクタ
KOCount::~KOCount() {
    clear();
}

// 描画
void KOCount::draw() const {
    // スコアの数字
    static const NumberTexture number{ Texture_Number, 32, 64 };
    number.draw_r(Count_Position, ko_count_, 4);
    // 文字
    gsDrawSprite2D(Texture_Text_KOCount, &Text_Position, NULL, NULL, NULL, NULL, 0.0f);
}

// 撃破数の加算
void KOCount::add() {
    // 「9999」をカンスト値として設定
    ko_count_ = std::min(ko_count_ + 1, Count_Stop);
}

// 撃破数の取得
int KOCount::get() const {
    return ko_count_;
}

// 撃破数のクリア
void KOCount::clear() {
    ko_count_ = 0;
}
