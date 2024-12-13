#include "ItemPot.h"
#include "ItemPotSpawner.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"

// コンストラクタ
ItemPotSpawner::ItemPotSpawner(IWorld* world, const GSvector3& position) {
    world_ = world;
    name_ = "ItemPotSpawner";
    tag_ = Tag_ItemSpawner;
    // 衝突判定球(デバッグ用)
    collider_ = BoundingSphere{ 0.1f, GSvector3::zero() };
    // 衝突判定は無効化
    enable_collider_ = false;
    // Transformの初期化
    transform_.position(position);
}

// アイテム壺生成
void ItemPotSpawner::generate_item_pot() {
    world_->add_actor(new ItemPot{ world_, transform_.position() , this });
    is_generated_ = true;
}

// アイテム壺を生成済みか？
bool ItemPotSpawner::is_generated() {
    return is_generated_;
}

// メッセージを受信
void ItemPotSpawner::handle_massage(const std::string& message, void* param) {
    // 生成した壺が破壊されたら、生成状態を戻す
    if (param == this) {
        is_generated_ = false;
    }
}
