#include "Assets.h"
#include "EnemySEManager.h"
#include "Actor/ActorEnum.h"
#include "Sound/SEManager.h"

#include <imgui/imgui.h>

// ダメージボイスの種類
const int Enemy_Damage_Voice{ 3 };
// 死亡ボイスの種類
const int Enemy_Death_Voice{ 3 };

// コンストラクタ
EnemySEManager::EnemySEManager() {
    tag_ = Tag_Other;
    name_ = "EnemySEManager";
    enable_collider_ = false;
}

// 更新
void EnemySEManager::update(float delta_time) {
    // ダメージボイスが再生中か取得
    if (playing_damage_ != -1 && !gsIsPlaySE(playing_damage_)) {
        playing_damage_ = -1;
    }
    // フラグの初期化
    is_played_death_ = false;
}

// ダメージボイス再生
void EnemySEManager::play_damage() {
    // ダメージボイス再生中は処理しない
    if (playing_damage_ != -1) return;
    int num = SE_Damage_Enemy1 + gsRand(0, Enemy_Damage_Voice - 1);
    SEManager::get_instance().play_se(num);
    playing_damage_ = num;
}

// 死亡ボイス再生
void EnemySEManager::play_death() {
    // 同一フレーム内で死亡ボイスを再生している場合は処理しない
    if (is_played_death_) return;
    int num = SE_Death_Enemy1 + gsRand(0, Enemy_Death_Voice - 1);
    SEManager::get_instance().play_se(num);
    is_played_death_ = true;
}
