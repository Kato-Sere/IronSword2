#ifndef CHARACTER_PARAMERTERS_H_
#define CHARACTER_PARAMERTERS_H_

#include "AttackRate.h"
#include "BlockPower.h"
#include "ColliderParam.h"
#include "CSVReader.h"
#include "HitPoint.h"
#include "MoveSpeed.h"
#include "SpecialPoint.h"
#include "Actor/Parameter/ActionData.h"

#include <GStype.h>
#include <vector>

// キャラクターのパラメーター管理クラス
class CharacterParameters {
public:
    // コンストラクタ
    CharacterParameters(const ColliderParam& collider, const HitPoint& hp, const MoveSpeed& speed,
        const BlockPower& block, const AttackRate& rate, float jump, const std::vector<ActionData>& action_n,
        const std::vector<ActionData>& action_c, const ActionData& action_jc,
        const ActionData& action_s, const std::vector<float>& knock);

    // 衝突判定を取得
    ColliderParam& collider();
    // 体力を取得
    HitPoint& hp();
    const HitPoint& hp() const;
    // 特殊技を取得
    SpecialPoint& sp();
    // 移動速度を取得
    MoveSpeed& move_speed();
    // ガード耐久力を取得
    BlockPower& block();
    // 攻撃倍率の取得
    AttackRate& attack_rate();
    // ジャンプ力を取得
    float jump_power();
    // 弱攻撃データを取得
    const ActionData& action_normal(GSuint value) const;
    // 強攻撃データを取得
    const ActionData& action_charge(GSuint value) const;
    // ジャンプチャージ攻撃データを取得
    const ActionData& action_jumpc() const;
    // 特殊技データを取得
    const ActionData& action_special() const;
    // ノックバック速度を取得
    float knockback_speed(int value);

    // NULL
    static CharacterParameters null();

private:
    // 衝突判定
    ColliderParam collider_{ 0, 0 };
    // 体力
    HitPoint hp_{ 0 };
    // 特殊技
    SpecialPoint sp_;
    // 移動速度
    MoveSpeed move_speed_{ 0 };
    // ガード耐久力
    BlockPower block_{ 0 };
    // 攻撃倍率
    AttackRate attack_rate_{ 0 };
    // ジャンプ力
    float jump_power_{ 0 };
    // 弱攻撃データ
    std::vector<ActionData> action_normal_{ 0, ActionData{} };
    // 強攻撃データ
    std::vector<ActionData> action_charge_{ 0, ActionData{} };
    // ジャンプチャージ攻撃データ
    ActionData action_jumpc_{ ActionData{} };
    // 特殊技データ
    ActionData action_special_{ ActionData{} };
    // ノックバック速度
    std::vector<float> knockback_speed_{ 0, 0 };
};

#endif // !CHARACTER_PARAMERTERS_H_
