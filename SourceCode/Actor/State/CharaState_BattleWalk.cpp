#include "CharaState_BattleWalk.h"
#include "Actor/ActorEnum.h"
#include "Actor/Character/Enemy.h"
#include "World/IWorld.h"
#include "Actor/EnemyControll/EnemyConstantValue.h"

// コンストラクタ
CharaState_BattleWalk::CharaState_BattleWalk(Enemy* chara) : chara_{ chara } {}

// 開始
void CharaState_BattleWalk::start() {
    // 攻撃権を付与されているか？
    if (chara_->is_attack()) {
        // 目標から遠すぎる時は前に、そうでないときは左右に移動or移動しない
        if (is_target_too_far()) move_direction_ = Forward;
        else move_direction_ = gsRand(None, Right);
    } else {
        // 目標から近すぎる時は後ろに、そうでないときは左右に移動or移動しない
        if (is_target_too_close()) move_direction_ = Back;
        else move_direction_ = gsRand(None, Right);
    }
    // 移動時間の決定
    move_timer_ = gsRandf(BattleWalk_Timer_Min, BattleWalk_Timer_Max);
}

// 更新
void CharaState_BattleWalk::update(float delta_time) {
    move_timer_ -= delta_time;
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_BattleWalk::end() {
}

// 衝突処理
void CharaState_BattleWalk::react(Actor& other) {
    // 攻撃を受けるタグなら被ダメージ処理
    if (other.tag() == chara_->damage_tag()) chara_->take_damage(other);
}

// モーション系更新処理
void CharaState_BattleWalk::motion_update(float delta_time) {
    GSvector3 direction = GSvector3::zero();
    GSuint motion = chara_->current_motion();
    float rate = 1.0f;

    // 攻撃権を付与されているか？
    if (chara_->is_attack()) {
        // 目標に十分近い場合は移動しない
        if (!is_target_too_far()) move_direction_ = None;
    } else {
        // 目標に近すぎる時は後ろに移動する
        if (is_target_too_close()) move_direction_ = Back;
    }

    switch (move_direction_) {
    case Left:
        direction = -chara_->transform().right();
        motion = Motion_Walk_L;
        rate = BattleWalk_Speed_Rate_LR;
        break;
    case Right:
        direction = chara_->transform().right();
        motion = Motion_Walk_R;
        rate = BattleWalk_Speed_Rate_LR;
        break;
    case Back:
        direction = -chara_->transform().forward();
        motion = Motion_Walk_B;
        rate = BattleWalk_Speed_Rate_B;
        break;
    case Forward:
        direction = chara_->transform().forward();
        motion = Motion_Walk_F;
        rate = BattleWalk_Speed_Rate_F;
        break;
    case None:
        direction = GSvector3::zero();
        motion = Motion_Idle;
        rate = 0.0f;
        break;
    default:
        break;
    }
    // モーション変更
    chara_->change_motion(motion);

    // 待機モーションなら移動しない
    if (chara_->current_motion() == Motion_Idle) return;
    // 方向転換
    float angle = chara_->target_signed_angle();
    chara_->transform_.rotate(0.0f, angle, 0.0f);
    // 速度を設定し移動する
    float speed = parameters().move_speed().speed();
    chara_->velocity_ = direction * speed * rate;
    chara_->transform_.translate(chara_->velocity_ * delta_time, GStransform::Space::World);
}

// 状態遷移系更新処理
void CharaState_BattleWalk::select_state() {
    // 弱攻撃
    if (chara_->state_trigger().AttackN) {
        chara_->change_state(State_Attack, Motion_AttackNormal1, false,
            parameters().action_normal(Motion_AttackNormal1).animation_speed);
    }
    // 強攻撃
    if (chara_->state_trigger().AttackC) {
        chara_->change_state(State_Attack, Motion_AttackCharge1, false,
            parameters().action_charge(Motion_AttackCharge1).animation_speed);
    }
    // タイマー終了後または撤退中の場合、通常状態に戻る
    if (move_timer_ <= 0.0f || chara_->is_retreat()) chara_->change_state(State_Move, Motion_Idle);
}

// パラメーター取得
CharacterParameters& CharaState_BattleWalk::parameters() {
    return chara_->parameters();
}

// 目標が近すぎるか？
bool CharaState_BattleWalk::is_target_too_close() {
    return chara_->target_distance() < BattleWalk_Back_Distance;
}

// 目標が遠すぎるか？
bool CharaState_BattleWalk::is_target_too_far() {
    return chara_->target_distance() > BattleWalk_Forward_Distance;
}
