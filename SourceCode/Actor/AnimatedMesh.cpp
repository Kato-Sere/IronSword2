#include "AnimatedMesh.h"
#include <algorithm>
#include <cmath>

// 補間フレーム数
const GSfloat LerpTime{ 10.0f };

// コンストラクタ
AnimatedMesh::AnimatedMesh(GSuint id, GSuint motion, bool loop, GSuint num_bones) :
    id_{ id },
    motion_{ motion },
    motion_timer_{ 0.0f },
    motion_speed_{ 1.0f },
    motion_loop_{ loop },
    transform_{ GS_MATRIX4_IDENTITY },
    prev_motion_{ motion },
    prev_motion_timer_{ 0.0f },
    lerp_timer_{ 0.0f },
    local_bone_matrices_{ num_bones },
    bone_matrices_{ num_bones } {
}

// 更新
void AnimatedMesh::update(float delta_time) {
    // 更新前のタイマー
    GSfloat prev_timer = motion_timer_;

    // モーションタイマーの更新
    motion_timer_ += delta_time * motion_speed_;
    // ループアニメーションか？
    if (motion_loop_) {
        // モーションタイマーをループさせる
        motion_timer_ = std::fmod(motion_timer_, current_motion_end_time());
    } else {
        // モーションタイマーをクランプする(GSLIBの仕様のため最終フレーム-1.0でクランプ)
        motion_timer_ = std::min(motion_timer_, current_motion_end_time() - 1.0f);
    }

    // 補間タイマーの更新(LerpTime以上にならないようにクランプする)
    lerp_timer_ = std::min(lerp_timer_ + delta_time, LerpTime);

    // モーションループ判定
    bool looped = motion_timer_ < prev_timer;
    // イベント発行チェック
    for (const auto& event : events_) {
        // イベント対象のモーションか？
        if (event->motion_ != motion_) continue;
        // モーションはループしているか？
        if(looped) {
            if (prev_timer < event->time_ || event->time_ <= motion_timer_) {
                event->callback_(); // コールバック処理呼び出し
            }
        } else {
            if (prev_timer < event->time_ && event->time_ <= motion_timer_) {
                event->callback_(); // コールバック処理呼び出し
            }
        }
    }
}

// 描画
void AnimatedMesh::draw() const {
    // スケルトンの自動計算を無効にする
    gsDisable(GS_CALC_SKELETON);
    // スケルトンのバインド
    gsBindSkeleton(id_);
    // ボーンの変換行列を設定
    gsSetMatrixSkeleton(bone_matrices_.data());
    // メッシュの描画
    glPushMatrix();
    // ワールド変換行列を設定する
    glMultMatrixf(transform_);
    // メッシュの描画
    gsDrawMesh(id_);
    glPopMatrix();

    // スケルトンの自動計算を有効にする
    gsEnable(GS_CALC_SKELETON);
}

// モーションの変更
void AnimatedMesh::change_motion(GSuint motion, bool loop, float speed) {
    // 現在と同じモーションの場合は何もしない
    if (motion_ == motion) return;
    // 補間中(前半)は、前回のモーションを更新しないようにする
    if (lerp_timer_ > (LerpTime * 0.5f)) {
        // 前回のモーション番号とモーションを保存
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // 補間中タイマーのリセット
        lerp_timer_ = 0.0f;
    }

    // モーションの変更
    motion_ = motion;
    // モーションタイマーの初期化
    motion_timer_ = 0.0f;
    // モーション再生速度の変更
    motion_speed_ = speed;
    // モーションループフラグの設定
    motion_loop_ = loop;
}

// モーション再生速度の変更
void AnimatedMesh::change_speed(float speed) {
    motion_speed_ = speed;
}

// 変換行列を設定する
void AnimatedMesh::transform(const GSmatrix4& matrix) {
    transform_ = matrix;
    // ボーンごとのローカル変換行列を計算
    gsCalculateAnimationLerp(
        id_, prev_motion_, prev_motion_timer_,
        id_, motion_, motion_timer_,
        lerp_timer_ / LerpTime,
        local_bone_matrices_.data()
    );

    // ボーンの変換行列を計算
    gsBindSkeleton(id_);
    gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

// モーションが終了しているか？
bool AnimatedMesh::is_end_motion() const {
    // ループのモーションは終了しない
    if (motion_loop_) return false;
    // 終了しているか？
    return motion_timer_ >= (current_motion_end_time() - 1.0f);
}

// 現在のモーションの再生時間を取得
float AnimatedMesh::motion_time() const {
    return motion_timer_;
}

// モーションの再生時間を設定
void AnimatedMesh::motion_time(float time) {
    motion_timer_ = time;
}

// 現在のモーションの終了時間を取得
float AnimatedMesh::current_motion_end_time() const {
    return gsGetEndAnimationTime(id_, motion_);
}


// 特定のモーション終了の時間を取得
float AnimatedMesh::motion_end_time(GSuint motion) const {
    return gsGetEndAnimationTime(id_, motion);
}

// ボーンのワールド変換行列を取得
GSmatrix4 AnimatedMesh::bone_matrices(int bone_no) const {
    return bone_matrices_[bone_no] * transform_;
}

// アニメーションイベント登録
void AnimatedMesh::add_event(GSuint motion, GSfloat time, std::function<void()> callback) {
    events_.push_back(new AnimationEvent{ motion, time, callback });
}

// アニメーションイベント一括削除
void AnimatedMesh::delete_event() {
    for (const auto& event : events_) {
        delete event;
    }
}
