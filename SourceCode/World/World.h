#ifndef WORLD_H_
#define WORLD_H_

#include "World/IWorld.h"
#include "Actor/ActorManager.h"
#include <gslib.h>
#include <mutex>

// ワールドクラス 授業教材使用
class World : public IWorld {
public:
	// コンストラクタ
	World() = default;
	// デストラクタ
	~World();
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// 消去
	void clear();
	// カメラの追加
	virtual void add_camera(Actor* camera) override;
	// ライトの追加
    virtual void add_light(Actor* light) override;
	// フィールドの追加
	virtual void add_field(Field* field) override;
    // UIの追加
    virtual void add_ui(Actor* ui) override;
	// アクターを追加
	virtual void add_actor(Actor* actor) override;
    // ウェイポイント配列の要素数設定
    virtual void set_size_waypoint_vector() override;
    // ウェイポイントを配列に追加
    virtual void add_waypoint(int x, int y, Actor* point) override;

    // アクターマネージャーの取得
    virtual ActorManager& actor_manager() override;
	// アクターの検索
	virtual Actor* find_actor(const std::string& name) const override;
	// 指定したタグを持つアクターの検索
	virtual std::vector<Actor*> find_actor_with_tag(const GSuint& tag) const override;
	// アクター数を返す
	virtual int count_actor() const override;
	// 指定したタグを持つアクター数を返す
	virtual int count_actor_with_tag(const GSuint& tag) const override;
	// メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) override;

    // ゲーム再生速度の取得
    virtual const float game_speed() const override;
    // ゲーム再生速度の変更
    virtual void change_speed(float value) override;

    // ゲームオーバーになっているか？
    bool is_game_over() const;
    // ゲームオーバー判定の変更
    void set_game_over(bool frag);

	// カメラの取得
	virtual Actor* camera() override;
	// ライトの取得
	virtual Actor* light() override;
	// フィールドの取得
	virtual Field* field() override;
    // UIの取得
    virtual PlayerUI* player_ui() override;

    // ウェイポイントの全取得
    const std::vector<std::vector<Actor*>>& way_points_all();
    // 自分に近いウェイポイントを取得
    std::vector<Actor*> way_points(const GSvector3& position);
    // 区画ごとにウェイポイントを取得
    std::vector<Actor*> way_points_block(int x, int y);
    // 撤退用ウェイポイントを取得
    std::vector<Actor*> way_points_retreat();
    // ウェイポイント区画ごとの中心位置の計算
    void calc_wp_blocks_pos();

	// シャドウマップの描画
	static void shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*);

	// コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

private:
	// アクターマネージャー
	ActorManager actors_;
	// ライト
	Actor* light_{ nullptr };
	// カメラ
	Actor* camera_{ nullptr };
	// フィールド
	Field* field_{ nullptr };
    // UI
    Actor* ui_{ nullptr };
    // ウェイポイントの配列
    std::vector<std::vector<Actor*>> way_points_;
    // ウェイポイント区画ごとの中心位置
    std::vector<std::vector<GSvector3>> wp_blocks_pos_;
    // ゲーム再生速度
    float game_speed_{ 1.0f };
    // ゲームオーバーになっていないか？
    bool game_over_{ false };
    // ロック用
    std::mutex mtx_;
};

#endif // !WORLD_H_
