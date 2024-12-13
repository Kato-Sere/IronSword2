#ifndef ACTOR_H_
#define ACTOR_H_

#include "Collider/BoundingSphere.h"

#include <bitset>
#include <gslib.h>
#include <GStransform.h>
#include <string>
#include <vector>

// 前方宣言
class Actor;
class IWorld;

// アクタークラス 授業教材から一部改造
class Actor {
public:
	// コンストラクタ
	Actor() = default;
	// デストラクタ
	virtual ~Actor() = default;
	// 更新
	virtual void update(float delta_time);
	// 遅延更新
	virtual void late_update(float delta_time);
	// 描画
	virtual void draw() const;
	// 半透明の描画
	virtual void draw_transparent() const;
	// GUIの描画
	virtual void draw_gui() const;
	// 衝突処理
	virtual void react(Actor& other);
    // 間隔拡張処理
    virtual void react_spaceing(Actor& other);
	// メッセージ処理
	virtual void handle_massage(const std::string& message, void* param);
	// アクターとの衝突判定
	void collide(Actor& other);
	// 共通の削除準備
	void prepare_erase();
    // 個別の削除準備
    virtual void this_prepare_erase() {};
	// 衝突したか？
	bool is_collide(const Actor& other) const;
    // 間隔拡張用衝突判定で衝突したか？
    bool is_spaceing_collide(const Actor& other) const;
	// 削除するか？
	bool is_erase() const;
	// 名前を取得
	const std::string& name() const;
	// タグを取得
	const GSuint& tag() const;
    // 攻撃力を取得
    const int& attack() const;
	// Transformを取得
	GStransform& transform();
	// Transformを取得 (const)
	const GStransform& transform() const;
	// 移動量を取得
	GSvector3 velocity() const;
    // 特殊効果を取得
    const std::bitset<8>& sp_effect() const;
	// 衝突判定データを取得
	BoundingSphere collider() const;
    // 間隔拡張用衝突判定データを取得
    BoundingSphere spaceing_collider() const;
    // 親アクターを取得
    Actor* owner();

    // アクターとの衝突判定
    void collide_actor(Actor& other, float length);
    // フィールドとの衝突判定
    virtual void collide_field();

    // 視錐台内に存在するか？
    bool is_inside_frustum(const float rate = 1.0f) const;

	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

public:
    // トランスフォーム
    GStransform    transform_;
    // 移動量
    GSvector3      velocity_{ GSvector3::zero() };

protected:
	// ワールド
	IWorld*        world_{ nullptr };
	// タグ名
    GSuint         tag_{ 0 };
	// 名前
	std::string    name_;
    // 親アクター
    Actor*         owner_{ nullptr };
	// 衝突判定は有効か？
	bool           enable_collider_{ true };
	// 衝突判定
	BoundingSphere collider_;
    // 間隔拡張用衝突判定を使用するか？
    bool           use_spaceing_{ false };
    // 間隔拡張用衝突判定
    BoundingSphere spaceing_collider_;
    // 攻撃力
    int            attack_{ 0 };
    // 特殊効果
    std::bitset<8> sp_effect_{ 0 };
	// 削除フラグ
	bool           erase_{ false };
};

#endif // !ACTOR_H_
