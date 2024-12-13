#ifndef MINI_MAP_H_
#define MINI_MAP_H_

#include "World/IWorld.h"
#include <GStype.h>

// マップ表示クラス
class Minimap {
public:
    // コンストラクタ
    Minimap(IWorld* world);
    // デストラクタ
    ~Minimap();

    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;

private:
    // アイコンの描画
    void draw_icon(const GSvector3& position, const GScolor& color) const;

private:
    // ワールド
    IWorld* world_{ nullptr };
    // プレイヤーの座標
    GSvector3 player_pos_{ GSvector3::zero() };
};

#endif // !MINI_MAP_H_
