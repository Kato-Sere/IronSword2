#include "SEManager.h"
#include "Assets.h"

// インスタンス取得
SEManager& SEManager::get_instance() {
    static SEManager manager;
    return manager;
}

// SE再生・通常
void SEManager::play_se(GSuint num, float volume) {
    gsSetVolumeSE(num, volume * settings_.Volume_SE);
    gsPlaySE(num);
}
