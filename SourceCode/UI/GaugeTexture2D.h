#ifndef GAUGE_TEXTURE_2D_H_
#define GAUGE_TEXTURE_2D_H_

#include<gslib.h>

// 2Dゲージテクスチャクラス
class GaugeTexture2D {
public:
	// コンストラクタ(ゲージ用、背景用、幅、高さ)
	GaugeTexture2D(GSuint gauge_texture, GSuint background_texture,
		int texture_width, int texture_height);
	// 描画・背景の描画倍率固定
	void draw_const_bg(const GSvector2& position, int width, int height, int value,
		int max_value, const GScolor& gauge_color = GScolor{ 1.0f,1.0f,1.0f,1.0f },
		const GScolor& bg_color = GScolor{ 1.0f,1.0f,1.0f,1.0f }) const;
    // 描画・背景の描画倍率可変
    void draw(const GSvector2& position, int width, int height, int value,
        int max_value, const GScolor& gauge_color = GScolor{ 1.0f,1.0f,1.0f,1.0f },
        const GScolor& bg_color = GScolor{ 1.0f,1.0f,1.0f,1.0f }) const;

private:
	// ゲージのテクスチャ
	GSuint gauge_texture_;
	// 背景のテクスチャ
	GSuint bg_texture_;
	// テクスチャの幅
	int texture_width_;
	// テクスチャの高さ
	int texture_height_;

};

#endif // !GAUGE_TEXTURE_2D_H_
