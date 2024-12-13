#include "Assets.h"
#include "World/World.h"
#include "World/Field.h"
#include "Actor/Actor.h"
#include "UI/PlayerUI.h"
#include <GSeffect.h>

#include <imgui/imgui.h>
#include <iterator>

// デストラクタ
World::~World() {
    clear();
}

// 更新
void World::update(float delta_time) {
    // ゲーム再生速度をかける
    float time = delta_time * game_speed_;

    /*更新はフィールド→アクター→カメラ→ライトの順に行う*/
    // フィールドの更新
    field_->update(time);
    // アクターの更新
    actors_.update(time);
    // アクターの衝突
    actors_.collide();
    // アクターの遅延更新
    actors_.late_update(time);
    // アクターの消去
    actors_.remove();
    // UIの更新（ゲームスピードは考慮しない）
    if (!game_over_) ui_->update(delta_time);
    // カメラの更新（ゲームスピードは考慮しない）
    camera_->update(delta_time);
    // 視錐台の更新
    gsUpdateFrustum();
    // ライトの更新
    light_->update(time);
    // エフェクトの更新処理
    gsUpdateEffect(time);
}

// 描画
void World::draw() const {
    /*描画はカメラ→ライト→フィールド→アクター→UIの順に行う*/

    // カメラの描画
    camera_->draw();
    // エフェクト用カメラの設定
    gsSetEffectCamera();
    // ライトの描画
    light_->draw();

    // シャドウマップの描画
    gsDrawShadowMap(World::shadow_map_callback, (void*)this);

    // フィールドの描画
    field_->draw();
    // 半透明アクターの描画
    actors_.draw_transparent();
    // アクターの描画
    actors_.draw();
    // エフェクトの描画
    gsDrawEffect();

    // ゲームオーバー時は以降の処理を行わない
    if (game_over_) return;

    // ＧＵＩの描画
    actors_.draw_gui();
    // UIの描画
    ui_->draw_gui();
}

// 消去
void World::clear() {
    // アクターの消去
    actors_.clear();
    // ウェイポイントの消去
    for (std::vector<Actor*> v_wp : way_points_) {
        std::fill(v_wp.begin(), v_wp.end(), nullptr);
        v_wp.clear();
    }
    way_points_.clear();
    // カメラの消去
    delete camera_;
    camera_ = nullptr;
    // ライトの消去
    delete light_;
    light_ = nullptr;
    // フィールドの消去
    delete field_;
    field_ = nullptr;
    // UIの消去
    delete ui_;
    ui_ = nullptr;
}

// カメラの追加
void World::add_camera(Actor* camera) {
    delete camera_; // 現在のカメラを削除
    camera_ = camera;
}

// ライトの追加
void World::add_light(Actor* light) {
    delete light_; // 現在のライトを削除
    light_ = light;
}

// フィールドの追加
void World::add_field(Field* field) {
    delete field_; // 現在のフィールドを削除
    field_ = field;
}

// UIの追加
void World::add_ui(Actor* ui) {
    delete ui_; // 現在のUIを削除
    ui_ = ui;
}

// アクターの追加
void World::add_actor(Actor* actor) {
    std::lock_guard<std::mutex> lock(mtx_);
    actors_.add(actor);
}

// ウェイポイント配列の要素数設定
void World::set_size_waypoint_vector() {
    // ウェイポイント用配列の要素数を設定
    way_points_.resize(WayPoint_Count + 1, std::vector<Actor*>(WayPoint_Count));
    // ウェイポイント区画中心座標用の要素数を設定
    wp_blocks_pos_.resize(WayPoint_Division_Count, std::vector<GSvector3>(WayPoint_Division_Count));
}

// ウェイポイントを配列に追加
void World::add_waypoint(int x, int y, Actor* point) {
    way_points_[x][y] = point;
    actors_.add(point);
}

// アクターマネージャーの取得
ActorManager& World::actor_manager() {
    return actors_;
}

// アクターの検索
Actor* World::find_actor(const std::string& name) const {
    return actors_.find(name);
}

// 指定したタグを持つアクターの検索
std::vector<Actor*> World::find_actor_with_tag(const GSuint& tag) const {
    return actors_.find_with_tag(tag);
}

// アクター数を返す
int World::count_actor() const {
    return actors_.count();
}

// 指定したタグを持つアクター数を返す
int World::count_actor_with_tag(const GSuint& tag) const {
    return actors_.count_with_tag(tag);
}

// メッセージの送信
void World::send_message(const std::string& message, void* param) {
    actors_.send_message(message, param);
}

// ゲーム再生速度の取得
const float World::game_speed() const {
    return game_speed_;
}

