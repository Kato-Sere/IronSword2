#include "Actor.h"
#include "ActorManager.h"
#include "FieldActor.h"
#include "World/Line.h"
#include "World/Ray.h"

#include <algorithm>

// アクター描画判定時の半径の倍率
const float Actors_Draw_Rate{ 1.2f };

// デストラクタ
ActorManager::~ActorManager() {
    clear();
}

// アクターの追加
void ActorManager::add(Actor* actor) {
    actors_.push_back(actor);
}

// アクターの更新
void ActorManager::update(float delta_time) {
    for (auto actor : actors_) {
        actor->update(delta_time);
    }
}

// アクターの遅延更新
void ActorManager::late_update(float delta_time) {
    for (auto actor : actors_) {
        actor->late_update(delta_time);
    }
}

// アクターの描画
void ActorManager::draw() const {
    for (auto actor : actors_) {
        if (actor->is_inside_frustum(Actors_Draw_Rate)) actor->draw();
    }
}

// 半透明アクターの描画
void ActorManager::draw_transparent() const {
    for (auto actor : actors_) {
        if (actor->is_inside_frustum()) actor->draw_transparent();
    }
}

// アクターのGUI描画
void ActorManager::draw_gui() const {
    for (auto actor : actors_) {
        if (actor->is_inside_frustum()) actor->draw_gui();
    }
}

// アクターの衝突判定
void ActorManager::collide() {
    for (auto i = actors_.begin(), end = actors_.end(); i != end; ++i) {
        for (auto j = std::next(i); j != actors_.end(); ++j) {
            (*i)->collide(**j);
        }
    }
}

// フィールドアクター・レイとの衝突判定
FieldActor* ActorManager::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane) const {
    // レイを線分に変換する
    Line line;
    line.start_ = ray.position_;
    line.end_ = ray.position_ + (ray.direction_.normalized() * max_distance);
    // 線分との判定を行う
    return collide(line, intersect, plane);
}

// フィールドアクター・線分との衝突判定
FieldActor* ActorManager::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
    // 最も始点から近いアクター
    FieldActor* closest_actor{ nullptr };
    // 最も始点から近い交点との距離
    float closest_distance = FLT_MAX;
    // 最も始点から近い交点
    GSvector3 closest_intersection;
    // 最も始点から近い交点の平面
    GSplane closest_intersection_plane;

    for (auto actor : actors_) {
        // フィールドアクター以外は処理しない
        if (actor->name() != "WoodBox") continue;
        FieldActor* field_actor = static_cast<FieldActor*>(actor);
        // フィールドアクターと線分の衝突判定を行う
        GSvector3 intersection_point;
        GSplane intersection_plane;
        if (field_actor->collide(line, &intersection_point, &intersection_plane)) {
            // 始点からの距離を計算
            float distance = line.start_.distance(intersection_point);
            if (distance < closest_distance) {
                // 距離が近い場合は、アクターを更新
                closest_distance = distance;
                closest_actor = field_actor;
                closest_intersection = intersection_point;
                closest_intersection_plane = intersection_plane;
            }
        }
    }
    // 衝突したアクターが存在するか？
    if (closest_actor != nullptr) {
        if (intersect != nullptr) {
            *intersect = closest_intersection;
        }
        if (plane != nullptr) {
            *plane = closest_intersection_plane;
        }
    }
    return closest_actor; // 始点に最も近いアクターを返す

}

// フィールドアクター・球体との衝突判定
FieldActor* ActorManager::collide(const BoundingSphere& sphere, GSvector3* collided_center) const {
    for (auto actor : actors_) {
        // フィールドアクター以外は処理しない
        if (actor->name() != "WoodBox") continue;
        FieldActor* field_actor = static_cast<FieldActor*>(actor);
        // フィールドアクターと球体の衝突判定を行う
        GSvector3 intersect;
        if (field_actor->collide(sphere, &intersect)) {
            return field_actor;
        }
    }
    return nullptr;
}

// 削除予定のアクターの削除
void ActorManager::remove() {
    for (auto i = actors_.begin(), end = actors_.end(); i != end;) {
        if ((*i)->is_erase()) {
            delete* i;
            i = actors_.erase(i);
        } else ++i;
    }
}

// アクターの検索
Actor* ActorManager::find(const std::string& name) const {
    for (auto actor : actors_) {
        if (actor->name() == name) return actor;
    }
    return nullptr;
}

// 指定したタグを持つアクターの検索
std::vector<Actor*> ActorManager::find_with_tag(const GSuint& tag) const {
    std::vector<Actor*> result;
    result.reserve(actors_.size());
    for (auto actor : actors_) {
        if (actor->tag() == tag) result.push_back(actor);
    }
    result.shrink_to_fit();
    return result;
}

// アクター数を返す
int ActorManager::count() const {
    return (int)actors_.size();
}


// 指定したタグを持つアクターを数を返す
int ActorManager::count_with_tag(const GSuint& tag) const {
    return std::count_if(actors_.begin(), actors_.end(), [tag](Actor* actor) { return actor->tag() == tag; });
}

// メッセージの送信
void ActorManager::send_message(const std::string& message, void* param) {
    for (auto actor : actors_) {
        actor->handle_massage(message, param);
    }
}

// 削除
void ActorManager::clear() {
    actors_.remove_if([](auto actor) {
        actor->this_prepare_erase();
        delete actor;
        return true;
        });
    actors_.clear();
}
