#ifndef FIRE_BALL_H_
#define FIRE_BALL_H_

#include "Actor/Actor.h"
#include "Actor/Parameter/ActionData.h"

// プレイヤーのＣ１攻撃・火球
class FireBall : public Actor {
public:
    // コンストラクタ
    FireBall(IWorld* world, Actor* owner, const BoundingSphere& collider, float lifespan = 0.0f, float attack = 0, const GSvector3& velocity = GSvector3::zero(), const EffectParam& effect = EffectParam{}, const std::bitset<8>& sp_effect = 0);
    // デストラクタ
    ~FireBall();

    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;

    // フィールドとの衝突判定
    void collide_field() override;

private:
    // 攻撃判定を出現させたアクターの名前
    std::string owner_name_;
    // 寿命タイマー
    float lifespan_{ 0.0f };
    // 爆発しているか？
    bool exploded_{ false };
    // エフェクトハンドル
    GSuint effect_handle_;
};

#endif // !FIRE_BALL_H_
