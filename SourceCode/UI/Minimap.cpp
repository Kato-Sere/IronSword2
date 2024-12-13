#include "Assets.h"
#include "Minimap.h"
#include "Actor/Actor.h"
#include "Actor/ActorEnum.h"
#include "Actor/Character/Character.h"

// マップの表示座標
const GSvector2 Draw_Pos{ 974.0f,50.0f };
// マップの大きさ
const GSvector2 Map_Size{ 256, 256 };
// 敵アイコンの描画範囲
const float Draw_Enemy_Range{ 15.0f };
// プレイヤーアイコンの描画色
const GScolor Player_Color{ 0.0f,1.0f, 0.0f,1.0f };
// 敵アイコンの描画色
const GScolor Enemy_Color{ 1.0f,0.0f,0.0f,1.0f };

// コンストラクタ
Minimap::Minimap(IWorld* world) {
    world_ = world;
}

// デストラクタ
Minimap::~Minimap() {
    world_ = nullptr;
}

// 更新
void Minimap::update(float delta_time) {
    // プレイヤーの座標を取得
    player_pos_ = world_->find_actor("Player")->transform().position();
}

// 描画
void Minimap::draw() const {
    // 背景の描画
    gsDrawSprite2D(Texture_Map_Base, &Draw_Pos, NULL, NULL, NULL, NULL, 0.0f);
    // プレイヤーの検索
    Actor* player = world_->find_actor("Player");
    // プレイヤーの逆行列を取得
    GSmatrix4 matrix = GS_MATRIX4_IDENTITY;
    // マップの上方向をプレイヤーの進行方向に合わせる
    if (player != nullptr) matrix = player->transform().worldToLocalMatrix();
    matrix *= GSmatrix4::rotateY(180.0f);
    // 敵アイコンの描画
    for (Actor* enemy : world_->find_actor_with_tag(Tag_Enemy)) {
        // 体力がない場合は描画しない
        if (static_cast<Character*>(enemy)->is_dead()) continue;
        draw_icon(enemy->transform().position(), Enemy_Color);
    }
    // プレイヤーアイコンの描画
    draw_icon(player_pos_, Player_Color);
}

// アイコンの描画
void Minimap::draw_icon(const GSvector3& position, const GScolor& color) const {
    // プレイヤーとの距離が描画範囲外なら処理しない
    float distance = GSvector3::distance(player_pos_, position);
    if (distance >= Draw_Enemy_Range) return;
    // プレイヤーから見た対象の方向を計算
    GSvector3 direction = GSvector3::normalize(player_pos_ - position);
    // 描画座標の計算
    GSvector2 draw_pos = GSvector2{ distance * direction.x * (Map_Size.x / 2) / Draw_Enemy_Range,
                                    distance * direction.z * (Map_Size.y / 2) / Draw_Enemy_Range};
    // マップの中心位置にずらす
    draw_pos += GSvector2{ Map_Size.x / 2, Map_Size.y / 2 } + Draw_Pos;
    gsDrawSprite2D(Texture_Map_Icon, &draw_pos, NULL, NULL, &color, NULL, 0.0f);
}
