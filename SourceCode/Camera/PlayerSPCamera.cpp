#include "CameraParam.h"
#include "PlayerSPCamera.h"
#include "MainCamera.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "World/Line.h"
#include "World/Field.h"
#include "Actor/Character/Character.h"
#include <imgui/imgui.h>

// プレイヤーからの最初の相対位置(z軸のみ)
const GSvector3 SP_Start_Offset{ 0.0f, 0.0f, -4.0f };
// プレイヤーからの最終的な相対位置(z軸のみ)
const GSvector3 SP_Final_Offset{ 0.0f, 0.0f, -2.0f };

// カメラの注視点の補正値
const GSvector3 SP_ReferencePointOffset{ 0.0f, 1.35f, 0.0f };

// カメラの回転速度
const float SP_MoveSpeed{ 10.0f };
// カメラのy軸周りの回転量
const float SP_Rotate_Yaw{ 80.0f };

// コンストラクタ
PlayerSPCamera::PlayerSPCamera(IWorld* world, const GSvector3& position, const GSvector3& at) {
    world_ = world;
    tag_ = Tag_Camera;
    name_ = "PlayerSPCamera";
    transform_.position(position);
    transform_.lookAt(at);
    pitch_ = (at - position).getPitch();
    yaw_ = (at - position).getYaw();
    final_yaw_ = yaw_ - SP_Rotate_Yaw;
    currnt_offset_ = SP_Start_Offset;
}

// 更新
void PlayerSPCamera::update(float delta_time) {
    // プレイヤーを検索
    Character* player = static_cast<Character*>(world_->find_actor("Player"));
    // プレイヤーの特殊技準備モーションが終了したらカメラを戻す
    if (player->current_motion() != Motion_AttackSpecial_Prepare) {
        // プレイヤーの正面をとらえるよう座標を計算する
        GSvector3 camera_pos = GSvector3::zero();
        camera_pos += player->transform().forward() * Main_Pos.z;
        camera_pos -= player->transform().right() * Main_Pos.x;
        camera_pos.y = Main_Pos.y;
        world_->add_camera(new MainCamera{ world_, camera_pos, Main_At });
        return;
    }

    // 最終角度との差を計算し、徐々に移動
    float diff_yaw = yaw_ - final_yaw_;
    yaw_ -= diff_yaw / SP_MoveSpeed * delta_time;
    if (yaw_ < final_yaw_) yaw_ = final_yaw_;

    // 注視点の座標を求める
    GSvector3 at = player->transform().position() + SP_ReferencePointOffset;
    // カメラの座標を求める　最終的な相対位置との差を求め、徐々に移動
    currnt_offset_ += (SP_Final_Offset - SP_Start_Offset) / 60.0f * SP_MoveSpeed * delta_time;
    if (currnt_offset_.z > SP_Final_Offset.z) currnt_offset_ = SP_Final_Offset;
    GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * currnt_offset_;

    // フィールドとの衝突判定
    Line line{ at, position };
    GSvector3 intersect;
    if (world_->field()->is_collide(line, &intersect)) {
        GSvector3 for_player = player->transform().position() - transform_.position();
        position = intersect + for_player.normalized() * 0.1f;
    }
    // 座標の設定
    transform_.position(position);
    // 注視点の方向を見る
    transform_.lookAt(at);
}

// 描画
void PlayerSPCamera::draw() const {
    GSvector3 eye = transform_.position();	   // 視点の位置
    GSvector3 at = eye + transform_.forward(); // 注視点の位置
    GSvector3 up = transform_.up();			   // 視点の上方向
    // 視点の位置
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eye.x, eye.y, eye.z, // 視点の位置
        at.x, at.y, at.z,	 // 注視点の位置
        up.x, up.y, up.z	 // 視点の上方向
    );
}
