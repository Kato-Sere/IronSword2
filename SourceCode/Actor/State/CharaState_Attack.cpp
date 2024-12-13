#include "CharaState_Attack.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"

#include <stdexcept>
#include <imgui/imgui.h>

// コンストラクタ
CharaState_Attack::CharaState_Attack(Character* chara, int combo) : chara_{ chara } {
    combo_count_ = combo;
}

// 開始
void CharaState_Attack::start() {
    // 次の攻撃用データを取得
    next_data(chara_->current_motion());
}

// 更新
void CharaState_Attack::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_Attack::end() {
}

// 衝突処理
void CharaState_Attack::react(Actor& other) {
    // 攻撃を受けるタグなら被ダメージ処理
    if (other.tag() == chara_->damage_tag()) chara_->take_damage(other);
}

// 移動・モーション系更新処理
void CharaState_Attack::motion_update(float delta_time) {
    // 攻撃データを取得
    if ((int)chara_->current_motion() >= AtkC_first) use_data_ = parameters().action_charge(chara_->current_motion());
    else use_data_ = parameters().action_normal(chara_->current_motion());

    // 移動処理
    chara_->translate_attack(delta_time, use_data_);

    // 連撃判定
    if (chara_->ratio_motion_time() >= 0.1f && use_data_.sp_effect.test(SPEffect_Combo)) {
        if (chara_->state_trigger().AttackN && data_ != next_charge_) data_ = next_normal_; // 弱攻撃
        if (chara_->state_trigger().AttackC && data_ != next_normal_) data_ = next_charge_; // 強攻撃
    }
    //攻撃を行う
    if (chara_->ratio_motion_time() >= 0.6f && data_ != ActionData{}) {
        chara_->change_state(State_Attack, data_.motion, false, data_.animation_speed);
        next_data(data_.motion);
        data_ = ActionData{};
    }

}

// 状態遷移系更新処理
void CharaState_Attack::select_state() {
    // キャンセル出来るモーション中に移動の入力がある場合、モーション終了を待たずに移動状態
    float motion_end_timing = chara_->state_trigger().Move && use_data_.sp_effect.test(SPEffect_MoveCancel) ? 0.8f : 1.0f;
    // モーションが終了したら移動状態
    if (chara_->ratio_motion_time() >= motion_end_timing || chara_->mesh().is_end_motion()) chara_->change_state(State_Move, Motion_Idle);
}

// 次の連撃のデータを取得
void CharaState_Attack::next_data(GSuint motion) {
    // 通常攻撃の最終段か？
    bool final_normal = motion == AtkN_first + (combo_count_ - 1);
    // チャージ攻撃か？
    bool is_charge = motion >= AtkC_first;
    if (final_normal || is_charge) {
        // 空の攻撃データを取得
        next_normal_ = ActionData{};
        next_charge_ = ActionData{};
    } else {
        try {
            // 次の攻撃データを取得
            next_normal_ = parameters().action_normal(motion + 1);
            next_charge_ = parameters().action_charge(motion + 7);
        }
        catch (std::out_of_range&) {
            OutputDebugString(L"連撃データの読み込み時に範囲外参照されました\n");
            // 空の攻撃データを取得
            next_normal_ = ActionData{};
            next_charge_ = ActionData{};
        }
    }
}

// パラメーター取得
CharacterParameters& CharaState_Attack::parameters() {
    return chara_->parameters();
}

// デバッグ用更新処理
void CharaState_Attack::debug_update(float delta_time) {}
