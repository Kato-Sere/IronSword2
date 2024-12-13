#include "Assets.h"
#include "Portion.h"
#include "Actor/ActorEnum.h"
#include "Sound/SEManager.h"
#include "World/IWorld.h"
#include "Actor/Character/Character.h"

#include <GSeffect.h>

// コンストラクタ
Portion::Portion(IWorld* world, const GSvector3& position) {
    world_ = world;
    tag_ = Tag_Item;
    name_ = "Portion";
    // 衝突判定球
    const GSvector3 center{ 0.0f, 0.5f, 0.0f };
    collider_ = BoundingSphere{ 0.2f, center };
    // Transformの初期化
    transform_.position(position);
}

// 描画
void Portion::draw() const {
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(Mesh_Portion);
    glPopMatrix();
}

// 衝突判定
void Portion::react(Actor& other) {
    // プレイヤーと衝突した場合のみ死亡する
    if (other.tag() != Tag_Player) return;
    // 相手の失った体力の半分を回復させる
    Character* chara = static_cast<Character*>(&other);
    int lost_hp = chara->parameters().hp().max() - chara->parameters().hp().current();
    // 回復量が５の倍数になるように調整
    int heal_hp = lost_hp / 2;
    if (heal_hp % 5 != 0) heal_hp += -(heal_hp % 5) + 5;
    chara->parameters().hp().add(heal_hp);
    // 回復エフェクト再生
    GSvector3 chara_pos = chara->transform().position();
    const GSvector3 pos = GSvector3{ chara_pos.x, chara->collider().center().y, chara_pos.z };
    gsPlayEffect(Effect_Heal, &pos);
    prepare_erase();
}

// 個別の削除処理
void Portion::this_prepare_erase() {
    // SE再生
    SEManager::get_instance().play_se(SE_GetItem);
}

