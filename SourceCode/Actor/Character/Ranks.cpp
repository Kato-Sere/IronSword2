#include "Assets.h"
#include "DiceRoll.h"
#include "Ranks.h"
#include "Actor/ActorEnum.h"
#include "Collider/ActionCollider.h"
#include "UI/PlayerUI.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "Actor/EnemyControll/EnemyConstantValue.h"

#include "Actor/State/CharaState_Attack.h"
#include "Actor/State/CharaState_BattleWalk.h"
#include "Actor/State/CharaState_BlowOff.h"
#include "Actor/State/CharaState_Damage.h"
#include "Actor/State/CharaState_Down.h"
#include "Actor/State/CharaState_Jump.h"
#include "Actor/State/CharaState_Move.h"

#include <GSgame.h>
#include <gslib.h>
#include <imgui/imgui.h>

// コンストラクタ
Ranks::Ranks(IWorld* world, const GSvector3& position, Captain* leader) {
    world_ = world;
    mesh_ = AnimatedMesh{ Mesh_Ranks, Motion_Idle };
    motion_ = Motion_Idle;
    motion_loop_ = true;
    motion_speed_ = 1.0f;
    state_timer_ = 0.0f;
    tag_ = Tag_Enemy;
    name_ = "Ranks";
    owner_ = this;
    attack_tag_ = Tag_EnemyAttack;
    damage_tag_ = Tag_PlayerAttack;
    // パラメーター設定
    load_param(ParamR_Ranks, name_);
    gauge_ = HPGauge{ this, Enemy_HPGauge_Color };
    leader_ = leader;
    // 状態の追加
    add_states(world_);
    change_state(State_Move, Motion_Idle);
    // 衝突判定球
    const GSvector3 center{ 0.0f, parameters_.collider().height(), 0.0f };
    collider_ = BoundingSphere{ parameters_.collider().radius(), center };
    spaceing_collider_ = BoundingSphere{ collider_.radius() * Spaceing_Collider_Radius_Rate, center };
    // Transformの初期化
    transform_.position(position);
    transform_.rotate(GSvector3::zero());
    mesh_.transform(transform_.localToWorldMatrix());
    if (leader_ != nullptr) {
        // 移動目標を隊長に設定
        set_move_target(leader_);
        // 隊長の部下リストに自分を追加
        leader_->ranks_.push_back(this);
    }
    // アニメーションイベントを追加
    add_events(Ranks_Combo);
    // 最終的な移動目標にプレイヤーの座標を設定
    final_target_ = world_->find_actor("Player");
    timer_ = gsRand(0, Set_Target_Interval);
}

// 更新
void Ranks::update(float delta_time) {
    // 状態の更新
    current_state_->update(delta_time);
    state_timer_ += delta_time * motion_speed_;
    // 状態遷移フラグの更新
    update_state_trigger(delta_time);
    // 重力値を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y * delta_time, 0.0f);
    // 落下速度を制限
    velocity_.y = CLAMP(velocity_.y, -0.1f, 0.15f);
    // フィールドとの衝突判定
    collide_field();
    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_, motion_speed_);
    // メッシュのモーションを更新
    mesh_.update(delta_time);
    // ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());

    // タイマー更新
    timer_++;
    // 戦闘参加判定
    judge_join_battle(ReSearch_Dist, Ranks_Atk_Dist);

    // 撤退中ではなく、移動しないときは移動目標の更新をしない
    if (!is_retreat() && !state_trigger_.Move) return;
    if (timer_ % Set_Target_Interval == 0) { // 数フレームに１回移動目標の設定
        std::thread th(&Ranks::set_target, this);
        th.detach();
    }
}

