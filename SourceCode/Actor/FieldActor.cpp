#include "FieldActor.h"
#include "World/Line.h"
#include "World/Ray.h"
#include "Collider/BoundingSphere.h"

// 描画
void FieldActor::draw() const {
    // メッシュの描画
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(mesh_);
    glPopMatrix();
}

// レイとの衝突判定
bool FieldActor::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane) const {
    // レイのパラメータを線分に変換
    Line line;
    line.start_ = ray.position_;
    line.end_ = ray.position_ + (ray.direction_.normalized() * max_distance);
    // 線分との衝突判定を行う
    return collide(line, intersect, plane);
}

// 線分との衝突判定
bool FieldActor::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
    // ローカル座標系の線分を求める
    Line local_line;
    local_line.start_ = transform_.inverseTransformPoint(line.start_);
    local_line.end_ = transform_.inverseTransformPoint(line.end_);
    // メッシュと線分が交差したか？
    if (gsCollisionMeshAndLine(gsGetMesh(mesh_collider_), &local_line.start_, &local_line.end_, intersect, plane) == GS_TRUE) {
        if (intersect != nullptr) {
            // 交点をワールド座標系に変換
            *intersect = transform_.transformPoint(*intersect);
        }
        if (plane != nullptr) {
            // 平面の方程式をワールド座標系に変換
            const GSmatrix4& local_to_world_matrix = transform_.localToWorldMatrix();
            gsPlaneTransform(plane, plane, &local_to_world_matrix);
        }
        return true;
    }
    return false;
}

// 球体との衝突判定
bool FieldActor::collide(const BoundingSphere& sphere, GSvector3* collided_center) const {
    // ローカル座標系の球体を求める
    BoundingSphere local_sphere;
    local_sphere.center_ = transform_.inverseTransformPoint(sphere.center_);
    local_sphere.radius_ = sphere.radius_;
    // メッシュと球体が衝突したか？
    GSvector3 local_collided_center;
    if (gsCollisionMeshAndSphere(gsGetMesh(mesh_collider_), &local_sphere.center_, local_sphere.radius_, &local_collided_center) == GS_TRUE) {
        if (collided_center != nullptr) {
            // 衝突判定後の中心座標をワールド座標系に変換
            *collided_center = transform_.transformPoint(local_collided_center);
        }
        return true;
    }
    return false;
}
