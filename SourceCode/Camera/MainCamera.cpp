#include "MainCamera.h"
#include "UserSettings.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "World/Line.h"
#include "World/Field.h"
#include <imgui/imgui.h>

// プレイヤーからの相対位置(z軸のみ)
const GSvector3 Main_PlayerOffset{ 0.0f,0.0f,-5.0f };
// カメラの注視点の補正値
const GSvector3 Main_ReferencePointOffset{ 0.0f, 1.8f, 0.0f };
// カメラの回転速度
const float Main_MoveSpeed{ 1.5f };
// カメラの補間速度
const float Main_InitializeSpeed{ 10.0f };
// x軸の回転角度の上限
const float Main_Clamp_Pitch_Max{ 30.0f };
// x軸の回転角度の下限
const float Main_Clamp_Pitch_Min{ -15.0f };

// コンストラクタ
MainCamera::MainCamera(IWorld* world, const GSvector3& position, const GSvector3& at) {
    // ワールドの設定
    world_ = world;
    // タグ名の設定
    tag_ = Tag_Camera;
    // 名前の設定
    name_ = "MainCamera";
    // 視点の位置を設定
    transform_.position(position);
    // 注視点の設定
    transform_.lookAt(at);
    // x軸周りの回転角度の初期化
    pitch_ = (at - position).getPitch();
    initial_pitch_ = pitch_;
    // y軸周りの回転角度の初期化
    yaw_ = (at - position).getYaw();
    initial_yaw_ = yaw_;
}

// TODO カメラリセットをトリガー式にする
// 更新
void MainCamera::update(float delta_time) {
#if _DEBUG
    // デバッグ表示
    disp_debug();
#endif
    // プレイヤーを検索
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return;
    // 入力を受け取りカメラを回転させる
    UserSettings settings;
    float yaw_rate = settings.Reverse_Camera_LR ? -1.0f : 1.0f;
    float pitch_rate = settings.Reverse_Camera_UB ? -1.0f : 1.0f;
    yaw_   -= input_.stick_right().x * delta_time * Main_MoveSpeed * yaw_rate;
    pitch_ += input_.stick_right().y * delta_time * Main_MoveSpeed * pitch_rate;
    // x軸周りの回転角度の制限をする
    pitch_ = CLAMP(pitch_, Main_Clamp_Pitch_Min, Main_Clamp_Pitch_Max);

    // カメラリセット
    if (input_.button_lb()) {
        // プレイヤーの前方向のベクトルを取得
        float forward = player->transform().forward().getYaw();
        // カメラをプレイヤー正面方向、縦方向の初期角度との角度差を求め、徐々に移動させる
        float diff_pitch = pitch_ - initial_pitch_;
        pitch_ = pitch_ - diff_pitch / Main_InitializeSpeed * delta_time;
        float diff_yaw = get_normalized_angle180(yaw_ - forward);
        yaw_ = yaw_ - diff_yaw / Main_InitializeSpeed * delta_time;
    }
    // 縦方向の入力が行われていない場合
    else if (input_.stick_right().y == 0 && pitch_ != initial_pitch_) {
        // 初期状態のx軸の回転角度との差を求め、徐々に元の角度に戻るように移動
        float diff = pitch_ - initial_pitch_;
        pitch_ = pitch_ - diff / Main_InitializeSpeed * delta_time;
    }

    // 注視点の座標を求める
    GSvector3 at = player->transform().position() + Main_ReferencePointOffset;
    // カメラの座標を求める
    GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * Main_PlayerOffset;

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
void MainCamera::draw() const {
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

// 正規化した角度を返す(-180～180度)
float MainCamera::get_normalized_angle180(float angle) {
    float normalized_angle = fmodf(angle + 180, 360) - 180;
    if (normalized_angle < -180) normalized_angle += 360;
    return normalized_angle;
}

// デバッグ表示
void MainCamera::disp_debug() {
    //ImGui::Begin("camera");
    //ImGui::DragFloat3("position", transform_.position());
    //ImGui::DragFloat3("rotation", transform_.rotation().getAxis());
    //ImGui::DragFloat3("localEuler", transform_.localEulerAngles());
    //ImGui::DragFloat3("Euler", transform_.eulerAngles());
    //ImGui::DragFloat2("pitch・yaw", GSvector2{ pitch_, yaw_ });
    //ImGui::DragFloat2("initial", GSvector2{ initial_pitch_, initial_yaw_ });
    //ImGui::End();

    //ImGui::Begin("input");
    //ImGui::DragFloat2("inputR", input_.stick_right());
    //ImGui::End();
}
