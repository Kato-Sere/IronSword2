#include "Assets.h"
#include "ResultScene.h"
#include "Sound/BGMManager.h"
#include "UI/NumberTexture.h"

#include <imgui/imgui.h>

// 評価項目数
const int Evaluation_Count{ 3 };
// 数字テクスチャ
const NumberTexture Number{ Texture_Number, 32, 64 };
// 「PRESS B TO TITLE」の描画位置
const GSvector2 Totitle_Pos{ 440.0f, 630.0f };
// 「PRESS B TO TITLE」の点滅速度
const float Totitle_Flash_Speed{ 0.1f };
// 画面上に表示する「RESULT」の描画位置
const GSvector2 Result_Pos{ 470.0f, 40.0f };
// 「K.O.COUNT RANKING」の描画位置
const GSvector2 Text_Ranking_Pos { 780.0f, 180.0f };
// ランキングの下地の描画位置
const GSvector2 Ranking_Base_Pos{ 740.0f, 130.0f };

// 開始
void ResultScene::start() {
    is_end_ = false;
    load_assets();
    // 撃破数ランキングの更新
    result_data_.update_ranking();
    // フェードアウト開始
    fader_.clear();
    fader_.start_out();
    BGMManager::get_instance().play_bgm(BGM_Result);
}

// 更新
void ResultScene::update(float delta_time) {
    // 決定ボタンでゲームシーンへ
    if (fader_.is_end_fade_out() && input_.button_b()) {
        BGMManager::get_instance().fade_out_bgm();
        fader_.start_in();
        // 撃破数ランキングを保存
        result_data_.save();
    }
    if (fader_.is_end_fade_in()) is_end_ = true;

    // 文字の点滅処理
    flashing_to_title(delta_time);

    BGMManager::get_instance().update(delta_time);
    fader_.update(delta_time);
}

// 描画
void ResultScene::draw() const {
    // 背景の描画
    GSvector2 bg_pos = GSvector2::zero();
    gsDrawSprite2D(Texture_Result_BG, &bg_pos, NULL, NULL, NULL, NULL, 0.0f);
    // 評価項目の描画
    draw_evaluation();
    // 撃破数ランキングの描画
    draw_ranking();
    // 文字の描画
    GScolor text_color{ 1.0f, 1.0f, 1.0f, text_alpha_ };
    gsDrawSprite2D(Texture_Text_Totitle, &Totitle_Pos, NULL, NULL, &text_color, NULL, 0.0f);
    // フェードの描画
    fader_.draw();
}

// 終了しているか？
bool ResultScene::is_end() const {
    return is_end_;
}

// このシーンの番号を返す
GSuint ResultScene::current() const {
    return Result_Scene;
}

// 次のシーンを返す
GSuint ResultScene::next() const {
    return Title_Scene;
}

// 終了
void ResultScene::end() {
    result_data_.clear();
    delete_assets();
}

// アセット読み込み
void ResultScene::load_assets() {
    gsLoadTexture(Texture_Result_BG, "Assets/UI/result_bg.png");
    gsLoadTexture(Texture_Ranking_Base, "Assets/UI/ranking_base.png");
    gsLoadTexture(Texture_Text_Rank, "Assets/UI/rank.png");
    gsLoadTexture(Texture_Text_Evaluation, "Assets/UI/result_evaluation.png");
    gsLoadTexture(Texture_Text_Totitle, "Assets/UI/result_totitle.png");
    gsLoadTexture(Texture_Text_KORanking, "Assets/UI/result_ranking.png");
    gsLoadBGM(BGM_Result, "Assets/Sound/ResultBGM_Loop.ogg", GS_TRUE);
    // ランキングデータの読み込み
    result_data_.load();
}

// アセット削除
void ResultScene::delete_assets() {
    gsDeleteTexture(Texture_Result_BG);
    gsDeleteTexture(Texture_Ranking_Base);
    gsDeleteTexture(Texture_Text_Rank);
    gsDeleteTexture(Texture_Text_Evaluation);
    gsDeleteTexture(Texture_Text_Totitle);
    gsDeleteTexture(Texture_Text_KORanking);
    gsDeleteBGM(BGM_Result);
}

// タイトルへの文字点滅処理
void ResultScene::flashing_to_title(float delta_time) {
    sin_rad_ += delta_time * Totitle_Flash_Speed;
    text_alpha_ = (sinf(sin_rad_) + 1) / 2;
}

// 評価項目の描画
void ResultScene::draw_evaluation() const {
    for (int i = 0; i < Evaluation_Count; ++i) {
        // 表示するy座標を計算
        float pos_y = 240.0f + 120.0f * i;
        // 文字を描画
        const GSrect eval_rect{ 0.0f, 64.0f * i, 450.0f, 64.0f * (i + 1) };
        const GSvector2 eval_pos{ 50.0f, pos_y };
        gsDrawSprite2D(Texture_Text_Evaluation, &eval_pos, &eval_rect, NULL, NULL, NULL, 0.0f);

        // 数字部分の描画
        const GSvector2 scl{ 1.0f, 1.0f };
        const GSvector2 pos{ 500.0f, pos_y };
        switch (i) {
        case Time:
            Number.draw(pos, result_data_.battle_time(), scl);
            break;
        case KOCount:
            Number.draw_r(pos, result_data_.ko_kount(), 7, scl);
            break;
        case Damage:
            Number.draw_r(pos, result_data_.damage(), 7, scl);
            break;
        default:
            break;
        }
    }
}

// ランキングの描画
void ResultScene::draw_ranking() const {
    // 下地の描画
    gsDrawSprite2D(Texture_Ranking_Base, &Ranking_Base_Pos, NULL, NULL, NULL, NULL, 0.0f);
    // 見出し文字の描画
    gsDrawSprite2D(Texture_Text_KORanking, &Text_Ranking_Pos, NULL, NULL, NULL, NULL, 0.0f);

    for (int i = 0, count = result_data_.show_worst_rank(); i < count; ++i) {
        // 順位の文字を描画
        const GSrect rank_rect{ 0.0f, 64.0f * i, 120.0f, 64.0f * (i + 1) };
        const GSvector2 rank_pos{ 770.0f, 290.0f + 60.0f * i };
        gsDrawSprite2D(Texture_Text_Rank, &rank_pos, &rank_rect, NULL, NULL, NULL, 0.0f);

        // 順位ごとの数字を描画
        const GSvector2 num_pos{ 970.0f, 290.0f + 60.0f * i };
        Number.draw_r(num_pos, result_data_.ranking().at(i), 4);
    }
}