// ゲーム再生速度の変更
void World::change_speed(float value) {
    game_speed_ = value;
}

// ゲームオーバーになっているか？
bool World::is_game_over() const {
    return game_over_;
}

// ゲームオーバー判定の変更
void World::set_game_over(bool frag) {
    game_over_ = frag;
}

// カメラの取得
Actor* World::camera() {
    return camera_;
}

// ライトの取得
Actor* World::light() {
    return light_;
}

// フィールドの取得
Field* World::field() {
    return field_;
}

// UIの取得
PlayerUI* World::player_ui() {
    return static_cast<PlayerUI*>(ui_);
}

// ウェイポイントの全取得
const std::vector<std::vector<Actor*>>& World::way_points_all() {
    return way_points_;
}

// 自分に近いウェイポイントを取得
std::vector<Actor*> World::way_points(const GSvector3& position) {
    // 自分に一番近いウェイポイント区画を取得
    GSvector2 nearest{}; float nearest_dist = FLT_MAX;
    for (GSfloat i = 0, x_size = wp_blocks_pos_.size(); i < x_size; ++i) {
        for (GSfloat j = 0, y_size = wp_blocks_pos_[i].size(); j < y_size; ++j) {
            // 距離の計算
            float dist = GSvector3::distance(position, wp_blocks_pos_[i][j]);
            if (dist < nearest_dist) {
                nearest = GSvector2{ i, j };
                nearest_dist = dist;
            }
        }
    }

    // 周囲のウェイポイント区画を取得
    std::vector<GSvector2> wp_blocks_list{};
    wp_blocks_list.reserve(9); // 最大取得数にあらかじめ変更
    for (GSfloat x = nearest.x - 1; x <= nearest.x + 1; ++x) {
        bool is_out_range_x = x < 0 || x >= wp_blocks_pos_.size();
        if (is_out_range_x) continue;
        for (GSfloat y = nearest.y - 1; y <= nearest.y + 1; ++y) {
            bool is_out_range_y = y < 0 || y >= wp_blocks_pos_[x].size();
            if (is_out_range_y) continue;
            wp_blocks_list.emplace_back(x, y);
        }
    }

    // ウェイポイントを取得
    std::vector<Actor*> result{};
    int div = WayPoint_Count / WayPoint_Division_Count;
    result.reserve(div * div * 9); // 最大取得数にあらかじめ変更
    for (const GSvector2& value : wp_blocks_list) {
        std::vector<Actor*> add_wps = way_points_block(value.y, value.x);
        copy(add_wps.begin(), add_wps.end(), back_inserter(result));
    }
    return result;
}

// 区画ごとにウェイポイントを取得
std::vector<Actor*> World::way_points_block(int x, int y) {
    std::vector<Actor*> result{};
    int wp_block_count = WayPoint_Count / WayPoint_Division_Count;
    result.reserve(pow(wp_block_count, 2));
    for (int i = wp_block_count * x, i_end = wp_block_count * (x + 1); i < i_end; ++i) {
        for (int j = wp_block_count * y, j_end = wp_block_count * (y + 1); j < j_end; ++j) {
            result.emplace_back(way_points_[i][j]);
        }
    }
    return result;
}

// 撤退用ウェイポイントを取得
std::vector<Actor*> World::way_points_retreat() {
    std::vector<Actor*> waypoints = way_points_[WayPoint_Count];
    std::vector<Actor*> result;
    result.reserve(waypoints.size());
    for (int i = 0, size = waypoints.size(); i < size; ++i) {
        if (waypoints[i] != nullptr) result.push_back(waypoints[i]);
    }
    return result;
}

// ウェイポイント区画ごとの中心位置の計算
void World::calc_wp_blocks_pos() {
    // ステージの広さを計算
    GSvector2 stage_size = Stage_Max - Stage_Min;
    for (int x = 0, size_x = wp_blocks_pos_.size(); x < size_x; ++x) {
        for (int y = 0, size_y = wp_blocks_pos_[x].size(); y < size_y; ++y) {
            float add_value = WayPoint_Count / WayPoint_Division_Count;
            // x座標を計算
            float rate_x = add_value * y + 1;
            float pos_x = Stage_Min.x + stage_size.x / (WayPoint_Count - 1) * rate_x;
            // z座標を計算
            float rate_z = add_value * x + 1;
            float pos_z = Stage_Min.y + stage_size.y / (WayPoint_Count - 1) * rate_z;
            wp_blocks_pos_[x][y] = GSvector3{ pos_x, 1.0f, pos_z };
        }
    }
}

// シャドウマップの描画
void World::shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*) {
    World* self = (World*)param;
    self->actors_.draw(); // シャドウマップにはアクターのみ描画
}
