#include "Assets.h"
#include "PreparationAssets.h"
#include "Actor/ActorEnum.h"
#include "Camera/CameraParam.h"
#include "Camera/MainCamera.h"
#include "UI/PlayerUI.h"
#include "World/Field.h"
#include "World/Light.h"
#include "Actor/Character/Player.h"
#include "Actor/Character/Captain.h"
#include "Actor/EnemyControll/EnemySEManager.h"
#include "Actor/EnemyControll/EnemySpawner.h"
#include "Actor/Item/ItemPotSpawner.h"
#include "Actor/Navigation/ExitPoint.h"
#include "Actor/Navigation/WayPoint.h"
#include "Actor/Obstacle/InvisibleWall.h"
#include "Actor/Obstacle/WoodBox.h"

#include <GSeffect.h>
#include <GSgame.h>
#include <gslib.h>
#include <thread>

// 準備実行
void PreparationAssets::Run(IWorld* world) {
    // 準備の進捗初期化
    prepared_count_ = 0;
    set_prepare_end(false);
    gslib::Game::run_thread([=] {prepare_resource(world); });
}

// 使用リソースの準備
void PreparationAssets::prepare_resource(IWorld* world) {
    // 使用アセットの読み込み
    load_assets();

    // アクターの追加
    add_actors(world);
    // オブジェクトの追加
    add_objects(world);
#if _DEBUG
    // デバッグ用アクターの追加
    debug_add_actors(world);
#endif // _DEBUG
    // 読み込み進捗ゲージが満タンになるのを確認する用の待機
    auto sleep_gauge_full = std::chrono::milliseconds(500);
    std::this_thread::sleep_for(sleep_gauge_full);
    // 準備終了の通知を行う
    set_prepare_end(true);
}

