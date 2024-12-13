#include "Assets.h"
#include "CSVReader.h"
#include "WoodBox.h"
#include "Actor/ActorEnum.h"
#include "Sound/SEManager.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "World/Line.h"

#include <GSeffect.h>

// TODO 木箱は破壊時のエフェクト作れそうなら作る

// コンストラクタ
WoodBox::WoodBox(IWorld* world, const GSvector3& position) {
    world_ = world;
    tag_ = Tag_Obstacle;
    name_ = "WoodBox";
    // パラメーターの読み込み
    CSVReader csv;
    // ファイルの読み込み
    csv.load("Assets/Actors/Data/ActorParameters.csv");
    float height = csv.getf(ParamR_WoodBox, ParamC_Height);
    float radius = csv.getf(ParamR_WoodBox, ParamC_Radius);
    hp_ = csv.geti(ParamR_WoodBox, ParamC_HP);
    // 衝突判定球
    const GSvector3 center{ 0.0f, height, 0.0f };
    collider_ = BoundingSphere{ radius * 1.1f, center };
    mesh_ = Mesh_WoodBox;
    mesh_collider_ = Mesh_WoodBox;
    // Transformの初期化
    transform_.position(position);
    transform_.rotate(GSvector3::zero());
}

// 更新
void WoodBox::update(float delta_time) {
    // フィールドとの衝突判定
    collide_field();
}

// 衝突判定
void WoodBox::react(Actor& other) {
    // 攻撃タグと衝突した時のみ体力減少
    if (other.tag() != Tag_PlayerAttack && other.tag() != Tag_EnemyAttack) return;
    hp_ -= other.attack();
    // エフェクト再生
    GSvector3 to_other;
    if (other.owner() != nullptr) to_other = -other.owner()->transform().forward();
    else to_other = -other.transform().forward();
    const GSvector3 position = GSvector3{
    transform_.position().x + to_other.x * collider_.radius(),
    collider_.center().y,
    transform_.position().z + to_other.z * collider_.radius()
    };
    gsPlayEffect(Effect_Hit, &position);

    // 体力がなくなったら死亡
    if (hp_ < 0) prepare_erase();
}

// 個別の削除処理
void WoodBox::this_prepare_erase() {
    SEManager::get_instance().play_se(SE_Break_Wood);
}

// デバッグ用更新処理
void WoodBox::debug_update(float delta_time) {
}
