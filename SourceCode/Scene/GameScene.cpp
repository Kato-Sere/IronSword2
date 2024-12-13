#include "Assets.h"
#include "GameScene.h"
#include "Actor/ActorEnum.h"
#include "GameScene/PreparationAssets.h"
#include "Sound/BGMManager.h"
#include "UI/PlayerUI.h"
#include "Actor/Item/ItemPotSpawner.h"
#include "Actor/EnemyControll/EnemySpawner.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <GSeffect.h>
#include <GSgame.h>
#include <GSstandard_shader.h>
#include <imgui/imgui.h>
#include <iostream>

// 読み込み画面の背景の位置
const GSvector2 Load_BG_Position{ GSvector2::zero() };
// 進捗ゲージの座標
const GSvector2 Gauge_Position{ 0, 650 };
// 進捗ゲージのサイズ
const GSvector2 Gauge_Size{ Screen_Size.x, 10 };
// ゲームオーバー表示位置
const GSvector2 GameOver_Position{ 0.0f, 300.0f };
// ゲームオーバー用画像のフェードイン速度
const float Gameover_Fadein_Speed{ 2.5f };

// ゲーム中で使用する項目数
const int Max_Prepare_Assets{ 78 };
// ロード画面で使用する項目数
const int Assets_Loding{ 3 };

