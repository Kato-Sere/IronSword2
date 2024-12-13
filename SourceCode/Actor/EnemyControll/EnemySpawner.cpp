#include "EnemySpawner.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"
#include "Actor/Character/Captain.h"

// 敵を生成するプレイヤーとの最短距離
const float Generate_Distance{ 30.0f };

// コンストラクタ
EnemySpawner::EnemySpawner(IWorld* world, const GSvector3& position) {
    world_ = world;
    name_ = "EnemySpawner";
    tag_ = Tag_EnemySpawner;
    // 衝突判定球(デバッグ用)
    collider_ = BoundingSphere{ 0.1f, GSvector3::zero() };
    // 衝突判定は無効化
    enable_collider_ = false;
    // transformの初期化
    transform_.position(position);
}

// 敵アクター生成
void EnemySpawner::generate_enemy() {
    // カメラ外か？
    GSvector3 center = collider_.center();
    bool out_view = gsIsSphereInsideFrustum(&center, collider_.radius());
    if (out_view) return;
    // プレイヤーとの距離が開いているか？
    Actor* player = world_->find_actor("Player");
    float distance = GSvector3::distance(player->transform().position(), transform_.position());
    bool is_near_player = distance < Generate_Distance;
    if (is_near_player) return;
    // 隊長生成
    world_->add_actor(new Captain{ world_, transform_.position() });
}
