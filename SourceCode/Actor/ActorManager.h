#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <gslib.h>
#include <list>
#include <string>
#include <vector>

// 前方宣言
class Actor;
class BoundingSphere;
class Character;
class FieldActor;
class Line;
class Ray;

// アクター管理クラス 授業教材使用
class ActorManager {
public:
	// コンストラクタ
    ActorManager() = default;
	// デストラクタ
    ~ActorManager();
	// アクターの追加
	void add(Actor* actor);
	// アクターの更新
	void update(float delta_time);
	// アクターの遅延更新
	void late_update(float delta_time);
	// アクターの描画
	void draw() const;
	// 半透明アクターの描画
	void draw_transparent() const;
	// アクターのGUI描画
	void draw_gui() const;
	// アクターの衝突判定
	void collide();
    // フィールドアクター・レイとの衝突判定
    FieldActor* collide(const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // フィールドアクター・線分との衝突判定
    FieldActor* collide(const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // フィールドアクター・球体との衝突判定
    FieldActor* collide(const BoundingSphere& sphere,
        GSvector3* collided_center = nullptr) const;
	// 削除予定のアクターの削除
	void remove();
	// アクターの検索
	Actor* find(const std::string& tag) const;
	// 指定したタグを持つアクターの検索
	std::vector<Actor*> find_with_tag(const GSuint& tag) const;
	// アクター数を返す
	int count() const;
	// 指定したタグを持つアクター数を返す
	int count_with_tag(const GSuint& tag) const;
	// メッセージの送信
	void send_message(const std::string& message, void* param);
	// 削除
	void clear();
	// コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	// アクターリスト
	std::list<Actor*> actors_;
};

#endif // !ACTOR_MANAGER_H_
