#ifndef NUMBER_TEXTURE_H_
#define NUMBER_TEXTURE_H_

#include<gslib.h>
#include<string>

// ナンバーテクスチャ
class NumberTexture {
public:
    // コンストラクタ
    NumberTexture(GSuint texture, int width, int height);
    // 描画(右詰め、0補完あり)
    void draw_r_fill(const GSvector2& position, int num, int digit, char fill = '0',
        const GSvector2& scale = GSvector2{ 1.0f, 1.0f }) const;
    // 描画(右詰め、0補完なし)
    void draw_r(const GSvector2& position, int num, int digit,
        const GSvector2& scale = GSvector2{ 1.0f, 1.0f }) const;

    // 描画(左詰め)
    void draw_l(const GSvector2& position, int num,
        const GSvector2& scale = GSvector2{ 1.0f, 1.0f }) const;
    // 描画
    void draw(const GSvector2& position, const std::string& num,
        const GSvector2& scale = GSvector2{ 1.0f, 1.0f }) const;

private:
    // フォント用のテクスチャ
    GSuint texture_;
    // 文字の幅
    int width_;
    // 文字の高さ
    int height_;
};

#endif // !NUMBER_TEXTURE_H_