// 開始
void GameScene::start() {
    // リセット
    reset();

    // デフォルトシェーダーの初期化
    gsInitDefaultShader();
    // シャドウマップの作成
    static const GSuint shadow_map_size[] = { 2048,2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // シャドウマップ適用位置(視点からの位置)
    gsSetShadowMapDistance(60.0f);
    // カスケードシャドウマップ分割位置を調整(デフォルトは0.5)
    gsSetShadowMapCascadeLamda(0.7f);
    // シャドウの濃さ
    gsSetShadowMapAttenuation(0.0f);

    load_assets();

    // 視錐台カリングの有効化
    gsEnable(GS_FRUSTUM_CULLING);
}

// 更新
void GameScene::update(float delta_time) {
#if _DEBUG
    degug_update(delta_time);
    // 更新の処理時間計測用
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
#endif // _DEBUG
    // フェードの更新
    fader_.update(delta_time);
    switch (current_state_) {
    case State_Load:
        update_state_load(delta_time);
        break;
    case State_Game:
        update_state_game(delta_time);
        break;
    case State_GameOver:
        update_state_gameover(delta_time);
        break;
    default:
        break;
    }
#if _DEBUG
    end = chrono::system_clock::now();
    // 更新処理の所要時間をデバッグ表示
    float time = static_cast<float>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    if (time > game_time_max_) game_time_max_ = time;
    if (time < game_time_min_) game_time_min_ = time;
    ImGui::Begin("GameTime");
    ImGui::Value("time", time);
    ImGui::Value("max", game_time_max_);
    ImGui::Value("min", game_time_min_);
    if (ImGui::Button("reset")) {
        game_time_max_ = 0;
        game_time_min_ = FLT_MAX;
    }
    ImGui::End();
#endif // _DEBUG
}

// 描画
void GameScene::draw() const {
    GaugeTexture2D load_gauge{ Texture_Gauge_White, Texture_Gauge_Black, 16, 16 };
    GScolor game_over_color{ 1.0f, 1.0f, 1.0f, game_over_alpha_ };
    switch (current_state_) {
    case State_Load:
        // 背景
        gsDrawSprite2D(Texture_Load_BG, &Load_BG_Position, NULL, NULL, NULL, NULL, 0.0f);
        // 進捗ゲージ
        load_gauge.draw(Gauge_Position, Gauge_Size.x, Gauge_Size.y, pre_assets_.get_prepared_count(), Max_Prepare_Assets);
        break;
    case State_Game:
        world_.draw();
        break;
    case State_GameOver:
        world_.draw();
        // ゲームオーバー表示
        gsDrawSprite2D(Texture_Game_Over, &GameOver_Position, NULL, NULL, &game_over_color, NULL, 0.0f);
        break;
    default:
        break;
    }
    // フェードの描画
    fader_.draw();
}

// 終了しているか？
bool GameScene::is_end() const {
    return is_end_;
}

// このシーンの番号を返す
GSuint GameScene::current() const {
    return Game_Scene;
}

// 次のシーンの番号を返す
GSuint GameScene::next() const {
    // ゲームオーバー時はタイトル、それ以外はリザルトシーンに移動
    if (world_.is_game_over()) return Title_Scene;
    else return Result_Scene;
}

// 終了
void GameScene::end() {
    // 今回のリザルト用データを記録
    save_result();
    // ワールドの削除
    world_.clear();
    // アセット削除
    gsStopAllEffects();
    delete_assets();

}

// 読み込み状態の更新処理
void GameScene::update_state_load(float delta_time) {
    // 読み込み終了後の処理
    if (pre_assets_.get_prepare_end()) {
        // フェードイン開始
        fader_.start_in();
        // ステージの準備
        stage_.prepare(&world_);
        // BGM再生
        BGMManager::get_instance().play_bgm(BGM_Game);
        // フェードイン終了後、ゲーム画面に遷移
        if (fader_.is_end_fade_in()) current_state_ = State_Game;

    } else {
        // ロード画面用の画像の読み込み終了後にフェードアウト
        if (pre_assets_.get_prepared_count() > Assets_Loding) fader_.start_out();
    }
}

// ゲーム状態の更新処理
void GameScene::update_state_game(float delta_time) {
    // フェードアウト開始
    if (!is_timer_started_) fader_.start_out();
    // フェードアウトが終了し、まだタイマーが動いていないか？
    if (fader_.is_end_fade_out() && !is_timer_started_) {
        // タイマー開始
        world_.player_ui()->timer().start();
        is_timer_started_ = true;
        // 戦闘開始時のテキスト表示
        world_.player_ui()->battle_text().set(Text_Begin1);
        world_.player_ui()->battle_text().set(Text_Begin2);
    }
    // BGMの更新処理
    BGMManager::get_instance().update(delta_time);
    // ステージ管理の更新処理
    stage_.update(delta_time);
    // プレイヤーが死亡したらゲームオーバー処理実行
    if (is_dead_player()) game_over();
    // タイマー終了時テキスト表示
    if (world_.player_ui()->timer().is_time_over()) {
        // 一度だけ表示
        if (!is_appeared_end_text_) {
            world_.player_ui()->battle_text().set(Text_End);
            is_appeared_end_text_ = true;
        }
        // テキスト表示終了後フェードアウト開始
        if (!world_.player_ui()->battle_text().is_appearing()) {
            fader_.start_in();
            BGMManager::get_instance().fade_out_bgm();
            // フェードアウト終了時シーン終了
            if (fader_.is_end_fade_in()) is_end_ = true;
        }
    }
    // ワールドの更新処理
    world_.update(delta_time);
}

// ゲームオーバー状態の更新処理
void GameScene::update_state_gameover(float delta_time) {
    // 画像のα値増加
    game_over_alpha_ += (Gameover_Fadein_Speed / 60.0f) * delta_time;
    // BGMの更新処理
    BGMManager::get_instance().update(delta_time);
    // ワールドの更新処理
    world_.update(delta_time);
    // BGM再生終了もしくはいずれかのボタンが押された時フェードアウト開始
    if (input_.button_any()) is_change_title_scene_ = true;
    if (gsIsPlayBGM() && !is_change_title_scene_) return;
    fader_.start_in();
    BGMManager::get_instance().fade_out_bgm();
    // フェードアウト終了時シーン終了
    if (fader_.is_end_fade_in()) is_end_ = true;
}

// リザルトデータ記録
void GameScene::save_result() {
    std::ofstream result;
    result.open(Current_Result, std::ios::out);
    try {
        if (world_.player_ui() == nullptr) throw 0;
        result << world_.player_ui()->timer().get_elapsed() << endl;
        result << world_.player_ui()->ko_count().get() << endl;
        const Character* player = static_cast<Character*>(world_.find_actor("Player"));
        result << player->parameters().hp().damage() << endl;
    }
    catch (int) {
        for (int i = 0; i < 3; ++i) {
            result << 0 << endl;
        }
    }
    result.close();
}

// アセット読み込み
void GameScene::load_assets() {
    std::thread th(&PreparationAssets::Run, &pre_assets_, &world_);
    th.detach();
}

// アセット削除
void GameScene::delete_assets() {
    // メッシュの削除
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_Captain);
    gsDeleteMesh(Mesh_Ranks);
    gsDeleteMesh(Mesh_WoodBox);
    gsDeleteMesh(Mesh_ItemPot);
    gsDeleteMesh(Weapon_LongSword);
    gsDeleteMesh(Shield_SmallShield);
    gsDeleteMesh(Mesh_Portion);
    gsDeleteMesh(Mesh_Skybox);
    // 使用画像の削除
    gsDeleteTexture(Texture_Load_BG);
    gsDeleteTexture(Texture_Gauge_White);
    gsDeleteTexture(Texture_Gauge_Black);
    gsDeleteTexture(Texture_HPGauge);
    gsDeleteTexture(Texture_SPGauge);
    gsDeleteTexture(Texture_Text_KOCount);
    gsDeleteTexture(Texture_Button_Guid);
    // オクトリーの削除
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
    // エフェクトの削除
    gsDeleteEffect(Effect_Slash_White);
    gsDeleteEffect(Effect_Slash_Yellow);
    gsDeleteEffect(Effect_Thrusting);
    gsDeleteEffect(Effect_FireBall);
    gsDeleteEffect(Effect_Explosion);
    gsDeleteEffect(Effect_Dash24f);
    gsDeleteEffect(Effect_Hit);
    gsDeleteEffect(Effect_GroundDust);
    gsDeleteEffect(Effect_GroundSmoke);
    gsDeleteEffect(Effect_GroundShock);
    gsDeleteEffect(Effect_ItemPot);
    gsDeleteEffect(Effect_Heal);
    // サウンドの削除
    gsDeleteBGM(BGM_Game);
    gsDeleteBGM(Jingle_Death);
    gsDeleteSE(SE_Attack_Blow);
    gsDeleteSE(SE_Attack_Hit1);
    gsDeleteSE(SE_Attack_Hit2);
    gsDeleteSE(SE_Attack_Hit3);
    gsDeleteSE(SE_Attack_Miss1);
    gsDeleteSE(SE_Attack_Miss2);
    gsDeleteSE(SE_Attack_Miss3);
    gsDeleteSE(SE_Attack_Pot1);
    gsDeleteSE(SE_Attack_Pot2);
    gsDeleteSE(SE_Attack_Pot3);
    gsDeleteSE(SE_Attack_Wood);
    gsDeleteSE(SE_Break_Pot1);
    gsDeleteSE(SE_Break_Pot2);
    gsDeleteSE(SE_Break_Pot3);
    gsDeleteSE(SE_Break_Wood);
    gsDeleteSE(SE_Damage_Enemy1);
    gsDeleteSE(SE_Damage_Enemy2);
    gsDeleteSE(SE_Damage_Enemy3);
    gsDeleteSE(SE_Damage_Player1);
    gsDeleteSE(SE_Damage_Player2);
    gsDeleteSE(SE_Damage_Player3);
    gsDeleteSE(SE_Death_Enemy1);
    gsDeleteSE(SE_Death_Enemy2);
    gsDeleteSE(SE_Death_Enemy3);
    gsDeleteSE(SE_Death_Player);
    gsDeleteSE(SE_Fire_Appaer);
    gsDeleteSE(SE_Fire_Charge);
    gsDeleteSE(SE_Fire_Explode);
    gsDeleteSE(SE_GetItem);
    gsDeleteSE(SE_PlayerSP1);
    gsDeleteSE(SE_PlayerSP2);
}

// プレイヤーは死亡していないか？
bool GameScene::is_dead_player() {
    return static_cast<Character*>(world_.find_actor("Player"))->is_dead();
}

// リセット
void GameScene::reset() {
    // ゲームオーバー判定を戻す
    world_.set_game_over(false);
    // ゲームオーバー表示のα値を戻す
    game_over_alpha_ = 0.0f;
    // 終了フラグを戻す
    is_end_ = false;
    // 読み込み状態に戻す
    current_state_ = State_Load;
    // タイマーのスタート状態を戻す
    is_timer_started_ = false;
    // 終了時テキスト表示状態を戻す
    is_appeared_end_text_ = false;
    // ステージ管理の初期化
    stage_.clear();
    // タイトル画面への遷移フラグの初期化
    is_change_title_scene_ = false;
}

// ゲームオーバー処理
void GameScene::game_over() {
    BGMManager::get_instance().play_bgm(Jingle_Death);
    world_.set_game_over(true);
    current_state_ = State_GameOver;
}
