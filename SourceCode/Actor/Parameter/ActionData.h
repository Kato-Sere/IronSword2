#ifndef ACTION_DATA_H_
#define ACTION_DATA_H_

#include <bitset>
#include <GStype.h>
#include <string>
#include <vector>

// 攻撃判定のデータ
struct AttackColliderParam {
    // 出現対象からの距離
    float distance{ 0 };
    // 攻撃判定の半径
    float radius{ 0 };
    // 出現対象からの方向
    float direction{ 0 };
    // 攻撃判定の速度
    float speed{ 0 };
};

// 攻撃エフェクトのデータ
struct EffectParam {
    // 使用エフェクト
    GSuint    effect_id{ 0 };
    // 出現フレーム
    int       appear_frame{ 0 };
    // 出現ローカル座標
    GSvector3 local_position{ 0.0f, 0.0f, 0.0f };
    // ローカル回転角
    GSvector3 local_rotation{ 0.0f, 0.0f, 0.0f };
    // ローカル拡大率
    GSvector3 local_scale{ 1.0f, 1.0f, 1.0f };
};

// SEのデータ
struct SEParam {
    // 使用SE
    GSuint se_id{ 0 };
    // 再生タイミング
    int play_frame{ 0 };
};

// 攻撃データ
struct ActionData {
    // モーション番号
    GSuint                    motion{ 0 };
    // 攻撃の種類
    GSuint                    attack_type{ 0 };
    // 攻撃判定の個数
    int                       collider_count{ 0 };
    // 攻撃判定のデータ
    std::vector<AttackColliderParam> collider_data{ 0, AttackColliderParam{} };
    // 攻撃エフェクトの個数
    int                       effect_count{ 0 };
    // 攻撃エフェクトのデータ
    std::vector<EffectParam>  effect_data{ 0, EffectParam{} };
    // 再生するSEの個数
    int                       se_count{ 0 };
    // 再生するSEのデータ
    std::vector<SEParam>      se_data{ 0, SEParam{} };
    // 攻撃判定の寿命
    float                     lifespan{ 0 };
    // 攻撃力
    int                       attack{ 0 };
    // 攻撃判定のタイミング
    int                       collider_timing{ 0 };
    // 移動開始タイミング
    float                     move_start{ 0 };
    // 移動終了タイミング
    float                     move_end{ 0 };
    // 移動速度
    float                     move_speed{ 0 };
    // 移動方向
    float                     move_direction{ 0 };
    // 特殊効果
    std::bitset<8>            sp_effect{ std::bitset<8>(0) };
    // モーションの再生速度
    float                     animation_speed{ 0 };

    // 比較演算子のオーバーロード
    bool operator ==(const ActionData& data) const {
        return motion == data.motion;
    }
    bool operator !=(const ActionData& data) const {
        return motion != data.motion;
    }
};

#endif // !ACTION_DATA_H_
