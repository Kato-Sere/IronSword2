#include "Assets.h"
#include "CSVReader.h"
#include "ItemPot.h"
#include "Portion.h"
#include "Actor/ActorEnum.h"
#include "Sound/SEManager.h"
#include "World/IWorld.h"

#include <GSeffect.h>

// 壺破壊時のSEの種類
const int Break_Pot_SE{ 3 };

// コンストラクタ
ItemPot::ItemPot(IWorld* world, const GSvector3& position, ItemPotSpawner* spawner) {
    world_ = world;
    tag_ = Tag_Item;
    name_ = "ItemPot";
    spawner_ = spawner;
    // パラメーターの読み込み
    CSVReader csv;
    // ファイルの読み込み
    csv.load("Assets/Actors/Data/ActorParameters.csv");
    float height = csv.getf(ParamR_ItemPot, ParamC_Height);
    float radius = csv.getf(ParamR_ItemPot, ParamC_Radius);
    hp_ = csv.geti(ParamR_ItemPot, ParamC_HP);
    // 衝突判定球
    const GSvector3 center{ 0.0f, height, 0.0f };
    collider_ = BoundingSphere{ radius, center };
    // Transformの初期化
    transform_.position(position);
    transform_.rotate(GSvector3::zero());
    // エフェクト再生
    const GSvector3 pos = transform_.position();
    effect_handle_ = gsPlayEffect(Effect_ItemPot, &pos);
}

// デストラクタ
ItemPot::~ItemPot() {
    // エフェクト停止
    gsStopEffect(effect_handle_);
}

// 更新
void ItemPot::update(float delta_time) {
    // フィールドとの衝突判定
    collide_field();
}

// 描画
void ItemPot::draw() const {
    // メッシュの描画
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(Mesh_ItemPot);
    glPopMatrix();
}

// 衝突判定
void ItemPot::react(Actor& other) {
    // プレイヤーの攻撃タグと衝突した時のみ体力減少
    if (other.tag() != Tag_PlayerAttack) return;
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
    // 体力がなくなったら削除準備を開始
    if (hp_ <= 0) prepare_erase();
}

// 個別の削除処理
void ItemPot::this_prepare_erase() {
    // SE再生
    int num = SE_Break_Pot1 + gsRand(0, Break_Pot_SE - 1);
    SEManager::get_instance().play_se(num);
    // アイテムドロップ
    drop_item();
    // このアイテム壺を生成したスポナーの生成状態を戻す
    world_->send_message("Broken Pot", this);
}

// アイテムドロップ処理
void ItemPot::drop_item() {
    const GSvector3 position = transform_.position();
    world_->add_actor(new Portion{ world_, position });
}
