#include "Assets.h"
#include "TitleScene.h"
#include "Sound/BGMManager.h"
#include "Sound/SEManager.h"

#include <imgui/imgui.h>

// タイトルロゴの場所・拡大率
const GSvector2 Logo_Pos{ 0.0f,-40.0f };
const float Logo_Scl{ 1.0f };
// 文字の表示座標
const GSvector2 Text_Pos{ 480.0f,600.0f };
// 文字の点滅速度
const float Text_Flashing_Speed{ 0.1f };

// 開始
void TitleScene::start() {
    // アセット読み込み
    load_assets();
    // 終了フラグの初期化
    is_end_ = false;
    // フェードを初期化してフェードアウト開始
    fader_.clear();
    fader_.start_out();
    BGMManager::get_instance().play_bgm(BGM_Title);
}

// 更新
void TitleScene::update(float delta_time) {
    // BGMの音量更新
    BGMManager::get_instance().update(delta_time);

    // 決定ボタンでゲームシーンへ
    if (fader_.is_end_fade_out() && input_.button_b() && fader_.alpha() <= 0) {
        fader_.start_in();
        SEManager::get_instance().play_se(SE_Click);
        BGMManager::get_instance().fade_out_bgm();
    }
    if (fader_.is_end_fade_in() && !gsIsPlayBGM()) is_end_ = true;

    // 文字の点滅処理
    sin_rad_ += delta_time * Text_Flashing_Speed;
    text_alpha_ = (sinf(sin_rad_) + 1 ) / 2;

    // フェードの更新
    fader_.update(delta_time);
}

// 描画
void TitleScene::draw() const {
    // タイトル背景の描画
    GSvector2 bg_pos = GSvector2::zero();
    gsDrawSprite2D(Texture_Title_BG, &bg_pos, NULL, NULL, NULL, NULL, 0.0f);
    // タイトルロゴの描画
    GSvector2 logo_scale{ Logo_Scl, Logo_Scl };
    gsDrawSprite2D(Texture_Title_Logo, &Logo_Pos, NULL, NULL, NULL, &logo_scale, 0.0f);
    // 文字の描画
    GScolor text_color{ 1.0f, 1.0f,1.0f, text_alpha_ };
    gsDrawSprite2D(Texture_Text_PressB, &Text_Pos, NULL, NULL, &text_color, NULL, 0.0f);
    fader_.draw();
}

// 終了しているか？
bool TitleScene::is_end() const {
    return is_end_;
}

// このシーンの番号を返す
GSuint TitleScene::current() const {
    return Title_Scene;
}

// 次のシーンを返す
GSuint TitleScene::next() const {
    return Game_Scene;
}

// 終了
void TitleScene::end() {
    delete_assets();
}

// アセット読み込み
void TitleScene::load_assets() {
    gsLoadTexture(Texture_Fade, "Assets/UI/Fade.png");
    gsLoadTexture(Texture_Title_BG, "Assets/UI/title_bg.png");
    gsLoadTexture(Texture_Title_Logo, "Assets/UI/title_logo.png");
    gsLoadTexture(Texture_Text_PressB, "Assets/UI/title_press.png");
    gsLoadBGM(BGM_Title, "Assets/Sound/TitleBGM.ogg", GWAVE_LOOP);
    gsLoadSE(SE_Click, "Assets/Sound/Click.wav", 1, GWAVE_DEFAULT);
}

// アセット削除
void TitleScene::delete_assets() {
    gsDeleteTexture(Texture_Title_Logo);
    gsDeleteTexture(Texture_Title_BG);
    gsDeleteTexture(Texture_Text_PressB);
    gsDeleteBGM(BGM_Title);
    gsDeleteSE(SE_Click);
}
