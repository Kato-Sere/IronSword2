#include "Assets.h"
#include "Player.h"
#include "Actor/ActorEnum.h"
#include "Collider/ActionCollider.h"
#include "Sound/SEManager.h"
#include "UI/PlayerUI.h"
#include "World/IWorld.h"
#include "Actor/AtkMethod/FireBall.h"
#include "Actor/Character/CharacterConstantValue.h"
#include "Actor/Obstacle/InvisibleWall.h"

#include "Actor/State/CharaState_Attack.h"
#include "Actor/State/CharaState_Block.h"
#include "Actor/State/CharaState_Damage.h"
#include "Actor/State/CharaState_Down.h"
#include "Actor/State/CharaState_Jump.h"
#include "Actor/State/CharaState_JumpAttack.h"
#include "Actor/State/CharaState_Special.h"
#include "Actor/State/CharaState_Move.h"

#define _USE_MATH_DEFINES
#include <math.h>

// 死亡時の消滅タイマー
const int Dead_Timer{ 3600 };
// 防御時のスティックのデッドゾーン
const float DeadZone_StateBlock{ 0.2f };
// 防御時のノックバック減衰率
const float Knockback_DecayRate_Block{ 0.5f };

// 振動強度・弱
const float Vibration_Power_Weak{ 0.3f };
// 振動強度・強
const float Vibration_Power_Strong{ 0.6f };
// 振動強度・特殊技
const float Vibration_Power_Special{ 1.0f };

// ダメージボイスの種類
const int Player_Damage_Voice{ 3 };

// 瀕死判定の体力の割合(%)
const int Dying_HP_Rate{ 25 };
// 瀕死時の特殊技ゲージ増加タイマー
const int Default_Danger_Add_SP_Timer{ 10 };

// 連続攻撃回数
const int Player_Combo{ 6 };

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& positon) {
    world_ = world;
    mesh_ = AnimatedMesh{ Mesh_Player, Motion_Idle };
    motion_ = Motion_Idle;
    motion_loop_ = true;
    motion_speed_ = 1.0f;
    state_timer_ = 0.0f;
    tag_ = Tag_Player;
    name_ = "Player";
    attack_tag_ = Tag_PlayerAttack;
    damage_tag_ = Tag_EnemyAttack;
    // パラメータ設定
    load_param(ParamR_Player, name_);
    // 状態の追加
    add_states(world_);
    change_state(State_Move, Motion_Idle);
    // 衝突判定球
    const GSvector3 center{ 0.0f, parameters_.collider().height(), 0.0f };
    collider_ = BoundingSphere{ parameters_.collider().radius(), center };
    // Transformの初期化
    transform_.position(positon);
    transform_.rotate(GSvector3::zero());
    mesh_.transform(transform_.localToWorldMatrix());
    // アニメーションイベントを追加
    add_events(Player_Combo);
    this_add_events();
}

// 更新
void Player::update(float delta_time) {
    // 状態の更新
    current_state_->update(delta_time);
    state_timer_ += delta_time * motion_speed_;
    // 状態遷移フラグの更新
    update_state_trigger(delta_time);
    // パッドの振動状態の更新
    update_vibration(delta_time);
    // 瀕死時の更新
    bool is_danger = parameters_.hp().rate() <= Dying_HP_Rate && !is_dead();
    if (is_danger) update_dying(delta_time);
    // 重力値を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y * delta_time, 0.0f);
    // 落下速度を制限
    velocity_.y = CLAMP(velocity_.y, -0.1f, 0.15f);
    // フィールドとの衝突判定
    collide_field();
    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_, motion_speed_);
    // メッシュのモーションを更新
    mesh_.update(delta_time);
    // ワールド変換行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}

// 描画
void Player::draw() const {
    // メッシュの描画
    mesh_.draw();
    // 武器・防具の描画
    draw_weapon();
    draw_shield();
}

// 衝突処理
void Player::react(Actor& other) {
    // 自分の攻撃用タグ持ちと衝突した場合は何もしない
    if (other.tag() == attack_tag_) return;
    float length = collider_.radius_ + other.collider().radius_;
    // 自分と同じタグか障害物と衝突した場合は重ならないように補正をする
    if (other.tag() == tag() || other.tag() == Tag_Obstacle) {
        collide_actor(other,length);
        return;
    }
    // 透明な壁に衝突した場合は相手の妨害タグを確認
    if (other.tag() == Tag_InvisibleWall) {
        GSuint jamming = dynamic_cast<InvisibleWall*>(&other)->jamming_tag();
        if (tag() == jamming) collide_actor(other,length);
        return;
    }
    // 比較用に現在の体力を取得
    int prev_hp = parameters_.hp().current();
    current_state_->react(other);
    // 防御中でないかつ体力が変化している場合はコントローラー振動
    bool changed_hp = prev_hp != parameters_.hp().current();
    bool strong_vibration = other.sp_effect().test(SPEffect_KnockBack);
    float vibration_power = strong_vibration ?
        Vibration_Power_Strong : Vibration_Power_Weak;
    if (changed_hp && current_state() != State_Block) set_vibration(30, vibration_power);
}

