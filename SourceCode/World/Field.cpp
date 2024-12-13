#include "Field.h"
#include "Ray.h"
#include "Line.h"
#include "Actor/FieldActor.h"
#include "Collider/BoundingSphere.h"

// コンストラクタ
Field::Field(ActorManager& actors, GSuint octree, GSuint collider, GSuint skybox) :
    actors_{ actors }, octree_{ octree }, collider_{ collider }, skybox_{ skybox } {}

// 更新
void Field::update(float delta_time) {}

// 描画
void Field::draw() const {
    // スカイボックス・オクツリーの描画
    gsDrawSkybox(skybox_);
    gsDrawOctree(octree_);
}

// 線分との衝突判定
bool Field::is_collide(const Line& line, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    // オクトリーとの交差判定
    GSvector3 octree_intersection_point;
    GSplane octree_intersection_plane;
    bool intersection_octree = gsOctreeCollisionLine(gsGetOctree(collider_), &line.start_, &line.end_, &octree_intersection_point, &octree_intersection_plane) == GS_TRUE;
    // アクターとの交差判定
    GSvector3 actor_intersection_point;
    GSplane actor_intersection_plane;
    Actor* intersection_actor = actors_.collide(line, &actor_intersection_point, &actor_intersection_plane);
    // オクトリーとアクター両方に衝突しているか？
    if (intersection_octree && intersection_actor != nullptr) {
        // 始点と交点の距離を計算する
        float distance_to_actor = line.start_.distance(actor_intersection_point);
        float distance_to_octree = line.start_.distance(octree_intersection_point);
        if (distance_to_octree < distance_to_actor) {
            // オクトリーの方が近い場合はアクターに衝突していないことにする
            intersection_actor = nullptr;
        } else {
            // アクターの方が近い場合オクトリーに衝突していないことにする
            intersection_octree = false;
        }
    }
    // オクトリーに衝突しているか？
    if (intersection_octree) {
        if (intersect != nullptr) {
            *intersect = octree_intersection_point;
        }
        if (plane != nullptr) {
            *plane = octree_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = nullptr;
        }
        return true;
    }
    // アクターに衝突しているか？
    if (intersection_actor) {
        if (intersect != nullptr) {
            *intersect = actor_intersection_point;
        }
        if (plane != nullptr) {
            *plane = actor_intersection_plane;
        }
        if (actor != nullptr) {
            *actor = intersection_actor;
        }
        return true;
    }
    // 何も衝突しなかった
    return false;
}

// レイとの衝突判定
bool Field::is_collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane, Actor** actor) const {
    Line line{ ray.position_, ray.position_ + (ray.direction_.normalized() * max_distance) };
    return is_collide(line, intersect, plane, actor);
}

// 球との衝突判定
bool Field::is_collide(const BoundingSphere& sphere, GSvector3* center, Actor** actor) const {
    return gsOctreeCollisionSphere(gsGetOctree(collider_), &sphere.center_, sphere.radius_, center) == GS_TRUE;
}