// 使用アセットの読み込み
void PreparationAssets::load_assets() {
    // ロード画面用画像の読み込み 3
    gsLoadTexture(Texture_Load_BG, "Assets/UI/load_bg.png"); prepared_count_++;
    gsLoadTexture(Texture_Gauge_White, "Assets/UI/Gauge_White.png"); prepared_count_++;
    gsLoadTexture(Texture_Gauge_Black, "Assets/UI/Gauge_Black.png"); prepared_count_++;

    // ステージデータの読み込み 1
    csv_.load("Assets/Map/Stage1.csv"); prepared_count_++;

    // ライトマップの読み込み 1
    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt"); prepared_count_++;
    // リフレクションプローブの読み込み 1
    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt"); prepared_count_++;

    // プレイヤーの読み込み 3
    gsLoadSkinMesh(Mesh_Player, "Assets/Actors/Player/Player.mshb"); prepared_count_++;
    gsLoadMesh(Weapon_LongSword, "Assets/Actors/Player/Weapons/LongSword.mshb"); prepared_count_++;
    gsLoadMesh(Shield_SmallShield, "Assets/Actors/Player/Weapons/SmallShield.mshb"); prepared_count_++;
    // 敵の読み込み 2
    gsLoadSkinMesh(Mesh_Ranks, "Assets/Actors/Ranks/Ranks.mshb"); prepared_count_++;
    gsLoadSkinMesh(Mesh_Captain, "Assets/Actors/Captain/Captain.mshb"); prepared_count_++;
    // 障害物の読み込み 3
    gsLoadMesh(Mesh_WoodBox, "Assets/Actors/Obstacle/WoodBox.mshb"); prepared_count_++;
    gsLoadMesh(Mesh_ItemPot, "Assets/Actors/Obstacle/ItemPot.mshb"); prepared_count_++;
    gsLoadMesh(Mesh_Portion, "Assets/Actors/Obstacle/Portion.mshb"); prepared_count_++;

    // UI用画像の読み込み 9
    gsLoadTexture(Texture_HPGauge, "Assets/UI/HP_Gauge.png"); prepared_count_++;
    gsLoadTexture(Texture_SPGauge, "Assets/UI/SP_Gauge.png"); prepared_count_++;
    gsLoadTexture(Texture_Number, "Assets/UI/num.png"); prepared_count_++;
    gsLoadTexture(Texture_Text_KOCount, "Assets/UI/ko_count.png"); prepared_count_++;
    gsLoadTexture(Texture_Battle_Text, "Assets/UI/game_text.png"); prepared_count_++;
    gsLoadTexture(Texture_Game_Over, "Assets/UI/game_over.png"); prepared_count_++;
    gsLoadTexture(Texture_Map_Base, "Assets/UI/map_base.png"); prepared_count_++;
    gsLoadTexture(Texture_Map_Icon, "Assets/UI/map_icon.png"); prepared_count_++;
    gsLoadTexture(Texture_Button_Guid, "Assets/UI/button_guid.png"); prepared_count_++;


    // エフェクトの読み込み 12
    gsLoadEffect(Effect_Slash_White, "Assets/Effect/Slash03/Slash03.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_Slash_Yellow, "Assets/Effect/Slash03/Slash03_y.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_Thrusting, "Assets/Effect/Thrusting/Thrusting.efk"); prepared_count_++;
    gsLoadEffect(Effect_FireBall, "Assets/Effect/FireBall/FireBall_Orange.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_Explosion, "Assets/Effect/Explosion01/Explosion01_medium.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_Hit, "Assets/Effect/Hit02/Hit02-random-direction.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_Dash24f, "Assets/Effect/DashSonicBoom/DashSonicBoom_24f.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_GroundDust, "Assets/Effect/GroundDust/FootDust.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_GroundSmoke, "Assets/Effect/GroundDust/JumpGroundSmoke.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_GroundShock, "Assets/Effect/GroundShock/GroundShock.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_ItemPot, "Assets/Effect/MagicCircle/MagicCircle01.efkefc"); prepared_count_++;
    gsLoadEffect(Effect_Heal, "Assets/Effect/Heal/Heal02.efkefc"); prepared_count_++;

    // BGMの読み込み 2
    gsLoadBGM(BGM_Game, "Assets/Sound/BattleBGM_1Loop.ogg", GWAVE_LOOP); prepared_count_++;
    gsLoadBGM(Jingle_Death, "Assets/Sound/DeathBGM.ogg", GWAVE_DEFAULT); prepared_count_++;

    // SEの読み込み 35
    gsLoadSE(SE_Attack_Hit1, "Assets/Sound/AttackHit1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Hit2, "Assets/Sound/AttackHit2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Hit3, "Assets/Sound/AttackHit3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Miss1, "Assets/Sound/Slash1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Miss2, "Assets/Sound/Slash2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Miss3, "Assets/Sound/Slash3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Fire_Charge, "Assets/Sound/Charge.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Fire_Appaer, "Assets/Sound/Fire.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Fire_Explode, "Assets/Sound/Explosion.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Blow, "Assets/Sound/AttackBlow.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_JumpC, "Assets/Sound/JumpCLand.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_PlayerSP0, "Assets/Sound/PlayerSP0.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_PlayerSP1, "Assets/Sound/PlayerSP1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_PlayerSP2, "Assets/Sound/PlayerSP2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Damage_Player1, "Assets/Sound/PlayerDamage1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Damage_Player2, "Assets/Sound/PlayerDamage2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Damage_Player3, "Assets/Sound/PlayerDamage3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Death_Player, "Assets/Sound/PlayerDeath.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Damage_Enemy1, "Assets/Sound/EnemyDamage1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Damage_Enemy2, "Assets/Sound/EnemyDamage2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Damage_Enemy3, "Assets/Sound/EnemyDamage3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Death_Enemy1, "Assets/Sound/EnemyDeath1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Death_Enemy2, "Assets/Sound/EnemyDeath2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Death_Enemy3, "Assets/Sound/EnemyDeath3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_PlayerReact, "Assets/Sound/PlayerReact.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Pot1, "Assets/Sound/AttackPot1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Pot2, "Assets/Sound/AttackPot2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Pot3, "Assets/Sound/AttackPot3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Break_Pot1, "Assets/Sound/BreakPot1.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Break_Pot2, "Assets/Sound/BreakPot2.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Break_Pot3, "Assets/Sound/BreakPot3.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Attack_Wood, "Assets/Sound/AttackWood.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_Break_Wood, "Assets/Sound/BreakWood.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_GetItem, "Assets/Sound/GetItem.wav", 1, GWAVE_DEFAULT); prepared_count_++;
    gsLoadSE(SE_SP_Full, "Assets/Sound/SPFull.wav", 1, GWAVE_DEFAULT); prepared_count_++;

    // スカイボックスメッシュの読み込み 1
    gsLoadMeshFixed(Mesh_Skybox, "Assets/Skybox/skydome.msh"); prepared_count_++;
    // 描画用オクトリーの読み込み 1
    gsLoadOctree(Octree_Stage, "Assets/Map/stage1.oct"); prepared_count_++;
    // 衝突判定用オクトリーの読み込み 1
    gsLoadOctree(Octree_Collider, "Assets/Map/stage1_collider.oct"); prepared_count_++;
}

// アクターの追加 1
void PreparationAssets::add_actors(IWorld* world) {
    //フィールドクラスの追加
    world->add_field(new Field{ world->actor_manager(), Octree_Stage, Octree_Collider, Mesh_Skybox });
    //カメラクラスの追加
    world->add_camera(new MainCamera{ world, Main_Pos, Main_At });
    //ライトクラスの追加
    world->add_light(new Light{ world });

    // プレイヤーの追加
    world->add_actor(new Player{ world, GSvector3{0.0f, 0.0f, -5.0f} });
    // 敵ボイス再生管理クラスの追加
    world->add_actor(new EnemySEManager{});
    // UIの追加
    world->add_ui(new PlayerUI{ world, Limit_Time });
    prepared_count_++;
}

// オブジェクトの追加 4
void PreparationAssets::add_objects(IWorld* world) {
    // 敵兵士の退却地点を追加
    world->add_actor(new ExitPoint{ world, GSvector3{0.0f, 1.0f, 85.0f} });
    // 退却地点まで誘導するためのウェイポイントを追加
    world->set_size_waypoint_vector();
    world->add_waypoint(WayPoint_Count, 0, new WayPoint{ world, GSvector3{0.0f, 1.0f, 77.5f} });
    world->add_waypoint(WayPoint_Count, 1, new WayPoint{ world, GSvector3{0.0f, 1.0f, 82.5f} }); prepared_count_++;
    // ステージ上に等間隔にウェイポイントを追加する
    GSvector2 stage_size = Stage_Max - Stage_Min;
    for (int i = 0; i < WayPoint_Count; ++i) {
        for (int j = 0; j < WayPoint_Count; ++j) {
            GSvector3 position = GSvector3{
                Stage_Min.x + stage_size.x / (WayPoint_Count - 1) * i,
                1.0f,
                Stage_Min.y + stage_size.y / (WayPoint_Count - 1) * j
            };
            // ウェイポイント追加
            world->add_waypoint(i, j, new WayPoint{ world, position });
        }
    } prepared_count_++;
    // ウェイポイント区画ごとの中心座標を計算
    static_cast<World*>(world)->calc_wp_blocks_pos();

    // 移動制限用の透明な壁を設置
    for (int i = 0; i < 6; i += 2) {
        GSfloat pos_y = i;
        // プレイヤーが奥の部屋に進入できないように透明な壁を設置する
        world->add_actor(new InvisibleWall{ world, GSvector3{1.5f,  pos_y, 75.0f}, Tag_Player });
        world->add_actor(new InvisibleWall{ world, GSvector3{0.0f,  pos_y, 75.0f}, Tag_Player });
        world->add_actor(new InvisibleWall{ world, GSvector3{-1.5f, pos_y, 75.0f}, Tag_Player });

        // 敵が手前の部屋に進入できないように透明な壁を設定する
        world->add_actor(new InvisibleWall{ world, GSvector3{1.5f,  pos_y, 15.0f}, Tag_Enemy });
        world->add_actor(new InvisibleWall{ world, GSvector3{0.0f,  pos_y, 15.0f}, Tag_Enemy });
        world->add_actor(new InvisibleWall{ world, GSvector3{-1.5f, pos_y, 15.0f}, Tag_Enemy });
    } prepared_count_++;

    // その他のオブジェクトを追加する
    // ステージ左奥から追加
    GSvector2 repeat_count = Stage_Max - Stage_Min;
    for (int i = 0; i < repeat_count.x; ++i) {
        for (int j = 0; j < repeat_count.y; ++j) {
            // 出現予定の座標
            GSvector3 position = GSvector3{ Stage_Max.x - i, 0.0f, Stage_Max.y - j };
            switch (csv_.geti(j, i)) {
            case Obj_WayPoint:     // 個別追加ウェイポイント
                position.y = 1.0f;
                world->add_actor(new WayPoint{ world, position });
                break;
            case Obj_WoodBox:      // 障害物・木箱
                world->add_actor(new WoodBox{ world, position });
                break;
            case Obj_EnemySpawner: // 敵出現ポイント
                world->add_actor(new EnemySpawner{ world, position });
                break;
            case Obj_ItemPot:      // アイテム壺
                world->add_actor(new ItemPotSpawner{ world, position });
                break;
            default:
                break;
            }
        }
    } prepared_count_++;
}

// デバッグ用アクターの追加
void PreparationAssets::debug_add_actors(IWorld* world) {
}

// 準備終了フラグの設定
void PreparationAssets::set_prepare_end(bool frag) {
    std::lock_guard<std::mutex> lock(mutex_);
    is_prepare_end_ = frag;
}

// 準備終了フラグの取得
bool PreparationAssets::get_prepare_end() {
    std::lock_guard<std::mutex> lock(mutex_);
    return is_prepare_end_;
}

// 準備進捗パラメーターの取得
const int PreparationAssets::get_prepared_count() const {
    return prepared_count_;
}
