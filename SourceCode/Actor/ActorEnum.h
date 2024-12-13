#ifndef ACTOR_ENUM_H_
#define ACTOR_ENUM_H_

// パラメーター検索用・行
enum ParamRow {
    ParamR_Player  = 1,
    ParamR_Captain = 2,
    ParamR_Ranks   = 3,
    ParamR_WoodBox = 4,
    ParamR_ItemPot = 5,
};

// パラメーター検索用・列
enum ParamColumn {
    ParamC_Name       = 0,
    ParamC_Height     = ParamC_Name + 1,
    ParamC_Radius     = ParamC_Name + 2,
    ParamC_HP         = ParamC_Name + 3,
    ParamC_MoveSpeed  = ParamC_Name + 4,
    ParamC_JumpPower  = ParamC_Name + 5,
    ParamC_BlockPower = ParamC_Name + 6,
    ParamC_ComboCount = ParamC_Name + 7,
    ParamC_SPAttack   = ParamC_Name + 8,
    ParamC_Knockback  = ParamC_Name + 9,
};

// 攻撃データ検索用・列
enum AttackData_Column {
    AtkDataC_AtkType = 1,
    AtkDataC_ColliderCount  = AtkDataC_AtkType +  1,
    AtkDataC_EffectCount    = AtkDataC_AtkType +  2,
    AtkDataC_SECount        = AtkDataC_AtkType +  3,
    AtkDataC_LifeSpan       = AtkDataC_AtkType +  4,
    AtkDataC_Attack         = AtkDataC_AtkType +  5,
    AtkDataC_Ratio          = AtkDataC_AtkType +  6,
    AtkDataC_MoveStart      = AtkDataC_AtkType +  7,
    AtkDataC_MoveEnd        = AtkDataC_AtkType +  8,
    AtkDataC_MoveSpeed      = AtkDataC_AtkType +  9,
    AtkDataC_MoveDirection  = AtkDataC_AtkType + 10,
    AtkDataC_SPEffect       = AtkDataC_AtkType + 11,
    AtkDataC_AnimationSpeed = AtkDataC_AtkType + 12,
};

// 攻撃判定データ検索用・列
enum ColliderData_Column {
    ColliderDataC_Distance  = 1,
    ColliderDataC_Radius    = ColliderDataC_Distance + 1,
    ColliderDataC_Direction = ColliderDataC_Distance + 2,
    ColliderDataC_Speed     = ColliderDataC_Distance + 3,
};

// エフェクトデータ検索用・列
enum EffectData_Column {
    EffectDataC_ID       = 1,
    EffectDataC_Timing   = EffectDataC_ID + 1,
    EffectDataC_Position = EffectDataC_ID + 2,
    EffectDataC_Rotation = EffectDataC_ID + 5,
    EffectDataC_Scale    = EffectDataC_ID + 8,
};

// SEデータ検索用・列
enum SEData_Column {
    SEDataC_ID     = 1,
    SEDataC_Timing = SEDataC_ID + 1,
};

// 2つ目以降の攻撃判定データを読みこむ時の補正値
const int Add_ColliderData_Column{ 6 };
// 2つ目以降のエフェクトデータを読み込む時の補正値
const int Add_EffectData_Column{ 13 };
// 2つ目以降のSEデータを読み込む時の補正値
const int Add_SEData_Column{ 3 };

// モーションの名称
enum ActorMotion {
    Motion_Idle,
    Motion_Run,
    Motion_Walk_L,
    Motion_Walk_R,
    Motion_Walk_B,
    Motion_Walk_F,
    Motion_AttackNormal1,
    Motion_AttackNormal2,
    Motion_AttackNormal3,
    Motion_AttackNormal4,
    Motion_AttackNormal5,
    Motion_AttackNormal6,
    Motion_AttackCharge1,
    Motion_AttackCharge2,
    Motion_AttackCharge3,
    Motion_AttackCharge4,
    Motion_AttackCharge5,
    Motion_AttackCharge6,
    Motion_AttackSpecial,
    Motion_AttackSpecial_Prepare,
    Motion_Block_In,
    Motion_Block_Idle,
    Motion_Block_Out,
    Motion_Block_Hit,
    Motion_Block_Break,
    Motion_Block_WalkF,
    Motion_Block_WalkB,
    Motion_Block_WalkL,
    Motion_Block_WalkR,
    Motion_Jump_In,
    Motion_Jump_Idle,
    Motion_Jump_Out,
    Motion_JumpC_In,
    Motion_JumpC_Out,
    Motion_Damage_Weak,
    Motion_Damage_Strong,
    Motion_Damage_Air,
    Motion_Damage_BlowOff,
    Motion_Damage_InDown,
    Motion_Damage_Involved,
    Motion_Down_In_Land,
    Motion_Down,
    Motion_Down_Idle,
    Motion_Down_Wake,
};

// 通常攻撃の基準位置
static unsigned int AtkN_first = Motion_AttackNormal1;
// チャージ攻撃の基準位置
static unsigned int AtkC_first = Motion_AttackCharge1;

// 状態
enum ActorStates {
    State_Move,
    State_Attack,
    State_Block,
    State_Jump,
    State_JumpAttack,
    State_Damage,
    State_BlowOff,
    State_Down,
    State_Special,
    State_BattleWalk,
};

// アクターのタグ
enum ActorTag {
    Tag_None,          // タグなし
    Tag_Camera,        // カメラ
    Tag_Light,         // ライト
    Tag_UI,            // ＵＩ
    Tag_Player,        // プレイヤー
    Tag_Enemy,         // 敵
    Tag_EnemyAttack,   // 敵の攻撃判定
    Tag_PlayerAttack,  // プレイヤーの攻撃判定
    Tag_WayPoint,      // ウェイポイント
    Tag_Obstacle,      // 障害物
    Tag_InvisibleWall, // 不可視の壁
    Tag_EnemySpawner,  // 敵生成
    Tag_ItemSpawner,   // アイテム壺生成
    Tag_Item,          // アイテム
    Tag_Other,         // その他
};

// 特殊効果
enum SPEffectData {
    SPEffect_Combo,      // 連撃可
    SPEffect_KnockBack,  // 強ノックバック
    SPEffect_BlowOff,    // 吹き飛ばし
    SPEffect_MoveCancel, // 移動キャンセル
    SPEffect_DownHit,    // ダウンしている敵に当たる
};

// 特殊攻撃
enum SPAttackData {
    SPAttack_Special, // 特殊技
    SPAttack_JumpC,   // ジャンプチャージ攻撃
};

// 攻撃の種類
enum AttackType {
    AtkType_Slash,      // 斬撃
    AtkType_FireBall,   // 火球
    AtkType_ShieldBash, // 盾突進
    AtkType_Blow,       // 打撃
};

// アニメーションイベントを使用して再生するSEの種類
enum SEType {
    SEType_None,       // SE再生なし
    SEType_Slash,      // 斬撃
    SEType_Fire,       // 火球
    SEType_Blow,       // 打撃
    SEType_PlayerSP,   // プレイヤーの特殊技
    SEType_JumpC,      // ジャンプチャージ攻撃
};

#endif // !ACTOR_ENUM_H_
