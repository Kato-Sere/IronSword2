#include "Enemy.h"
#include "UserSettings.h"
#include "Actor/ActorEnum.h"
#include "World/Field.h"
#include "World/IWorld.h"
#include "World/Line.h"
#include "World/World.h"
#include "UI/PlayerUI.h"
#include "Actor/Obstacle/InvisibleWall.h"
#include "Actor/Character/CharacterConstantValue.h"
#include "Actor/EnemyControll/EnemyConstantValue.h"
#include "Actor/EnemyControll/EnemySEManager.h"

#include <chrono>
#include <iterator>
#include <imgui/imgui.h>

#define _USE_MATH_DEFINES
#include <math.h>

// コンストラクタ
Enemy::Enemy() {
    use_spaceing_ = true;
}

// 描画
void Enemy::draw() const {
    // プレイヤーがある程度近くにいる場合のみ描画
    if(distance_to_player() < Enemy_Draw_Distance) mesh_.draw();
}

// GUIの描画
void Enemy::draw_gui() const {
    // 体力ゲージを描画する設定になっていない場合は処理しない
    UserSettings settings;
    if (!settings.Draw_Enemies_HP_Gauge) return;
    // カメラが壁の向こうにいる場合は描画しない
    GSvector3 camera_pos = world_->camera()->transform().position();
    camera_pos.y = Draw_HPGauge_Correction_Height;
    GSvector3 selt_pos = pos_coly();
    selt_pos.y = Draw_HPGauge_Correction_Height;
    if (is_target_behind_wall(selt_pos, camera_pos, false)) return;
    // カメラがプレイヤーの特殊技演出用なら描画しない
    if (world_->camera()->name() == "PlayerSPCamera") return;
    // 体力が0かつダウン待機モーションになっている場合は描画しない
    if (is_dead() && motion_ == Motion_Down_Idle) return;
    // プレイヤーがある程度近くにいない場合は描画しない
    if (distance_to_player() >= Enemy_Draw_Distance) return;
    gauge_.draw();
}

// 衝突処理
void Enemy::react(Actor& other) {
    GSuint other_tag = other.tag();
    float length = collider_.radius_ + other.collider().radius_;
    // 自分の攻撃用タグ、自分と同じタグと衝突した場合は何もしない
    if (other_tag == attack_tag_ || other_tag == tag_) return;
    // 透明な壁に衝突した場合は相手の妨害タグを確認
    if (other_tag == Tag_InvisibleWall) {
        GSuint jamming = dynamic_cast<InvisibleWall*>(&other)->jamming_tag();
        if (tag() == jamming) {
            collide_actor(other, length);
            return;
        }
    }
    // 以下のタグと衝突した場合は重ならないように補正をする
    // プレイヤー、障害物、アイテム、タグなし
    if (other_tag == Tag_Player || other_tag == Tag_Obstacle ||
        other_tag == Tag_Item || other_tag == Tag_None) {
        collide_actor(other, length);
        return;
    }
    current_state_->react(other);
}

// 間隔拡張用衝突処理
void Enemy::react_spaceing(Actor& other) {
    // 自分と同じタグと衝突した時のみ処理する
    if (other.tag() != tag_ || is_dead()) return;
    // 吹き飛ばされ始めの敵と衝突した時は、ノックバックする
    if (static_cast<Character*>(&other)->current_state() == State_BlowOff) {
        GSvector3 direction = other.transform().position() - transform_.position();
        velocity_ = direction * parameters_.knockback_speed(KnockBack_Weak);
        change_state(State_Damage, Motion_Damage_Involved, false, Involved_MotionSpeed);
    } else {
        float length = spaceing_collider_.radius_ + other.spaceing_collider().radius_;
        collide_actor(other, length);
    }
}

// 移動状態時の移動処理
void Enemy::translate_move(float delta_time, float speed) {
    // ターゲット方向の角度を求める
    float angle = target_signed_angle();
    // 向きを変える
    transform_.rotate(0.0f, angle, 0.0f);
    // 移動する条件が満たされていない時は待機する
    if (!state_trigger_.Move) {
        change_state(State_Move, Motion_Idle);
        return;
    }
    // モーション変更
    change_state(State_Move, Motion_Run);
    // 前進する
    transform_.translate(0.0f, 0.0f, speed * delta_time);
}

