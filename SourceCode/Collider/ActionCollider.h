#ifndef ACTION_COLLIDER_H_
#define ACTION_COLLIDER_H_

#include "Actor/Actor.h"
#include "Actor/Character/Character.h"
#include <bitset>

// 攻撃判定
class ActionCollider : public Actor {
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="world">ワールド</param>
    /// <param name="owner">親アクター</param>
    /// <param name="collider">衝突判定</param>
    /// <param name="owner_name">親の名前</param>
    /// <param name="tag">自分のタグ</param>
    /// <param name="lifespan">寿命</param>
    /// <param name="attack">攻撃力</param>
    /// <param name="delay">遅延時間</param>
    /// <param name="velocity">速度</param>
    /// <param name="sp_effect">特殊効果</param>
    ActionCollider(IWorld* world, Actor* owner, const BoundingSphere& collider,
        const std::string &owner_name = "", const GSuint& tag = 0, float lifespan = 1.0f,
        int attack = 0, float delay = 0.0f, const GSvector3& velocity = GSvector3::zero(),
        const std::bitset<8>& sp_effect = 0);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;

private:
    // 衝突相手に合わせたサウンドを再生する
    void play_hit_sound(Actor* other);
    // デバッグ用更新
    void debug_update(float delta_time);

private:
	// 攻撃判定を出現させたアクターの名前
	std::string owner_name_;
	// 寿命
	float lifespan_timer_;
	// 衝突判定が有効になるまでの遅延時間
	float delay_timer_;
};

#endif // !ACTION_COLLIDER_H_
