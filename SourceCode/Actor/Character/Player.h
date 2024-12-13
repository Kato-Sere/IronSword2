#ifndef PLAYER_H_
#define PLAYER_H_

#include "Character.h"
#include "CSVReader.h"
#include "InputManager.h"

// プレイヤークラス
class Player : public Character {
public:
    // コンストラクタ
    Player(IWorld* world = nullptr, const GSvector3& positon = GSvector3::zero());
    // 更新
    void update(float delta_time) override;
    // 描画
    void draw() const override;
    // 衝突処理
    void react(Actor& other) override;

    // 体力が0になった瞬間の処理
    void down() override {};

    // 各状態への遷移条件
    void update_state_trigger(float delta_time) override;
    // 移動状態時の移動処理
    void translate_move(float delta_time, float speed) override;
    // 攻撃状態時の移動処理
    void translate_attack(float delta_time, const ActionData& actionData) override;
    // 防御状態時の移動処理
    void translate_block(float delta_time, float speed) override;
    // ジャンプ時の移動処理
    void translate_jump(float delta_time, float speed) override;

    // 移動方向の計算
    GSvector2 move_direction(float direction) override;

    // ダメージボイス再生
    void damage_voice() override;
    // 死亡時ボイス再生
    void death_voice() override;
    // 特殊技ゲージ増加処理
    void add_sp_gauge(int attack, float rate = 1.0f) override;

private:
    // 防御モーション中の移動処理
    void move_blocking(float delta_time);
    // 状態の追加
    void add_states(IWorld* world) override;
    // 個別のアニメーションイベントを追加
    void this_add_events();
    // 武器の描画
    void draw_weapon() const;
    // 防具の描画
    void draw_shield() const;
    // パッドの振動を設定
    void set_vibration(int timer, float power);
    // パッドの振動状態の更新
    void update_vibration(float delta_time);
    // 瀕死時の更新処理
    void update_dying(float delta_time);

private:
    // プレイヤーの入力
    InputManager input_;
    // パッドの振動タイマー
    int vibration_timer_;
    // パッドの振動の強度
    float vibration_power_;
    // 瀕死時の特殊技ゲージ増加タイマー
    float danger_add_sp_timer_;
};

#endif // !PLAYER_H_
