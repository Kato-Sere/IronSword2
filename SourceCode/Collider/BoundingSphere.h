#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include<gslib.h>

// 境界球クラス 授業教材使用
class BoundingSphere {
public:
	// コンストラクタ
	explicit BoundingSphere(float radius = 0.0f, const GSvector3& center = GSvector3::zero());
	// 平行移動
	BoundingSphere translate(const GSvector3& position) const;
	// 座標変換
	BoundingSphere transform(const GSmatrix4& matrix) const;
	// 重なっているか？
	bool intersects(const BoundingSphere& other) const;
	// デバッグ表示
	void draw(int wire = 16) const;

    // 半径を取得
    const float radius() const;
    // 中心座標を取得
    const GSvector3 center() const;

public:
	// 半径
	float radius_{ 0.0f };
	// 中心座標
	GSvector3 center_{ 0.0f,0.0f,0.0f };
};

#endif // !BOUNDING_SPHERE_H_