// 各状態への遷移条件
void Player::update_state_trigger(float delta_time) {
    // 状態更新を行わない場合はすべての遷移条件を無視する
    if (!is_update_state()) {
        reset_state_trigger();
        return;
    }
    state_trigger_.Move = input_.stick_left() != GSvector2::zero();
    // プレイヤーから入力があった場合のみ各状態に遷移する
    state_trigger_.AttackN = input_.button_x();
    state_trigger_.AttackC = input_.button_y();
    state_trigger_.Block = input_.button_lb();
    state_trigger_.Jump = input_.button_a();
    state_trigger_.React = input_.button_any() || state_trigger_.Move;
    // 特殊技ゲージが満タンかつボタン入力があった場合は特殊状態に遷移する
    state_trigger_.Special = parameters_.sp().is_full() && input_.button_b();
    state_trigger_.BattleWalk = false;
}

// 移動状態時の移動処理
void Player::translate_move(float delta_time, float speed) {
    // 移動条件を満たしていない場合は処理終了
    if (!state_trigger_.Move) {
        change_state(State_Move, Motion_Idle);
        return;
    }

    // 移動量を計算
    GSvector3 move_velocity = calc_velocity(parameters_.move_speed().speed(), input_.stick_left(), false);
    // 向きの補間
    GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(move_velocity), Rotate_Speed_Move * delta_time);
    transform_.rotation(rotation);
    // モーション変更
    change_state(State_Move, Motion_Run);
    // xz成分を更新して平行移動する
    velocity_ = GSvector3{ move_velocity.x, velocity_.y, move_velocity.z };
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

// 攻撃状態時の移動処理
void Player::translate_attack(float delta_time, const ActionData& actionData) {
    // スティック入力がある場合はそちらを反映する
    bool input_left_stick = input_.stick_left().length() > 0;
    // 移動方向の計算
    GSvector2 move_dir;
    if (input_left_stick) {
        move_dir = input_.stick_left();
    } else {
        GSvector2 from_dir = GSvector2::createFromDirection(actionData.move_direction);
        move_dir = GSvector2{ from_dir.y, from_dir.x };
    }

    // モーションの再生割合が指定の範囲内の時は移動する
    GSvector3 move_velocity = calc_velocity(actionData.move_speed, move_dir, !input_left_stick);
    bool move_before = ratio_motion_time() <= actionData.move_start; // 範囲の前か？
    bool move_after = ratio_motion_time() >= actionData.move_end;   // 範囲の後か？
    if (move_before || move_after) move_velocity = GSvector3::zero();

    // 向きの補間 プレイヤーの入力がある時のみ補間する
    if (input_left_stick) {
        GSvector3 direction = move_velocity.normalized();
        GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(direction), Rotate_Speed_Attack * delta_time);
        if (move_velocity.length() > 0) transform_.rotation(rotation);
    }

    // xz成分の更新
    velocity_ = GSvector3{ move_velocity.x, velocity_.y, move_velocity.z };
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

// 防御状態時の移動処理
void Player::translate_block(float delta_time, float speed) {
    switch (current_motion()) {
        // 構え時もしくは構え移動時
    case Motion_Block_Idle:
    case Motion_Block_WalkF: case Motion_Block_WalkB:
    case Motion_Block_WalkL: case Motion_Block_WalkR:
        move_blocking(delta_time);
        break;
        // 被撃時
    case Motion_Block_Hit:
        // ノックバックさせ減速させる
        transform_.translate(velocity_ * delta_time, GStransform::Space::World);
        velocity_ -= GSvector3{ velocity_.x, 0.0f,velocity_.z } *Knockback_DecayRate_Block * delta_time;
        break;
    default:
        return;
    }
}

