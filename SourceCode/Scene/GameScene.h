#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "InputManager.h"
#include "IScene.h"
#include "GameScene/PreparationAssets.h"
#include "UI/GaugeTexture2D.h"
#include "UI/FadeSwitcher.h"
#include "World/World.h"
#include "World/StageManager.h"
#include "Actor/EnemyControll/EnemySpawnManager.h"
#include "Actor/Item/ItemPotSpawnerManager.h"

// ゲームシーンクラス
class GameScene : public IScene {
    // シーンの状態
    enum Scene_State {
        State_Load,
        State_Game,
        State_GameOver,
    };
    // 戦闘テキスト
    enum Battle_Text {
        Text_Begin1, // 戦闘開始時表示
        Text_Begin2, // 戦闘開始時表示(Begin1の後)
        Text_End,    // 戦闘終了時表示
    };

public:
	//開始
	virtual void start() override;
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//終了しているか？
	virtual bool is_end() const override;
    // このシーンの番号を返す
    virtual GSuint current() const override;
	//次のシーンを返す
	virtual GSuint next() const override;
	//終了
	virtual void end() override;

private:
    // 読み込み状態の更新処理
    void update_state_load(float delta_time);
    // ゲーム状態の更新処理
    void update_state_game(float delta_time);
    // ゲームオーバー状態の更新処理
    void update_state_gameover(float delta_time);
    // リザルトデータ記録
    void save_result();
    // アセット読み込み
    virtual void load_assets() override;
    // アセット削除
    virtual void delete_assets() override;
    // プレイヤーは死亡していないか？
    bool is_dead_player();
    // リセット
    void reset();
    // ゲームオーバー時の処理
    void game_over();

private:
	// ワールドクラス
	World world_;
    // シーンの状態
    GSuint current_state_{ State_Load };
	// シーン終了フラグ
	bool is_end_{ false };
    // 使用アセット準備用
    PreparationAssets pre_assets_;
    // ステージ管理
    StageManager stage_;
    // フェード管理
    FadeSwitcher fader_;
    // タイマーはスタートしたか？
    bool is_timer_started_{ false };
    // 終了テキストは表示したか？
    bool is_appeared_end_text_{ false };
    // ゲームオーバー画像のα値
    float game_over_alpha_{ 0.0f };
    // プレイヤーの入力
    InputManager input_;
    // タイトル画面に移行するか？
    bool is_change_title_scene_{ false };
#if _DEBUG
    float game_time_max_{ 0 };
    float game_time_min_{ FLT_MAX };
#endif // _DEBUG

};

#endif // !GAME_SCENE_H_
