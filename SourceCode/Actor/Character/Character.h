#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Actor/Actor.h"
#include "Actor/ActorEnum.h"
#include "Actor/AnimatedMesh.h"
#include "Actor/State/ICharaState.h"
#include "Actor/State/CharaState_Null.h"
#include "Actor/Parameter/CharacterParameters.h"

#include <unordered_map>

// キャラクタークラス
class Character : public Actor {
public:
    // 状態遷移フラグ
    struct StateTrigger {
        bool Move;       // 移動
        bool AttackN;    // 弱攻撃
        bool AttackC;    // 強攻撃
        bool Block;      // 防御
        bool Jump;       // ジャンプ
        bool React;      // 起き上がり
        bool Special;    // 特殊技
        bool BattleWalk; // 戦闘時移動
    };

    // 攻撃データファイル
    enum ActionDataFile {
        File_Param,    // 基本データ
        File_Collider, // 攻撃判定
        File_Effect,   // エフェクト
        File_SE,       // 効果音
    };

    // コンストラクタ
    Character();
    // デストラクタ
    virtual ~Character() = default;

    // パラメーターの読み込み
    void load_param(GSuint param_r, const std::string& name);
    // パラメーター取得
    const CharacterParameters& parameters() const;
    CharacterParameters& parameters();

    // 個別の消滅準備
    void this_prepare_erase() override;
    // 体力が0になった瞬間の処理
    virtual void down() = 0;

    // アニメーションメッシュ取得
    const AnimatedMesh& mesh() const;
    // モーションの終了時間を取得
    const float current_motion_end_time() const;
    // 現在のアニメーションの再生割合を取得
    const float ratio_motion_time() const;

    // 状態の追加
    void add_state(const GSuint num, std::shared_ptr<ICharaState> state, IWorld* world);
    // 現在の状態を取得
    const GSuint current_state() const;
    // 状態の変更
    void change_state(GSuint state, GSuint motion, bool loop = true, float speed = 1.0f, bool reset = false);
    // モーションの変更(状態変更なし)
    void change_motion(GSuint motion, bool loop = true, float speed = 1.0f);

    // 状態ごとの経過時間取得
    const float state_timer() const;
    // 各状態に移行するか？
    virtual void update_state_trigger(float delta_time) = 0;
    // 状態遷移フラグを取得
    const StateTrigger& state_trigger() const;

    // モーション番号取得
    const GSuint current_motion() const;
    // 攻撃を受けるタグを取得
    const GSuint damage_tag() const;

    // 被ダメージ処理
    void take_damage(Actor& other);
    // ダメージボイス再生
    virtual void damage_voice() = 0;
    // 死亡時ボイス再生
    virtual void death_voice() = 0;
    // 特殊技ゲージ増加処理
    virtual void add_sp_gauge(int attack, float rate) = 0;

    // 移動状態時の移動処理
    virtual void translate_move(float delta_time, float speed) = 0;
    // 攻撃状態時の移動処理
    virtual void translate_attack(float delta_time, const ActionData& actionData) = 0;
    // 防御状態時の移動処理
    virtual void translate_block(float delta_time, float speed) = 0;
    // ジャンプ時の移動処理
    virtual void translate_jump(float delta_time, float speed) = 0;

    // 移動方向の計算
    virtual GSvector2 move_direction(float direction) = 0;
    // 移動量の計算
    GSvector3 calc_velocity(float speed, const GSvector2& direction, bool override_by_self = true);

    // アニメーションイベントを追加
    void add_events(int combo);
    // 追加するアニメーションイベントを決定
    void add_event(const ActionData& data);

    // 攻撃判定生成
    void generate_collider(const ActionData& data);
    // 火球の生成
    void generate_fireball(const ActionData& data);
    // Effekseerのエフェクトを再生
    void play_effect(const EffectParam& param);
    // Effekseerのエフェクトを再生(自身の座標指定)
    void event_effect(GSuint effect);
    // Effekseerのエフェクトを再生(色変更版)
    void play_effect_col(const EffectParam& param, const GScolor& color);
    // 斬撃音再生
    void play_slash();

    // 接地状態を取得
    bool is_land() const;
    // 状態遷移フラグの初期化
    void reset_state_trigger();

    // 状態更新を行うか？
    bool is_update_state();
    // 死亡しているか？
    bool is_dead() const;

    // コピー禁止
    Character(const Character& other) = delete;
    Character& operator = (const Character& other) = delete;

private:
    // 状態の追加
    virtual void add_states(IWorld* world) {};
    // 攻撃データ取得
    ActionData action_data(GSuint motion, int data_r, const std::vector<CSVReader>& readers);

    // モーション関係
protected:
    // アニメーションメッシュ
    AnimatedMesh             mesh_{ 0, 0, true, 256 };
    // モーション番号
    GSuint                   motion_{ 0 };
    // モーションのループ指定
    bool                     motion_loop_{ true };
    // モーション再生速度
    float                    motion_speed_{ 1.0f };
    // 状態ごとの経過時間
    float                    state_timer_{ 0.0f };
    // 重力値
    const float Gravity{ -0.008f };

    // 状態関係
protected:
    // 現在の状態
    std::shared_ptr<ICharaState>              current_state_{ nullptr };
    // 状態リスト
    std::unordered_map<GSuint, std::shared_ptr<ICharaState>> states_;
    // 状態遷移フラグ構造体
    StateTrigger              state_trigger_{ false };
    // ステータス
    CharacterParameters       parameters_{ CharacterParameters::null() };
    // 自分の攻撃判定のタグ
    GSuint                    attack_tag_{ Tag_None };
    // 攻撃を受けるタグ
    GSuint                    damage_tag_{ Tag_None };
    // 接地しているか？
    bool                      is_land_{ true };
    // 死亡しているか？
    bool                      is_dead_{ false };
    // 既にdown関数を処理したか？
    bool                      is_downed_{ false };
};

#endif // !CHARACTER_H_
