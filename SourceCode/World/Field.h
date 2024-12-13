#ifndef FIELD_H_
#define FIELD_H_

#include "Actor/ActorManager.h"
#include <gslib.h>

// 前方宣言
class Ray;
class Line;
class BoundingSphere;

// フィールドクラス 授業教材使用
class Field {
public:
	// コンストラクタ
	Field(ActorManager& actors, GSuint octree, GSuint collider, GSuint skybox);
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;

	// 線分との衝突判定
	bool is_collide(const Line& line, GSvector3* intersect = nullptr, GSplane* plane = nullptr, Actor** actor = nullptr) const;
	// レイとの衝突判定
	bool is_collide(const Ray& ray, float max_distance, GSvector3* intersect = nullptr, GSplane* plane = nullptr, Actor** actor = nullptr) const;
	// 球との衝突判定
	bool is_collide(const BoundingSphere& sphere, GSvector3* center = nullptr, Actor** actor = nullptr) const;

	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
    // アクターマネージャー
    ActorManager& actors_;
	// 描画用オクトリー
	GSuint octree_;
	// 衝突判定用オクトリー
	GSuint collider_;
	// スカイボックス
	GSuint skybox_;
};

#endif // !FIELD_H_
