#include "Collider/BoundingSphere.h"

// コンストラクタ
BoundingSphere::BoundingSphere(float radius, const GSvector3& center) :
	radius_{ radius }, center_{ center }{
}

// 平行移動
BoundingSphere BoundingSphere::translate(const GSvector3& position) const {
	return BoundingSphere{ radius_, center_ + position };
}

// 座標変換
BoundingSphere BoundingSphere::transform(const GSmatrix4& matrix) const {
	return BoundingSphere{ radius_ * matrix.getScale().y, matrix.transform(center_) };
}

// 交差しているか？
bool BoundingSphere::intersects(const BoundingSphere& other) const {
	return gsCollisionSphereAndSphere(&center_, radius_, &other.center_, other.radius_) == GS_TRUE;
}

// デバッグ表示
void BoundingSphere::draw(int wire) const {
	glPushMatrix();
	glTranslatef(center_.x, center_.y, center_.z);
	glutWireSphere(radius_, wire, wire); // ワイヤーフレームの球体を描画
	glPopMatrix();
}

// 半径を取得
const float BoundingSphere::radius() const {
    return radius_;
}

// 中心座標を取得
const GSvector3 BoundingSphere::center() const {
    return center_;
}