// 各状態への遷移条件
void Ranks::update_state_trigger(float delta_time) {
    // 状態更新を行わない場合はすべての遷移条件を無視する
    if (!is_update_state()) {
        reset_state_trigger();
        return;
    }
    // 移動判定
    state_trigger_.Move = judge_move();
    // 弱攻撃・強攻撃の判定
    if (is_retreat()) {
        state_trigger_.AttackN = false;
        state_trigger_.AttackC = false;
    } else judge_attack(delta_time);

    // 起き上がり判定
    state_trigger_.React = true;
    // 戦闘時移動判定
    if (target_actor_ == nullptr || is_retreat()) state_trigger_.BattleWalk = false;
    else state_trigger_.BattleWalk = judge_battlewalk();
}

// 体力が0になった瞬間の処理
void Ranks::down() {
    if (is_downed_) return;
    // 隊長クラスの部下一覧から自分を削除
    if (leader_ != nullptr) leader_->erase_soldier(*this);
    // 撃破数加算
    world_->player_ui()->ko_count().add();
    // 衝突判定をなくす
    enable_collider_ = false;
    is_downed_ = true;
}

// 撤退の準備をする
void Ranks::set_reterat() {
    leader_ = nullptr;
    is_attack_ = false;
    final_target_ = world_->find_actor("ExitPoint");
}

// 自身の隊長が生存しているか？
bool Ranks::is_arive_leader() const {
    return leader_ != nullptr && leader_->parameters().hp().current() > 0;
}

// 攻撃状態への遷移判定・移動時
void Ranks::judge_attack_moving(float delta_time) {
    // 未攻撃状態にする
    is_attacked_ = false;
    // 攻撃をする予定ならタイマー更新
    if (is_attack_) attack_timer_ -= delta_time;
    // タイマーが残っている場合は攻撃しない
    if (attack_timer_ > 0) return;
    // どちらの攻撃を行うかを決定(攻撃状態に遷移)
    DiceRoll dice;
    state_trigger_.AttackC = dice.roll_1d100(Ranks_P_of_AtkC);
    state_trigger_.AttackN = !state_trigger_.AttackC;
    // タイマー再設定
    attack_timer_ = gsRandf(Ranks_Atk_Interval, Ranks_Atk_Interval * 2);
}

// 攻撃状態への遷移判定・攻撃時
void Ranks::judge_attack_attacking(float delta_time) {
    // 一連の攻撃は終了しているか？
    if (is_attacked_) return;
    // モーションの後半か？
    if (ratio_motion_time() < 0.5f) return;
    // 攻撃を行うかを決定
    DiceRoll dice;
    state_trigger_.AttackN = dice.roll_1d100(Ranks_P_of_AtkN);
    if (!state_trigger_.AttackN) state_trigger_.AttackC = dice.roll_1d100(Ranks_P_of_AtkC);
    // どの攻撃も行わない場合は終了
    if (!state_trigger_.AttackN && !state_trigger_.AttackC) {
        is_attacked_ = true;
        is_attack_ = false;
    }
}

// 状態の追加
void Ranks::add_states(IWorld* world) {
    add_state(State_Attack, std::make_shared<CharaState_Attack>(this, Ranks_Combo), world);
    add_state(State_BattleWalk, std::make_shared<CharaState_BattleWalk>(this), world);
    add_state(State_BlowOff, std::make_shared<CharaState_BlowOff>(this), world);
    add_state(State_Damage, std::make_shared<CharaState_Damage>(this), world);
    add_state(State_Down, std::make_shared<CharaState_Down>(this), world);
    add_state(State_Jump, std::make_shared<CharaState_Jump>(this), world);
    add_state(State_Move, std::make_shared<CharaState_Move>(this), world);
}

// 移動状態への遷移条件定義
bool Ranks::judge_move() {
    // 最終目標が退却地点の時、隊長が不在の時は常に移動する
    if (is_retreat() || leader_ == nullptr) return true;
    // ジャンプ中は移動しない
    if (current_state() == State_Jump) return false;

    // プレイヤーは索敵範囲内に存在するか？
    float dist_player = distance_to_player();
    if (dist_player <= Ranks_Search_Dist) return !join_battle_;

    // 自分～隊長間の敵数から距離の補正倍率を計算する
    float rate_leader = 1 + between_specified_tag_count(Tag_Enemy, pos_coly(), leader_->transform().position()) * Specified_Count_Rate;
    // 隊長との距離が離れている際は移動する
    float dist_leader = GSvector3::distance(pos_coly(), leader_->transform().position());
    return dist_leader > Ranks_For_Captain_Dist * rate_leader;
}