// ジャンプ時の移動処理
void Player::translate_jump(float delta_time, float speed) {
    // 移動量を計算
    GSvector3 move_velocity = calc_velocity(parameters_.move_speed().speed(), input_.stick_left(), false);

    // 向きの補間
    if (move_velocity.length() > 0) {
        GSquaternion rotation = GSquaternion::rotateTowards(transform_.rotation(), GSquaternion::lookRotation(move_velocity), Rotate_Speed_Jump * delta_time);
        transform_.rotation(rotation);
    }

    // xz成分を更新して平行移動する
    velocity_ = GSvector3{ move_velocity.x, velocity_.y, move_velocity.z };
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

// 移動方向の計算
GSvector2 Player::move_direction(float direction) {
    return GSvector2::createFromDirection(direction);
}

// ダメージボイス再生
void Player::damage_voice() {
    int num = SE_Damage_Player1 + gsRand(0, Player_Damage_Voice - 1);
    SEManager::get_instance().play_se(num);
}

// 死亡時ボイス再生
void Player::death_voice() {
    SEManager::get_instance().play_se(SE_Death_Player);
}

// 特殊技ゲージ増加処理
void Player::add_sp_gauge(int attack, float rate) {
    // 増加前の状態を保存
    bool is_prev_max = parameters_.sp().is_full();
    // ゲージ増加
    parameters_.sp().add(attack * rate);
    // 増加前が満タンでないかつ増加後が満タンなら追加処理
    if (!is_prev_max && parameters_.sp().is_full()) {
        // SE再生
        SEManager::get_instance().play_se(SE_SP_Full);
        // コントローラー振動
        set_vibration(60, Vibration_Power_Strong);
    }
}

// 防御モーション中の移動処理
void Player::move_blocking(float delta_time) {
    // 移動量を計算
    GSvector3 move_velocity = calc_velocity(parameters_.move_speed().speed(), input_.stick_left(), false);
    // モーションを決定
    GSuint motion_after{ Motion_Block_Idle };
    // 移動量がない場合は処理終了
    if (move_velocity.length() == 0) {
        change_state(State_Block, Motion_Block_Idle);
        return;
    }
    // 移動方向に応じてモーション変更
    if (input_.stick_left().x < -DeadZone_StateBlock) motion_after = Motion_Block_WalkL;
    if (input_.stick_left().x > DeadZone_StateBlock)  motion_after = Motion_Block_WalkR;
    if (input_.stick_left().y < -DeadZone_StateBlock) motion_after = Motion_Block_WalkF;
    if (input_.stick_left().y > DeadZone_StateBlock)  motion_after = Motion_Block_WalkB;
    change_state(State_Block, motion_after);
    // xz成分を更新して平行移動する
    velocity_ = GSvector3{ move_velocity.x, velocity_.y, move_velocity.z };
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}

// 状態の追加
void Player::add_states(IWorld* world) {
    add_state(State_Attack, std::make_shared<CharaState_Attack>(this, Player_Combo), world);
    add_state(State_Block, std::make_shared<CharaState_Block>(this), world);
    add_state(State_Damage, std::make_shared<CharaState_Damage>(this), world);
    add_state(State_Down, std::make_shared<CharaState_Down>(this, Dead_Timer), world);
    add_state(State_Jump, std::make_shared<CharaState_Jump>(this), world);
    add_state(State_JumpAttack, std::make_shared<CharaState_JumpAttack>(this), world);
    add_state(State_Special, std::make_shared<CharaState_Special>(this), world);
    add_state(State_Move, std::make_shared<CharaState_Move>(this), world);
}

// 個別のアニメーションイベントを追加
void Player::this_add_events() {
    mesh_.add_event(Motion_AttackSpecial_Prepare, 1, [=] {set_vibration(60, Vibration_Power_Special); });
    mesh_.add_event(Motion_AttackSpecial_Prepare, 1, [=] {SEManager::get_instance().play_se(SE_PlayerSP0); });
    mesh_.add_event(Motion_AttackSpecial, 5, [=] {world_->change_speed(0.3f); });
    mesh_.add_event(Motion_AttackSpecial, 45, [=] {world_->change_speed(1.0f); });
    mesh_.add_event(Motion_Down_Wake, 1, [=] {SEManager::get_instance().play_se(SE_PlayerReact); });
}

// 武器の描画
void Player::draw_weapon() const {
    glPushMatrix();
    //右手のボーンに武器のメッシュを追加
    glMultMatrixf(mesh_.bone_matrices(45));
    gsDrawMesh(Weapon_LongSword);
    glPopMatrix();
}
// 防具の描画
void Player::draw_shield() const {
    glPushMatrix();
    //左手のボーンに防具のメッシュを追加
    glMultMatrixf(mesh_.bone_matrices(24));
    gsDrawMesh(Shield_SmallShield);
    glPopMatrix();
}

// パッドの振動を設定
void Player::set_vibration(int timer, float power) {
    // 既に振動中なら処理しない
    if (vibration_timer_ > 0) return;
    vibration_timer_ = timer;
    vibration_power_ = CLAMP(power, 0, 1);
}

// パッドの振動状態の更新
void Player::update_vibration(float delta_time) {
    vibration_timer_ -= delta_time;
    if (vibration_timer_ <= 0) vibration_power_ = 0;
    gsXBoxPadSetVibration(0, vibration_power_, vibration_power_);
}

// 瀕死時の更新処理
void Player::update_dying(float delta_time) {
    // 特殊技発動中は処理しない
    if (current_state() == State_Special) return;
    // 特殊技ゲージを徐々に増加させる
    danger_add_sp_timer_ -= delta_time;
    if (danger_add_sp_timer_ <= 0) {
        add_sp_gauge(1);
        danger_add_sp_timer_ = Default_Danger_Add_SP_Timer;
    }
}