// 攻撃状態の移動処理
void Enemy::translate_attack(float delta_time, const ActionData& actionData) {
    // ターゲット方向の角度を求める
    float angle = target_signed_angle();

    // モーションの再生割合が指定の範囲内の時は移動する
    GSvector3 move_velocity = calc_velocity(actionData.move_speed, move_direction(actionData.move_direction));
    bool move_before = ratio_motion_time() <= actionData.move_start; // 範囲の前か？
    bool move_after = ratio_motion_time() >= actionData.move_end;    // 範囲の後か？
    if (move_before || move_after) move_velocity = GSvector3::zero();

    // 向きの補間
    GSvector3 direction = move_velocity.normalized();
    GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(direction), Rotate_Speed_Attack * delta_time);
    if (move_velocity.length() > 0) transform_.rotation(rotation);

    // xz成分の更新
    velocity_ = GSvector3{ move_velocity.x, velocity_.y, move_velocity.z };
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

// 防御状態の移動処理
void Enemy::translate_block(float delta_time, float speed) {
}

// ジャンプ時の移動処理
void Enemy::translate_jump(float delta_time, float speed) {
    // 前進する
    transform_.translate(0.0f, 0.0f, speed * delta_time);
}

// 移動方向の計算
GSvector2 Enemy::move_direction(float direction) {
    // 方向をGSvector2に変換
    GSvector2 result = GSvector2::createFromDirection(direction);
    return GSvector2{ result.y, result.x };
}

// 最終目標を取得
const Actor* Enemy::final_target() {
    return final_target_;
}

// 移動目標を設定
void Enemy::set_move_target(Actor* target_actor) {
    target_actor_ = target_actor;
}

// 移動目標を設定(座標)
void Enemy::set_move_target(const GSvector3& target_position) {
    target_position_ = target_position;
}

// 攻撃する敵に設定
void Enemy::set_is_attack() {
    is_attack_ = true;
}

// 攻撃をするかを取得
const bool Enemy::is_attack() const {
    return is_attack_;
}

// 戦闘への参加判定
void Enemy::judge_join_battle(float research, float atk) {
    // 移動目標がプレイヤー以外の時は戦闘に参加していない扱い
    if (target_actor_ == nullptr || target_actor_->name() != "Player") {
        join_battle_ = false;
        return;
    }
    float distance = target_distance();
    float add_rate = (between_specified_tag_count(Tag_Enemy, pos_coly(), target()) * Specified_Count_Rate + 1);

    if (join_battle_) {
        // 再び追跡を始めるか？
        float research_dist = research * add_rate;
        if (distance > research_dist) join_battle_ = false;
    } else {
        // 攻撃できる距離か？
        float atk_dist = atk * add_rate;
        if (distance <= atk_dist) join_battle_ = true;
    }
}

// 戦闘時移動状態への遷移判定
bool Enemy::judge_battlewalk() {
    // 状態遷移をしない状態の場合は遷移しない
    if (!is_update_state()) return false;
    // 撤退時は遷移しない
    if (is_retreat()) return false;
    // 移動状態の時以外は遷移しない
    if (current_state() != State_Move) return false;
    // 目標との間に障害物がある場合は遷移しない
    if (is_between_specified_tag(Tag_Obstacle, pos_coly(), target())) return false;
    // 再追跡範囲外の時は遷移しない
    if (target_distance() > ReSearch_Dist) return false;
    // 戦闘に参加しているか？
    return join_battle_;
}

// プレイヤーとの距離を取得
const float Enemy::distance_to_player() const {
    // プレイヤーを検索
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return FLT_MAX;
    // 距離を計算
    return GSvector3::distance(player->transform().position(), transform_.position());
}

// 状況に応じた目的地を取得
const GSvector3 Enemy::target() const {
    // 移動目標のアクターが設定されている場合そのアクターの座標を返す
    if (target_actor_ != nullptr) {
        GSvector3 result = target_actor_->transform().position();
        result.y = target_actor_->collider().center().y;
        return result;
    }
    // 移動目標の座標が設定されている場合その座標を返す
    if (target_position_ != target_pos_none_) return target_position_;
    // どちらも設定されていない場合は自分の座標を返す
    return transform_.position();
}

