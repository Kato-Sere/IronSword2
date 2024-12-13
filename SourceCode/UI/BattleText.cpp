#include "Assets.h"
#include "BattleText.h"

// テキストを表示させる座標
const GSvector2 Text_Pos{ 0.0f, 500.0f };
// テキスト1行分の高さ
const float Text_Height{ 60.0f };
// フェード処理の速度
const float Text_Fade_Speed{ 4.0f };

// 初期化
void BattleText::clear() {
    text_num_ = -1;
    text_alpha_ = -0.1f;
    current_limit_ = 0.0f;
    is_appeared_ = false;
}

// 更新
void BattleText::update(float delta_time) {
    // 表示テキストが設定されていないときは処理しない
    if (text_num_ < 0) return;

    // テキストが出現中か？
    if (is_appeared_) {
        current_limit_ -= delta_time;
        // 出現時間が終了した場合はα値減少
        if (current_limit_ <= 0) text_alpha_ -= (Text_Fade_Speed / 60.0f) * delta_time;
        // α値が0.0f以下になったら待機用配列確認
        if (text_alpha_ <= 0.0f) {
            // 配列の先頭確認
            BattleTextData first = data_.front();
            // 先頭にデータが格納されていない場合は初期化
            if (first.num == -1) clear();
            else {
                // 先頭のデータを設定し配列のデータを削除
                set_text(first.num, first.sec);
                pop_data();
            }
        }
    } else {
        // α値増加
        text_alpha_ += (Text_Fade_Speed / 60.0f) * delta_time;
        // α値が1.0f以上になったらフラグ設定
        if (text_alpha_ >= 1.0f) is_appeared_ = true;
    }
}

// 描画
void BattleText::draw() const {
    // 表示テキストが設定されていないときは処理しない
    if (text_num_ < 0) return;

    // テキスト画像表示
    GSrect text_rect{ 0.0f, Text_Height * text_num_, Screen_Size.x, Text_Height * (text_num_ + 1) }; // テキストの矩形
    GScolor text_color{ 1.0f, 1.0f, 1.0f, text_alpha_ }; // テキストのα値
    gsDrawSprite2D(Texture_Battle_Text, &Text_Pos, &text_rect, NULL, &text_color, NULL, 0.0f);
}

// 表示するテキストの設定
void BattleText::set(GSuint num, float sec) {
    // 出現中のテキストがない場合はそのまま表示
    if (text_num_ == -1) set_text(num, sec);
    // 出現中のテキストがある場合は待機用配列に格納
    else push_data(num, sec);
}

// テキスト表示中か？
bool BattleText::is_appearing() const {
    return text_alpha_ >= 0.0f;
}

// 出現させるテキストの設定
void BattleText::set_text(GSuint num, float sec) {
    text_alpha_ = 0.0f;
    text_num_ = (int)num;
    current_limit_ = sec * 60.0f;
    is_appeared_ = false;
}

// 待機用配列に格納
void BattleText::push_data(GSuint num, float sec) {
    for (int i = 0, size = data_.size(); i < size; ++i) {
        // 先頭から検索し、データがない場所に格納
        if (data_[i].num == -1) {
            data_[i] = BattleTextData{ (int)num, sec };
            return;
        }
    }
}

// 先頭のデータを削除
void BattleText::pop_data() {
    for (int i = 0, size = data_.size(); i < size; ++i) {
        if (i == size - 1) data_[i] = BattleTextData{ -1, 0.0f };
        else data_[i] = data_[i + 1];
    }
}
