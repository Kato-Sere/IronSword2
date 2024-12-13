#include "Assets.h"
#include "Captain.h"
#include "DiceRoll.h"
#include "Ranks.h"
#include "Actor/ActorEnum.h"
#include "Collider/ActionCollider.h"
#include "UI/PlayerUI.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "World/Line.h"
#include "Actor/EnemyControll/EnemyConstantValue.h"

#include "Actor/State/CharaState_Attack.h"
#include "Actor/State/CharaState_BattleWalk.h"
#include "Actor/State/CharaState_BlowOff.h"
#include "Actor/State/CharaState_Damage.h"
#include "Actor/State/CharaState_Down.h"
#include "Actor/State/CharaState_Jump.h"
#include "Actor/State/CharaState_Move.h"

#include <imgui/imgui.h>
#include <thread>

// コンストラクタ
Captain::Captain(IWorld* world, const GSvector3& position) {
    world_ = world;
    mesh_ = AnimatedMesh{ Mesh_Captain, Motion_Idle };
    motion_ = Motion_Idle;
    motion_loop_ = true;
    motion_speed_ = 1.0f;
    state_timer_ = 0.0f;
    tag_ = Tag_Enemy;
    name_ = "Captain";
    owner_ = this;
    attack_tag_ = Tag_EnemyAttack;
    damage_tag_ = Tag_PlayerAttack;
    // パラメーター設定
    load_param(ParamR_Captain, name_);
    gauge_ = HPGauge{ this, Enemy_HPGauge_Color };
    // 状態の追加
    add_states(world_);
    change_state(State_Move, Motion_Idle);
    // 衝突判定球
    const GSvector3 center{ 0.0f, parameters_.collider().height(), 0.0f };
    collider_ = BoundingSphere{ parameters_.collider().radius(), center };
    spaceing_collider_ = BoundingSphere{ collider_.radius() * Spaceing_Collider_Radius_Rate, center };
    // transformの初期化
    transform_.position(position);
    transform_.rotate(GSvector3::zero());
    mesh_.transform(transform_.localToWorldMatrix());
    // 部下をゲーム上に追加
    add_soldiers(Captain_Ranks_Count);
    // アニメーションイベントを追加
    add_events(Captain_Combo);
    // 最終的な移動目標にプレイヤーの座標を設定
    final_target_ = world_->find_actor("Player");
    timer_ = gsRand(0, Set_Target_Interval);
}

// 更新
void Captain::update(float delta_time) {
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

    // 以後死亡時は処理しない
    if (is_dead()) return;

    // タイマー更新
    timer_++;
    // 自分の部隊に攻撃させる
    unit_attack();
    // 戦闘参加判定
    judge_join_battle(ReSearch_Dist, Captain_Atk_Dist);
    // プレイヤーとの距離が離れているか十分に近いときは移動目標を設定しない
    if (distance_to_player() >= Set_Target_Distance_Max || distance_to_player() < Set_Target_Distance_Min) return;
    // 数フレームに１回移動目標の設定
    if (timer_ % Set_Target_Interval == 0) {
        std::thread th(&Captain::set_target, this);
        th.detach();
    }
}

// 各状態への遷移条件
void Captain::update_state_trigger(float delta_time) {
    // 状態更新を行わない場合はすべての遷移条件を無視する
    if (!is_update_state()) {
        reset_state_trigger();
        return;
    }
    // 移動判定
    state_trigger_.Move = judge_move();
    // 弱攻撃・強攻撃の判定
    judge_attack(delta_time);
    // 起き上がり判定
    state_trigger_.React = true;
    // 戦闘時移動判定
    if (target_actor_ == nullptr) state_trigger_.BattleWalk = false;
    else state_trigger_.BattleWalk = judge_battlewalk();
}

// 体力が0になった瞬間の処理
void Captain::down() {
    if (is_downed_) return;
    // 撃破数加算
    world_->player_ui()->ko_count().add();
    // 衝突判定を行わないようにする
    enable_collider_ = false;
    // 部下のポインタを削除
    for (int i = 0, size = (int)ranks_.size(); i < size; ++i) {
        ranks_[i]->set_reterat();
        ranks_[i] = nullptr;
    }
    is_downed_ = true;
}

