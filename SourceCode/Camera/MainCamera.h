#ifndef MAIN_CAMERA_H_
#define MAIN_CAMERA_H_

#include "InputManager.h"
#include "Actor/Actor.h"

// 3人称カメラクラス
class MainCamera : public Actor {
public:
    // コンストラクタ
    MainCamera(IWorld* world, const GSvector3& position, const GSvector3& at);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;

private:
    // 正規化した角度を返す(-180～180度)
    static float get_normalized_angle180(float angle);
    // デバッグ表示
    void disp_debug();

private:
    // y軸周りの回転角度
    float yaw_{ 0.0f };
    // x軸周りの回転角度
    float pitch_{ 0.0f };
    // x軸回転の初期値
    float initial_pitch_;
    // y軸回転の初期値
    float initial_yaw_;
    // プレイヤーの入力
    InputManager input_;
};

#endif // !MAIN_CAMERA_H_
