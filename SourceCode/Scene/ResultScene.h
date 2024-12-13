#ifndef RESULT_SCENE_H_
#define RESULT_SCENE_H_

#include "IScene.h"
#include "InputManager.h"
#include "ResultScene/ResultData.h"
#include "UI/FadeSwitcher.h"

// リザルトシーン
class ResultScene : public IScene {
    // リザルト用データ
    enum ResultParam {
        Time,
        KOCount,
        Damage,
    };

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
    // タイトルへの文字点滅処理
    void flashing_to_title(float delta_time);
    // 評価項目の描画
    void draw_evaluation() const;
    // 撃破数ランキングの描画
    void draw_ranking() const;

private:
    // リザルトシーン用データ
    ResultData result_data_;
    // 戦闘時間・分の部分
    int time_min_{ 0 };
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

#endif // !RESULT_SCENE_H_
