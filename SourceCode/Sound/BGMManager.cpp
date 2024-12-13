#include "BGMManager.h"
#include "Assets.h"

// BGMのフェードアウト速度
const float BGM_Fadeout_Speed{ 2.5f };

// インスタンス取得
BGMManager& BGMManager::get_instance() {
    static BGMManager manager;
    return manager;
}

// 更新
void BGMManager::update(float delta_time) {
    if (fading_bgm_) fade(delta_time);
    gsSetVolumeBGM(volume_bgm_);
}

// BGM再生
void BGMManager::play_bgm(GSuint num, float volume) {
    fading_bgm_ = false;
    gsSetVolumeBGM(volume * settings_.Volume_BGM);
    volume_bgm_ = volume * settings_.Volume_BGM;
    gsPlayBGM(num);
}

// BGM停止・即時
void BGMManager::stop_bgm() {
    gsStopBGM();
}

// BGM停止・フェード
void BGMManager::fade_out_bgm() {
    fading_bgm_ = true;
}

// BGMのフェードアウト処理
void BGMManager::fade(float delta_time) {
    if (volume_bgm_ > 0.0f) volume_bgm_ -= (BGM_Fadeout_Speed / 60.0f) * settings_.Volume_BGM * delta_time;
    else {
        gsStopBGM();
        fading_bgm_ = false;
    }
}

// BGMの音量を取得
float BGMManager::volume() const {
    return volume_bgm_;
}