// 部下をリストから削除
void Captain::erase_soldier(Ranks& ranks) {
    for (auto i = ranks_.begin(), end = ranks_.end(); i != end;) {
        if (*i == &ranks) {
            i = ranks_.erase(i);
            return;
        } else ++i;
    }
}

// 状態の追加
void Captain::add_states(IWorld* world) {
    add_state(State_Attack, std::make_shared<CharaState_Attack>(this, Ranks_Combo), world);
    add_state(State_BattleWalk, std::make_shared<CharaState_BattleWalk>(this), world);
    add_state(State_BlowOff, std::make_shared<CharaState_BlowOff>(this), world);
    add_state(State_Damage, std::make_shared<CharaState_Damage>(this), world);
    add_state(State_Down, std::make_shared<CharaState_Down>(this), world);
    add_state(State_Jump, std::make_shared<CharaState_Jump>(this), world);
    add_state(State_Move, std::make_shared<CharaState_Move>(this), world);
}

// 部下を追加
void Captain::add_soldiers(int value) {
    for (int i = 0; i < value; ++i) {
        // 自分の座標を中心にランダムな位置に兵士を生成
        GSvector3 add_position = { gsRandf(Captain_Add_Ranks_Pos_Min, Captain_Add_Ranks_Pos_Max),
                                   0.0f,
                                   gsRandf(Captain_Add_Ranks_Pos_Min, Captain_Add_Ranks_Pos_Max) };
        world_->add_actor(new Ranks{ world_, transform_.position() + add_position, this });
    }
}

// 自分の部隊の攻撃処理
void Captain::unit_attack() {
    // 最終目標・移動目標のアクターのどちらかが設定されていなかったら処理しない
    if (final_target_ == nullptr || target_actor_ == nullptr) return;
    // 部隊内の攻撃予定の人数が最大人数なら処理しない
    if (attack_plan_addself() >= Captain_Max_Atk_Count) return;
    // 部隊内に攻撃できるキャラクターがいない場合は処理しない
    std::vector<Enemy*> enemies = ready_is_attack();
    if (enemies.size() == 0) return;
    // ランダムに１人攻撃予定にする
    int num = gsRand(0, enemies.size() - 1);
    enemies[num]->set_is_attack();
}

// 移動状態への遷移条件定義
bool Captain::judge_move() {
    // ジャンプ中は移動しない
    if (current_state() == State_Jump) return false;
    // 移動対象との間に壁がある場合は移動しない
    GSvector3 target_pos = target();
    target_pos.y = collider().center().y; // 高さを考慮しないため自分の高さに合わせる
    if (is_target_behind_wall(pos_coly(), target_pos)) return false;
    // 攻撃範囲内なら移動しない 目標との間に他の敵がいる場合は距離増加
    float distance = target_distance();
    float atk_dist = Captain_Atk_Dist * (between_specified_tag_count(Tag_Enemy, pos_coly(), target()) * Specified_Count_Rate + 1);
    if (distance <= atk_dist) return false;
    // 追跡開始距離以内なら移動する
    if (distance <= Captain_Search_Dist) return true;
    // 見失う距離以上なら移動しない
    if (distance > Captain_Lost_Dist) return false;
    // 残りは追跡開始距離以上見失う距離以内なので移動する
    return true;
}

