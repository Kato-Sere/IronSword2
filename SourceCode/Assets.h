#ifndef ASSETS_H_
#define ASSETS_H_

#include <GStype.h>
#include <string>
// 使用アセットのEnum・ゲーム全体に影響するパラメーター一覧

// 画面サイズ
const GSvector2 Screen_Size{ 1280, 720 };
// ステージの左下
const GSvector2 Stage_Min{ -27.0f, 18.0f };
// ステージの右上
const GSvector2 Stage_Max{ 27.0f, 72.0f };
// 一辺当たりのウェイポイントの設置数
// 増やすと処理の負荷が上がっていくので注意
const int WayPoint_Count{ 15 };
// ウェイポイント配列の分割数
const int WayPoint_Division_Count{ 5 };
// ゲーム画面の制限時間
const float Limit_Time{ 120.0f };
// リザルト用データ記録用ファイル
const std::string Current_Result{ "Assets/Save/Current.txt" };

// メッシュデータ
enum MeshData {
    Mesh_Player,  // プレイヤー
    Mesh_Captain, // 兵長
    Mesh_Ranks,   // 兵卒
    Mesh_WoodBox, // 木箱
    Mesh_ItemPot, // アイテム壺
    Mesh_Portion, // 回復薬
    /* 使用アクターはこの行より上に追加すること */
	Mesh_Skybox, // スカイボックス
    Weapon_LongSword,
    Shield_SmallShield,
};

// 使用するアクターの種類
const int CharacterCount = Mesh_Skybox;

// オクトリー
enum OctreeData {
	Octree_Stage,	 // ステージ用オクトリー
	Octree_Collider  // 衝突判定用オクトリー
};

// 使用画像
enum TextureData {
    Texture_Load_BG,          // 読み込み画面用背景
    Texture_Result_BG,        // リザルト画面用背景
    Texture_Gauge_White,      // 体力ゲージ白
    Texture_Gauge_Black,      // 体力ゲージ黒
    Texture_HPGauge,          // 体力ゲージ背景
    Texture_SPGauge,          // 特殊技ゲージ背景
    Texture_Number,           // 数字フォント
    Texture_Text_KOCount,     // 文字「KO.COUNT」
    Texture_Fade,             // フェード用画像
    Texture_Title_BG,         // タイトル画面用背景
    Texture_Title_Logo,       // タイトル画面用ロゴ
    Texture_Text_PressB,      // 文字「PRESS B BUTTON」
    Texture_Text_Rank,        // 順位の文字
    Texture_Text_Evaluation,  // リザルト項目の文字
    Texture_Text_Totitle,     // 文字「PRESS B TO TITLE」
    Texture_Text_KORanking,   // 文字「K.O.COUNT RANKING」
    Texture_Battle_Text,      // ゲームシーン用戦闘テキスト
    Texture_Game_Over,        // 文字「GAME OVER」
    Texture_Map_Base,         // マップの背景
    Texture_Map_Icon,         // マップのアイコン
    Texture_Ranking_Base,     // リザルト画面・ランキング部分の下地
    Texture_Button_Guid,      // ゲーム画面・操作説明
};

// 使用エフェクト
enum EffectData {
    Effect_None,         // エフェクトなし
    Effect_Slash_White,  // 斬撃(白)
    Effect_Slash_Yellow, // 斬撃(黄)
    Effect_Thrusting,    // 刺突
    Effect_FireBall,     // 火球
    Effect_Explosion,    // 小爆発
    Effect_Dash24f,      // ダッシュ(24f)
    Effect_Hit,          // ヒットエフェクト
    Effect_GroundDust,   // 土煙(小)
    Effect_GroundSmoke,  // 土煙(大)
    Effect_GroundShock,  // 地面への衝撃
    Effect_ItemPot,      // アイテム壺
    Effect_Heal,         // 回復アイテム取得
};

// 使用シーン
enum SceneData {
    Null_Scene,   // NULL
    Title_Scene,  // タイトル
    Game_Scene,   // ゲーム
    Result_Scene, // リザルト
};

// 使用BGM
enum BGMData {
    BGM_Title,    // タイトル用
    BGM_Game,     // ゲームシーン用
    BGM_Result,   // リザルトシーン用
    Jingle_Death, // 死亡用ジングル
};

// 使用SE
enum SEData {
    SE_Attack_Hit1,    // 攻撃ヒット・通常
    SE_Attack_Hit2,    // 攻撃ヒット・ピッチ低
    SE_Attack_Hit3,    // 攻撃ヒット・ピッチ高
    SE_Attack_Miss1,   // 攻撃空振り・通常
    SE_Attack_Miss2,   // 攻撃空振り・ピッチ低
    SE_Attack_Miss3,   // 攻撃空振り・ピッチ高
    SE_Attack_Wood,    // 攻撃ヒット・木箱
    SE_Attack_Blow,    // 攻撃ヒット・打撃
    SE_Attack_JumpC,   // ジャンプチャージ攻撃
    SE_Fire_Charge,    // 火球チャージ
    SE_Fire_Appaer,    // 火球射出
    SE_Fire_Explode,   // 火球爆発
    SE_PlayerSP0,      // プレイヤー特殊技・発動(効果音)
    SE_PlayerSP1,      // プレイヤー特殊技・発動(ボイス)
    SE_PlayerSP2,      // プレイヤー特殊技・〆

    SE_Attack_Pot1,    // 壺を攻撃・通常
    SE_Attack_Pot2,    // 壺を攻撃・ピッチ低
    SE_Attack_Pot3,    // 壺を攻撃・ピッチ高
    SE_Break_Pot1,     // 壺を破壊・通常
    SE_Break_Pot2,     // 壺を破壊・ピッチ低
    SE_Break_Pot3,     // 壺を破壊・ピッチ高
    SE_Break_Wood,     // 攻撃で破壊・木箱

    SE_Damage_Player1, // プレイヤーダメージ・通常
    SE_Damage_Player2, // プレイヤーダメージ・ピッチ低
    SE_Damage_Player3, // プレイヤーダメージ・ピッチ高
    SE_Damage_Enemy1,  // 敵ダメージ・通常
    SE_Damage_Enemy2,  // 敵ダメージ・ピッチ低
    SE_Damage_Enemy3,  // 敵ダメージ・ピッチ高
    SE_Death_Player,   // プレイヤー死亡
    SE_Death_Enemy1,   // 敵死亡・通常
    SE_Death_Enemy2,   // 敵死亡・ピッチ低
    SE_Death_Enemy3,   // 敵死亡・ピッチ高
    SE_PlayerReact,    // プレイヤー起き上がり

    SE_Click,          // 決定音
    SE_GetItem,        // アイテム取得
    SE_SP_Full,        // 特殊技ゲージ満タン
};

#endif // !ASSETS_H_