// 戦闘に参加しているか？
const bool Enemy::join_battle() const {
    return join_battle_;
}

// 撤退中か？
const bool Enemy::is_retreat() const {
    return final_target_->name() == "ExitPoint";
}

// 移動目標までの距離を取得
const float Enemy::target_distance() const {
    return GSvector3::distance(target(), transform_.position());
}

// 目標との間に指定タグのアクターが存在しているか？
bool Enemy::is_between_specified_tag(GSuint tag, const GSvector3& from, const GSvector3& target) {
    return between_specified_tag_count(tag, from, target) != 0;
}

// 目標との間に指定タグのアクターが何体存在するか？
int Enemy::between_specified_tag_count(GSuint tag, const GSvector3& from, const GSvector3& target) {
    int result = 0;
    // 自分から移動対象に向けて線分を生成
    Line line;
    line.start_ = from;
    line.end_ = target;
    // 線分が他の敵と衝突しているか？
    for (Actor* actor : world_->find_actor_with_tag(tag)) {
        // 自分とウェイポイント以外の時のみ判定
        if (actor == this || actor->tag() == Tag_WayPoint) continue;
        // 衝突判定の結果を取得
        BoundingSphere collider = actor->collider();
        GSboolean collide = gsCollisionSphereAndLine(&collider.center_, collider.radius_,
            &line.start_, &line.end_, nullptr);
        if (collide) result++;
    }
    return result;
}

// 範囲内かつ移動目標に最も近いウェイポイントを返却する(目標Actor版)
Wp_Distance Enemy::waypoint(float attack_distance, Actor* target) {
    // 目標の座標を取得(高さのみ衝突判定の高さ)
    const GSvector3 position = GSvector3{
        target->transform().position().x,
        target->collider().center().y,
        target->transform().position().z
    };
    // ウェイポイント返却
    return waypoint(attack_distance, position);
}

