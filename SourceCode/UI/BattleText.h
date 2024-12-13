#ifndef BATTLE_TEXT_H_
#define BATTLE_TEXT_H_

#include <GStype.h>
#include <gslib.h>
#include <vector>

// 戦闘テキスト表示クラス
class BattleText {
    // 戦闘テキストのデータ
    struct BattleTextData {
        int   num;
        float sec;
    };

public:
    // 初期化
    void clear();
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;

    // 表示するテキストの設定
    void set(GSuint num, float sec = 3.0f);
    // テキスト出現中か？
    bool is_appearing() const;

private:
    // 出現させるテキストの設定
    void set_text(GSuint num, float sec);
    // 待機用配列に格納
    void push_data(GSuint num, float sec);
    // 先頭のデータを削除
    void pop_data();

private:
    // 出現させるテキストの番号
    int text_num_{ -1 };
    // 現在のテキストのα値
    float text_alpha_{ -0.1f };
    // 現在のテキストの残り出現時間
    float current_limit_{ 0.0f };
    // 出現が完了しているか？
    bool is_appeared_{ false };
    // 表示を待機しているデータ
    std::vector<BattleTextData> data_{ 5, BattleTextData{ -1, 0.0f } };
};

#endif // !BATTLE_STATUS_TEXT_H_