// 攻撃状態への遷移条件定義
void Ranks::judge_attack(float delta_time) {
    // 退却地点が目標の場合、戦闘に参加していない場合は処理しない
    if (is_retreat() || !join_battle_) {
        state_trigger_.AttackN = false;
        state_trigger_.AttackC = false;
        return;
    }
    // 移動目標が自分の隊長か？
    if (target_actor_ == leader_) return;
    switch (current_state_->statenum_) {
    case State_Move: case State_BattleWalk:
        judge_attack_moving(delta_time);
        break;
    case State_Attack:
        judge_attack_attacking(delta_time);
        break;
    default:
        break;
    }
}

// 移動目標を決定
void Ranks::set_target() {
    // 死亡している場合は処理しない
    if (is_dead_) return;

    // 自分～目標までの距離を計算
    GSvector3 pos_final = final_target_->transform().position();
    GSvector3 pos_self = transform_.position();
    pos_final.y = max(pos_final.y, pos_self.y);
    pos_self.y = pos_final.y;
    float dist_final = GSvector3::distance(pos_self, pos_final);

    // 隊長がいない場合
    if (leader_ == nullptr) {
        // 退却地点への距離が最短のウェイポイントを検索する
        Wp_Distance wp = waypoint(Ranks_Atk_Dist, final_target_);
        // 障害物越しの時のコストをなくす
        if (wp.distance_ > Add_WayPoint_Cost) wp.distance_ -= Add_WayPoint_Cost;

        // 撤退地点までの距離が十分に近いか？
        bool near_exit = dist_final < Ranks_Pri_Dist_ExitPoint;
        // 撤退地点は壁の向こうか？
        bool behind_wall = is_target_behind_wall(pos_coly(), final_target_->transform().position());
        // 撤退地点が近い＆壁の向こうにないときは撤退地点が目標
        if (near_exit && !behind_wall) set_move_target(final_target_);
        else set_move_target(wp.way_point_);
    }
    // 隊長がいる場合
    // 移動目標の優先度は最終目標→隊長→隊長に一番近いウェイポイント
    else {
        // 最終目標までの最短距離のウェイポイントを取得
        Wp_Distance wp_for_final = waypoint(Ranks_Atk_Dist, final_target_);
        // 自身～最終目標までの間に障害物がないか？
        bool between_obstacle_for_final = !is_between_specified_tag(Tag_Obstacle, pos_self, pos_final);
        // ウェイポイントより最終目標の方が近いか？
        bool closer_than_waypoint = dist_final < wp_for_final.distance_;
        // 両条件を満たしているときは最終目標を移動目標にする
        if (between_obstacle_for_final && closer_than_waypoint) {
            set_move_target(final_target_);
            return;
        }

        // 隊長までの最短距離のウェイポイントを取得
        Wp_Distance wp_for_leader = waypoint(Ranks_Atk_Dist, leader_);
        // 隊長の座標とそこまでの距離を取得
        GSvector3 pos_leader = leader_->transform().position();
        pos_leader.y = leader_->collider().center().y;
        float dist_leader = GSvector3::distance(pos_self, pos_leader);
        // 自身～隊長までの間に障害物がないか？
        bool between_obstacle_for_leader = !is_between_specified_tag(Tag_Obstacle, pos_self, pos_leader);
        // ウェイポイントより隊長の方が近いか？
        bool closer_than_leader = dist_leader < wp_for_leader.distance_;
        // 両条件を満たしているときは隊長を、そうでない場合は一番近いウェイポイントを目標に設定
        if (between_obstacle_for_leader && closer_than_leader) set_move_target(leader_);
        else set_move_target(wp_for_leader.way_point_);
    }
}

// デバッグ用
void Ranks::debug_update(float delta_time) {
}