// 範囲内かつ移動目標に最も近いウェイポイントを返却する(目標座標版)
Wp_Distance Enemy::waypoint(float attack_distance, const GSvector3& target) {
#if _DEBUG
    // デバッグ用　処理時間返却用
    chrono::system_clock::time_point start, wps_time, end;
    start = chrono::system_clock::now();
#endif // _DEBUG
    std::vector<Wp_Distance> wps;
    wps.reserve(WayPoint_Count * WayPoint_Count);
    // 自分の周囲のウェイポイントを取得
    World* world = static_cast<World*>(world_);
    std::vector<Actor*> waypoints = world->way_points(pos_coly());
    // 撤退時は専用のウェイポイントを追加で取得
    if (is_retreat()) {
        std::vector<Actor*> retreat_wp = world->way_points_retreat();
        copy(retreat_wp.begin(), retreat_wp.end(), back_inserter(waypoints));
    }
    // 障害物や壁に邪魔されないウェイポイントを取得
    for (Actor* waypoint : waypoints) {
        WayPoint* wp = static_cast<WayPoint*>(waypoint);
        GSvector3 wp_pos = wp->transform().position();
        // 直線上に障害物がないか？
        if (is_between_specified_tag(Tag_Obstacle, pos_coly(), wp_pos)) continue;
        // 直線上を壁でふさがれていないか？
        if (is_target_behind_wall(pos_coly(), wp_pos)) continue;
        // 自分に近すぎないか？
        float distance = GSvector3::distance(pos_coly(), wp_pos);
        if (distance < (Stage_Max.x - Stage_Min.x) / (WayPoint_Count - 1)) continue;
        // ウェイポイントとそこまでの距離を格納
        wps.push_back(Wp_Distance{ wp, distance });
    }

#if _DEBUG
    // ここまでの処理時間を取得する
    wps_time = chrono::system_clock::now();
#endif // _DEBUG

    // 最終目標の座標を取得
    GSvector3 final_pos = final_target_->transform().position();
    // 中間地点～撤退地点の距離を計算
    float half_to_final = GSvector3::distance(Halfway_ExitPoint, final_pos);

    // 距離が最短のウェイポイントを取得
    Wp_Distance result{ nullptr, FLT_MAX };
    for (Wp_Distance& wp : wps) {
        // ウェイポイントの座標を取得
        GSvector3 wp_pos = wp.way_point_->transform().position();
        // ウェイポイント～最終目標までの距離を計算
        float wp_to_final = GSvector3::distance(wp_pos, final_pos);

        // 撤退中（最終目標が撤退地点）か？
        if (is_retreat()) {
            // 中間地点よりウェイポイントが撤退地点に近いか？
            if (wp_to_final <= half_to_final) {
                // ウェイポイント～撤退地点の距離を加算
                wp.distance_ += wp_to_final;
            } else {
                // ウェイポイント～中間地点～撤退地点の距離を加算
                wp.distance_ += GSvector3::distance(wp_pos, Halfway_ExitPoint) + half_to_final;
            }
            // ウェイポイント～中間地点に障害物がある場合は距離コストを加算
            if (is_between_specified_tag(Tag_Obstacle, wp_pos, Halfway_ExitPoint)) wp.distance_ += Add_WayPoint_Cost;
        } else {
            // 最終目標との間に壁がある場合は処理しない
            if (is_target_behind_wall(wp_pos, final_pos)) continue;
            // ウェイポイント～目標に障害物がある場合は距離コストを加算
            if (is_between_specified_tag(Tag_Obstacle, wp_pos, target)) wp.distance_ += Add_WayPoint_Cost;
            // 各ウェイポイントから目標までの距離を計算
            wp.distance_ += GSvector3::distance(wp_pos, target);
        }
        // 距離が今の戻り値の距離より短い場合は入れ替え
        if (wp.distance_ < result.distance_) result = wp;
    }
#if _DEBUG
    // ここまでの処理時間を取得する
    end = chrono::system_clock::now();
    // 各区間ごとの処理時間を計算する
    float lap1 = static_cast<float>(chrono::duration_cast<chrono::microseconds>(wps_time - start).count() / 1000.0);
    float lap2 = static_cast<float>(chrono::duration_cast<chrono::microseconds>(end - wps_time).count() / 1000.0);
    float time = static_cast<float>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    ImGui::Begin("wp_times");
    ImGui::Value("lap1", lap1);
    ImGui::Value("lap2", lap2);
    ImGui::Value("time", time);
    ImGui::End();
#endif // _DEBUG
    return result;
}

// ダメージボイス再生
void Enemy::damage_voice() {
    static_cast<EnemySEManager*>(world_->find_actor("EnemySEManager"))->play_damage();
}

// 死亡時ボイス再生
void Enemy::death_voice() {
    static_cast<EnemySEManager*>(world_->find_actor("EnemySEManager"))->play_death();
}

// 特殊技ゲージ増加処理
void Enemy::add_sp_gauge(int attack, float rate) {
    // ゲージ増加
    parameters_.sp().add(attack * rate);
}

// 移動目標の角度を求める(符号あり)
const float Enemy::target_signed_angle() const {
    // 移動目標方向のベクトルを求める
    GSvector3 to_target = target() - transform_.position();
    to_target.y = 0.0f;
    // 前方向のベクトルを求める
    GSvector3 forward = transform_.forward();
    forward.y = 0.0f;
    // ２つのベクトルの角度差を求める
    return GSvector3::signedAngle(forward, to_target);
}

// 移動目標方向の角度を求める(符号なし)
const float Enemy::target_angle() const {
    return abs(target_signed_angle());
}

// 目的地が壁の向こうか？
const bool Enemy::is_target_behind_wall(const GSvector3& from, const GSvector3& target, bool consider_height) const {
    // 対象に向けて線分を生成
    Line line;
    line.start_ = from;
    line.end_ = target;
    if (consider_height) line.end_.y = collider().center().y;
    // 線分がフィールドと衝突しているか？
    return world_->field()->is_collide(line);
}

// 自身の座標に衝突判定球の高さを加えた値を取得
const GSvector3 Enemy::pos_coly() const {
    return GSvector3{
        transform_.position().x,
        collider_.center().y,
        transform_.position().z
    };
}
