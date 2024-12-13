#include "StageManager.h"
#include "Actor/ActorEnum.h"
#include "UI/PlayerUI.h"
#include "World/World.h"

#include <imgui/imgui.h>

// 敵増援を追加する基準人数
const int Enemy_Lowest_Value{ 100 };
// アイテム壺を生成する基準人数
const int Add_ItemPot_Value{ 30 };

// 使用準備
void StageManager::prepare(IWorld* world) {
    world_ = world;
    // 敵生成地点取得
    enemy_spawners_.get_point(world_);
    // アイテム壺生成地点取得
    item_pot_spawners_.get_point(world_);
}

// 更新
void StageManager::update(float delta_time) {
    // 敵増援の追加処理
    int enemy_count = world_->find_actor_with_tag(Tag_Enemy).size();
    if (enemy_count < Enemy_Lowest_Value) {
        enemy_spawners_.generate();
    }
    // アイテム壺の追加処理
    int ko_count = world_->player_ui()->ko_count().get();
    if (ko_count >= next_generate_item_pot_) {
        item_pot_spawners_.generate(1);
        next_generate_item_pot_ += Add_ItemPot_Value;
    }
}

// 初期化
void StageManager::clear() {
    world_ = nullptr;
    // 敵生成管理クラスの初期化
    enemy_spawners_.clear();
    // アイテム壺管理クラスの初期化
    item_pot_spawners_.clear();
    // 次のアイテム壺生成撃破数を設定
    next_generate_item_pot_ = Add_ItemPot_Value;
}