// 攻撃状態への遷移条件定義
void Captain::judge_attack(float delta_time) {
    // 攻撃範囲外か？
    bool out_attack_range = target_distance() > Captain_Atk_Dist;
    if (out_attack_range) {
        state_trigger_.AttackN = false;
        state_trigger_.AttackC = false;
        return;
    }
    DiceRoll dice;
    switch (current_state_->statenum_) {
    case State_Move: case State_BattleWalk:
        // 未攻撃状態にする
        is_attacked_ = false;
        // 攻撃をする予定ならタイマー更新
        if (is_attack_) attack_timer_ -= delta_time;
        // タイマーが残っている場合は攻撃しない
        if (attack_timer_ > 0) return;
        // どちらの攻撃を行うかを決定(攻撃状態に遷移)
        state_trigger_.AttackC = dice.roll_1d100(Captain_P_of_AtkC);
        state_trigger_.AttackN = !state_trigger_.AttackC;
        // タイマー再設定
        attack_timer_ = gsRandf(Captain_Atk_Interval, Captain_Atk_Interval * 2);
        break;
    case State_Attack:
        // 一連の攻撃は終了しているか？
        if (is_attacked_) return;
        // モーション前半は処理を行わない
        if (ratio_motion_time() < 0.5f) return;
        // 攻撃を行うかを決定
        state_trigger_.AttackN = dice.roll_1d100(Captain_P_of_AtkN);
        if (!state_trigger_.AttackN) state_trigger_.AttackC = dice.roll_1d100(Captain_P_of_AtkC);
        // どの攻撃も行わない場合は終了
        if (!state_trigger_.AttackN && !state_trigger_.AttackC) {
            is_attacked_ = true;
            is_attack_ = false;
        }
        break;
    default:
        break;
    }
}

// 移動目標を決定
void Captain::set_target() {
    // 死亡している場合は処理しない
    if (is_dead_) return;

    // 最終目標までの最短距離を計算
    GSvector3 pos_final = final_target_->transform().position();
    pos_final.y = 1.0f;
    float dist_final = GSvector3::distance(pos_coly(), pos_final);

    // 索敵範囲内に移動目標がいない場合は移動しない
    if (dist_final > Captain_Search_Dist) {
        set_move_target(nullptr);
        return;
    }

    // 最終目標までの最短距離のウェイポイントを取得
    Wp_Distance wp = waypoint(Captain_Atk_Dist, final_target_);

    // 自身～最終目標の間に障害物がないか？
    bool between_obstacle = !is_between_specified_tag(Tag_Obstacle, pos_coly(), pos_final);
    // ウェイポイントより最終目標の方が近いか？
    bool closer_than_waypoint = dist_final < wp.distance_;
    // 両条件を満たしているときは最終目標、そうでない場合はウェイポイントを目標にする
    if (between_obstacle && closer_than_waypoint) set_move_target(final_target_);
    else set_move_target(wp.way_point_);
}

// 自身を含めた攻撃できるキャラクターを取得
std::vector<Enemy*> Captain::ready_is_attack() {
    std::vector<Enemy*> result;
    result.reserve(Captain_Ranks_Count + 1);
    // 目標との間に他の敵がいないか？
    bool between_other_enemy = is_between_specified_tag(Tag_Enemy, transform_.position(), target());
    // 自身は攻撃できるか？
    if (join_battle_ && !is_attack_ && !between_other_enemy) {
        result.emplace_back(this);
    }

    // 部下は攻撃できるか？
    for (int i = 0, size = (int)ranks_.size(); i < size; ++i) {
        // 攻撃に参加しているか？
        if (!ranks_[i]->join_battle()) continue;
        // 攻撃予定でないか？
        if (ranks_[i]->is_attack()) continue;
        // 目標との間に他の敵はいないか？
        if (ranks_[i]->is_between_specified_tag(Tag_Enemy, ranks_[i]->transform().position(), target())) continue;
        // ここまで来た場合は攻撃できるキャラとして扱う
        result.emplace_back(ranks_[i]);
    }
    result.shrink_to_fit();
    return result;
}

// 自身を含めた攻撃予定のキャラクターの数を取得
const int Captain::attack_plan_addself() const {
    int result = 0;
    // 自分は攻撃を行うか？
    if (is_attack_) result++;
    // 部下は攻撃を行うか？
    for (int i = 0, size = (int)ranks_.size(); i < size; ++i) {
        if (ranks_[i]->is_attack()) result++;
    }
    return result;
}

// デバッグ用更新処理
void Captain::debug_update(float delta_time) {
}
