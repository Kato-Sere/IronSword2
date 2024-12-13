#include"NumberTexture.h"

#include <sstream>
#include <iomanip>
#include <string>

// コンストラクタ
NumberTexture::NumberTexture(GSuint texture, int width, int height) : 
	texture_{ texture }, width_{ width }, height_{ height }{
}

// 描画(右詰め) 0補完あり
void NumberTexture::draw_r_fill(const GSvector2& position, int num, int digit, char fill,
	const GSvector2& scale) const {
	std::stringstream ss;
	ss << std::setw(digit) << std::setfill(fill) << num;
	draw(position, ss.str(), scale);
}

// 描画(右詰め) 0補完なし
void NumberTexture::draw_r(const GSvector2& position, int num, int digit,
    const GSvector2& scale) const {
    std::stringstream ss;
    ss << std::setw(digit) << num;
    draw(position, ss.str(), scale);
}

// 描画(左詰め)
void NumberTexture::draw_l(const GSvector2& position, int num,
	const GSvector2& scale) const {
	draw(position, std::to_string(num), scale);
}

// 描画
void NumberTexture::draw(const GSvector2& position, const std::string& num,
	const GSvector2& scale) const {
	// 文字を1文字ずつ描画する
	for (int i = 0; i < (int)num.size(); ++i) {
		if (num[i] == ' ') continue; // 空白文字はスキップ
		// char型をint型に変換
	    int n = (num[i] != '.') ? num[i] - '0' : 10; // 小数点は9の次にあることを前提としている
        if (num.substr(i, 1) == ":") n = 11; // 「：」は小数点の次にあることが前提
		// 数字に対応する画像を切り出すための矩形を計算する(数値フォント画像内の位置)
		GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
		// 数字を描画する座標を計算
		GSvector2 pos{ position.x + i * width_ * scale.x, position.y * scale.y};
		// 数字を1桁描画
		gsDrawSprite2D(texture_, &pos, &rect, NULL, NULL, &scale, 0);
	}
}
