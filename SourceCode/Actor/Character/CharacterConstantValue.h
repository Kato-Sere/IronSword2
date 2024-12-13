#ifndef CHARACTER_CONSTANT_VALUE_H_
#define CHARACTER_CONSTANT_VALUE_H_

// 全キャラクター適用の定数管理用ファイル
enum KnockBack {
    KnockBack_Weak,
    KnockBack_Strong,
    KnockBack_Dead,
};

/*防御*/
// 防御状態時の移動速度
const float Block_Move_Speed{ 0.3f };
// ノックバック減衰率
const float Blocking_KnockBack_Rate{ 0.5f };

/*ジャンプ・ジャンプ攻撃*/
// ジャンプ攻撃ができるようになるタイミング
const float JumpAttack_Timing{ 0.2f };
// ジャンプ後の着地硬直キャンセルができるようになるタイミング
const float Rigit_Cancell_Timing{ 0.5f };

/*ダメージ*/
// ノックバック減衰率
const float Damage_KnockBack_Rate{ 0.5f };

/*ダウン*/
// 消滅までの最短時間
const float DeadTimer_Min{ 30 };
// 消滅までの最長時間
const float DeadTimer_Max{ 150 };
// 立ち上がりまでの最短時間
const float React_Timer_Min{ 60 };
// 立ち上がりまでの最長時間
const float React_Timer_Max{ 150 };
// ノックバック減衰率
const float Down_KnockBack_Rate{ 0.66f };

/*吹き飛び*/
// 吹き飛ばし時のy軸方向の速度
const float BlowOff_Velocity_Y{ 0.01f };

/*旋回速度*/
// 旋回速度・移動時
const float Rotate_Speed_Move{ 12.0f };
// 旋回速度・攻撃時
const float Rotate_Speed_Attack{ 6.0f };
// 旋回速度・ジャンプ時
const float Rotate_Speed_Jump{ 12.0f };

#endif // !CHARACTER_CONSTANT_VALUE_H_
