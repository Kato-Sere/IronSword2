#ifndef PLAYER_SP_CAMERA_H_
#define PLAYER_SP_CAMERA_H_

#include "Actor/Actor.h"

// プレイヤーの特殊技で使用するカメラクラス
class PlayerSPCamera : public Actor {
public:
    // コンストラクタ
    PlayerSPCamera(IWorld* world, const GSvector3& position, const GSvector3& at);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;

private:
    // デバッグ表示
    void disp_debug();

private:
    // y軸周りの回転角度
    float yaw_{ 0.0f };
    // y軸周りの回転角度の最終値
    float final_yaw_{ 0.0f };
    // x軸周りの回転角度
    float pitch_{ 0.0f };
    // 現在の相対位置
    GSvector3 currnt_offset_;
};

#endif // !PLAYER_SP_CAMERA_H_
