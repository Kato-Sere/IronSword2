#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "IScene.h"
#include "InputManager.h"
#include "UI/FadeSwitcher.h"

// タイトルシーン
class TitleScene : public IScene {
public:
    // 開始
    virtual void start() override;
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 終了しているか？
    virtual bool is_end() const override;
    // このシーンの番号を返す
    virtual GSuint current() const override;
    // 次のシーンを返す
    virtual GSuint next() const override;
    // 終了
    virtual void end() override;

private:
    // アセット読み込み
    virtual void load_assets() override;
    // アセット削除
    virtual void delete_assets() override;

private:
    // 終了フラグ
    bool is_end_{ false };
    // フェード管理クラス
    FadeSwitcher fader_;
    // プレイヤーの入力
    InputManager input_;
    // 文字のα値
    float sin_rad_{ 0.0f };
    float text_alpha_{ 1.0f };
};

#endif // !TITLE_SCENE_H_
