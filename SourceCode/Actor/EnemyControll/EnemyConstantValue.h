#ifndef ENEMY_CONSTANT_VALUE_H_
#define ENEMY_CONSTANT_VALUE_H_

#include "Assets.h"

// 敵アクターの定数管理用ファイル

/* 全敵共通 */
//  最長の描画距離
const float Enemy_Draw_Distance{ 40.0f };
// 体力ゲージの色
const GScolor Enemy_HPGauge_Color{ 1.0f,0.1f,0.1f,1.0f };
// 体力ゲージ描画判定時の高さ補正値
const float Draw_HPGauge_Correction_Height{ 3.0f };
// 障害物に阻まれているウェイポイントに加算するコスト
const int Add_WayPoint_Cost { 100 };
// 移動目標を設定するフレーム間隔
const int Set_Target_Interval{ 20 };
// 移動目標を設定するプレイヤーとの最長距離
const float Set_Target_Distance_Max{ 25.0f };
// 移動目標を設定するプレイヤーとの最短距離
const float Set_Target_Distance_Min{ 4.0f };
// 目標との間に他の敵がいる時の攻撃距離増加率
const float Specified_Count_Rate{ 0.8f };
// 他の敵と間隔を空ける際の衝突判定球の半径の倍率
const float Spaceing_Collider_Radius_Rate{ 1.2f };
// 接近後、追跡を再開する距離
const float ReSearch_Dist{ 7.0f };
// 怯みモーションの再生速度
const float Involved_MotionSpeed{ 0.8f };

/*連続攻撃回数*/
const int Captain_Combo{ 3 };
const int Ranks_Combo{ 3 };

/* 兵長 */
// 部下の数
const int   Captain_Ranks_Count   { 9 };
// 兵士追加時の位置最小補正値
const float Captain_Add_Ranks_Pos_Min{ -4.0f };
// 兵士追加時の位置最大補正値
const float Captain_Add_Ranks_Pos_Max{ 4.0f };
// 追跡開始距離
const float Captain_Search_Dist   { 20.0f };
// 攻撃する距離
const float Captain_Atk_Dist      { 4.5f };
// 敵を見失う距離
const float Captain_Lost_Dist     { 25.0f };
// 攻撃間隔の初期値
const float Captain_Atk_Interval  { 120.0f };
// 弱攻撃を行う確率
const int   Captain_P_of_AtkN     { 50 };
// 強攻撃を行う確率
const int   Captain_P_of_AtkC     { 45 };
// １部隊当たりの最大攻撃人数
const int   Captain_Max_Atk_Count { 2 };
// 自分の攻撃確率
const int   Captain_P_of_Atk      { 25 };

/* 兵士 */
// 撤退地点への移動時の距離計算用中間地点
const GSvector3 Halfway_ExitPoint{ 0.0f, 1.0f, Stage_Max.y };
// 隊長のもとに移動する際の基準となる距離
const float Ranks_For_Captain_Dist   { 6.0f };
// 追跡開始距離
const float Ranks_Search_Dist        { 12.0f };
// 攻撃する距離
const float Ranks_Atk_Dist           { 4.5f };
// 敵を見失う距離
const float Ranks_Lost_Dist          { 16.0f };
// 攻撃間隔の初期値
const float Ranks_Atk_Interval       { 120.0f };
// 弱攻撃を行う確率
const int   Ranks_P_of_AtkN          { 30 };
// 強攻撃を行う確率
const int   Ranks_P_of_AtkC          { 20 };
// 撤退時の移動目標に撤退地点を指定する距離
const float Ranks_Pri_Dist_ExitPoint { 10.0f };

/*戦闘時移動*/
// 最長移動時間
const float BattleWalk_Timer_Max{ 120.0f };
// 最短移動時間
const float BattleWalk_Timer_Min{ 60.0f };
// 左右方向の移動倍率
const float BattleWalk_Speed_Rate_LR{ 0.15f };
// 後ろ方向の移動倍率
const float BattleWalk_Speed_Rate_B{ 0.3f };
// 前方向の移動倍率
const float BattleWalk_Speed_Rate_F{ 0.2f };
// 後ろに移動する基準の距離
const float BattleWalk_Back_Distance{ 2.0f };
// 前に移動する基準の距離
const float BattleWalk_Forward_Distance{ 4.0f };

#endif // !ENEMY_CONSTANT_VALUE_H_
