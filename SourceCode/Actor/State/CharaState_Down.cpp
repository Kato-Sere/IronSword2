#include "CharaState_Down.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "Actor/Character/CharacterConstantValue.h"

#include <imgui/imgui.h>

// コンストラクタ
CharaState_Down::CharaState_Down(Character* chara, int dead_timer) : chara_{ chara } {
    // 消滅タイマーを設定
    if (dead_timer == 0.0f) dead_timer_ = gsRandf(DeadTimer_Min, DeadTimer_Max);
    else dead_timer_ = dead_timer;
}

// 開始
void CharaState_Down::start() {
    // 死亡していないときは起き上がりタイマーを設定
    if (chara_->is_dead()) return;
    // プレイヤーは常に最短時間を設定
    if (chara_->name() == "Player") react_timer_ = React_Timer_Min;
    else react_timer_ = gsRandf(React_Timer_Min, React_Timer_Max);
}

// 更新
void CharaState_Down::update(float delta_time) {
    motion_update(delta_time);
    select_state();
    if (dead_timer_ < 0) chara_->prepare_erase();
}

// 衝突処理
void CharaState_Down::end() {}

// 衝突処理
void CharaState_Down::react(Actor& other) {
    // 攻撃を受けるタグかつダウン中も当たる攻撃なら被ダメージ処理
    if (other.tag() == chara_->damage_tag() && other.sp_effect().test(SPEffect_DownHit)) chara_->take_damage(other);
}

// モーション系更新処理
void CharaState_Down::motion_update(float delta_time) {
    // 速度を取得
    GSvector3 velocity = chara_->velocity_;
    switch (chara_->current_motion()) {
    case Motion_Damage_Air:
        if (chara_->is_land()) chara_->change_state(State_Down, Motion_Down, false);
        break;
    case Motion_Down_In_Land:
        // ノックバックさせる
        chara_->transform_.translate(velocity * delta_time, GStransform::Space::World);
        chara_->velocity_ -= GSvector3{ velocity.x, 0.0f, velocity.z } * Down_KnockBack_Rate * delta_time;
        // モーションが再生終了したら待機モーションに移行
        if (chara_->mesh().is_end_motion()) chara_->change_state(State_Down, Motion_Down, 0.3f);
        break;
    case Motion_Down:
        // ノックバックさせる
        chara_->transform_.translate(velocity * delta_time, GStransform::Space::World);
        chara_->velocity_ -= GSvector3{ velocity.x, 0.0f, velocity.z } * Down_KnockBack_Rate * delta_time;
        // モーションが再生終了したら待機モーションに移行
        if (chara_->ratio_motion_time() >= 0.9f) chara_->change_state(State_Down, Motion_Down_Idle);
        break;
    case Motion_Down_Idle:
        // 死亡しているか？
        if (chara_->is_dead()) {
            // 衝突判定を無効化し消滅タイマー更新
            dead_timer_ -= delta_time;
        } else {
            // 起き上がる
            if (chara_->state_trigger().React && react_timer_ <= 0) chara_->change_state(State_Down, Motion_Down_Wake, false);
            react_timer_ -= delta_time;
        }
        break;
    case Motion_Damage_InDown:
        // モーションが再生終了したら待機モーションに移行
        if (chara_->mesh().is_end_motion()) chara_->change_state(State_Down, Motion_Down_Idle, 0.3f);
    default:
        return;
    }
}

// 状態選択処理
void CharaState_Down::select_state() {
    switch (chara_->current_motion()) {
    case Motion_Down_Wake:
        if (chara_->mesh().is_end_motion()) chara_->change_state(State_Move, Motion_Idle);
        break;
    default:
        break;
    }
}

// パラメーター取得
CharacterParameters& CharaState_Down::parameters() {
    return chara_->parameters();
}
