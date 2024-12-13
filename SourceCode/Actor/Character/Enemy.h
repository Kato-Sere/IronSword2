#ifndef ENEMY_H_
#define ENEMY_H_

#include "Assets.h"
#include "Character.h"
#include "UI/HPGauge.h"
#include "Actor/Navigation/WayPoint.h"

// ウェイポイントとそこまでの距離
struct Wp_Distance {
    WayPoint* way_point_;
    float     distance_;
};

// 敵共通クラス
class Enemy : public Character {
public:
    // コンストラクタ
    Enemy();
    // デストラクタ
    ~Enemy() = default;

    // 描画
    void draw() const override;
    // GUIの描画
    void draw_gui() const override;
    // 衝突処理
    void react(Actor& other) override;
    // 間隔拡張処理
    void react_spaceing(Actor& other) override;

    // 移動状態時の移動処理
    void translate_move(float delta_time, float speed) override;
    // 攻撃状態時の移動処理
    void translate_attack(float delta_time, const ActionData& actionData) override;
    // 防御状態の移動処理
    void translate_block(float delta_time, float speed) override;
    // ジャンプ時の移動処理
    void translate_jump(float delta_time, float speed) override;

    // 移動方向の計算
    GSvector2 move_direction(float direction) override;

    // 最終目標を取得
    const Actor* final_target();
    // 移動目標を設定(アクター)
    void set_move_target(Actor* target_actor);
    // 移動目標を設定(座標)
    void set_move_target(const GSvector3& target_position);
    // 攻撃をする敵に設定
    void set_is_attack();
    // 攻撃をするかを取得
    const bool is_attack() const;
    // 撤退中か？
    const bool is_retreat() const;

    // 目的地までの距離を取得
    const float target_distance() const;
    // 目標との間に指定タグのアクターが存在しているか？
    bool is_between_specified_tag(GSuint tag, const GSvector3& from, const GSvector3& target);
    // 目標との間に指定タグのアクターが何体存在するか？
    int between_specified_tag_count(GSuint tag, const GSvector3& from, const GSvector3& target);

    // 範囲内かつ移動目標に最も近いウェイポイントを返却する(目標Actor版)
    Wp_Distance waypoint(float attack_distance, Actor* target);
    // 範囲内かつ移動目標に最も近いウェイポイントを返却する(目標座標版)
    Wp_Distance waypoint(float attack_distance, const GSvector3& target);

    // ダメージボイス再生
    void damage_voice() override;
    // 死亡時ボイス再生
    void death_voice() override;
    // 特殊技ゲージ増加処理
    void add_sp_gauge(int attack, float rate) override;

    // 移動目標方向の角度を求める(符号あり)
    const float target_signed_angle() const;
    // 移動目標方向の角度を求める(符号なし)
    const float target_angle() const;

    // 戦闘に参加しているか？
    const bool join_battle() const;

protected:
    // 戦闘への参加判定
    void judge_join_battle(float research, float atk);
    // 戦闘時移動状態への遷移判定
    bool judge_battlewalk();
    // プレイヤーとの距離を取得
    const float distance_to_player() const;
    // 状況に応じた目的地を取得
    const GSvector3 target() const;

    // 目的地が壁の向こうか？
    const bool is_target_behind_wall(const GSvector3& from, const GSvector3& target, bool consider_height = true) const;
    // 自身の座標に衝突判定球の高さを加えた値を取得
    const GSvector3 pos_coly() const;

    // デバッグ用更新処理
    virtual void debug_update(float delta_time) = 0;

protected:
    // 攻撃をするか？
    bool is_attack_{ false };
    // 移動目標(アクター)
    Actor* target_actor_{ nullptr };
    // 移動目標(座標)
    GSvector3 target_position_{ target_pos_none_ };
    // 移動目標座標無し用
    GSvector3 target_pos_none_{ FLT_MAX, FLT_MAX, FLT_MAX };
    // 最終的な移動目標
    Actor* final_target_{ nullptr };
    // 攻撃済みフラグ
    bool is_attacked_{ false };
    // 攻撃までのタイマー
    float attack_timer_{ 60.0f };
    // 体力ゲージ
    HPGauge gauge_;
    // 戦闘に参加しているか？
    bool join_battle_{ false };
};

#endif // !ENEMY_H_
