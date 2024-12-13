#include "CharaState_Special.h"
#include "Camera/CameraParam.h"
#include "Camera/PlayerSPCamera.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"

#include <imgui/imgui.h>

// コンストラクタ
CharaState_Special::CharaState_Special(Character* chara) : chara_{ chara } {
}

// 開始
void CharaState_Special::start() {
    // 特殊技ゲージ初期化
    chara_->parameters().sp().reset();
    // カメラの出現座標を計算
    GSvector3 position = GSvector3::zero();
    position += chara_->transform().forward() * PlayerSP_Pos.z;
    position -= chara_->transform().right() * PlayerSP_Pos.x;
    position.y = PlayerSP_Pos.y;
    // 使用するカメラを特殊技用に変更
    world_->add_camera(new PlayerSPCamera{ world_, position, PlayerSP_At });
}

// 更新
void CharaState_Special::update(float delta_time) {
    motion_update(delta_time);
    select_state();
}

// 終了
void CharaState_Special::end() {
}

// 衝突処理
void CharaState_Special::react(Actor& other) {
}

// 移動・モーション系更新処理
void CharaState_Special::motion_update(float delta_time) {
    switch (chara_->current_motion()) {
    case Motion_AttackSpecial_Prepare:
        if (chara_->ratio_motion_time() >= 1.0f) chara_->change_motion(Motion_AttackSpecial, false);
        break;
    default:
        break;
    }
}

// 状態遷移系更新処理
void CharaState_Special::select_state() {
    switch (chara_->current_motion()) {
    case Motion_AttackSpecial:
        // モーションが終了したら移動状態
        if (chara_->ratio_motion_time() >= 1.0f) chara_->change_state(State_Move, Motion_Idle);
        break;
    default:
        break;
    }

}

// パラメーター取得
CharacterParameters& CharaState_Special::parameters() {
    return chara_->parameters();
}
