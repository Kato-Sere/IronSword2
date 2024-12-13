#include "Assets.h"
#include "Character.h"
#include "CharacterConstantValue.h"
#include "Ranks.h"
#include "Actor/ActorEnum.h"
#include "Collider/ActionCollider.h"
#include "Sound/SEManager.h"
#include "UI/PlayerUI.h"
#include "World/Field.h"
#include "World/IWorld.h"
#include "World/Line.h"
#include "Actor/State/CharaState_Null.h"
#include "Actor/AtkMethod/FireBall.h"

#include <GSeffect.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <imgui/imgui.h>

/* ファイル読み込み時のアドレス */
const std::string File_Pos_Front{ "Assets/Actors/Data/" };      // 前のアドレス
const std::string Parameter_Pos{ "ActorParameters.csv" };       // 基本パラメーター
const std::string AtkParam_Pos{ "/AttackParameterData.csv" };   // 攻撃データ
const std::string AtkCollider_Pos{ "/AttackColliderData.csv" }; // 攻撃判定データ
const std::string AtkEffect_Pos{ "/AttackEffectData.csv" };     // エフェクトデータ
const std::string AtkSE_Pos{ "/AttackSEData.csv" };             // ＳＥデータ

// ノックバック値の種類
const int KnockBacks_Count{ 3 };
// ダメージモーションの再生速度
const float Damage_MotionSpeed{ 1.5f };

// 吹き飛ばし速度減衰率
const float BlowOff_Decay_Rate{ 0.2f };
// ダメージを与えた側の特殊技ゲージ増加率
const float SP_Add_Rate_Give{ 0.3f };
// ダメージを受けた側の特殊技ゲージ増加率
const float SP_Add_Rate_Take{ 0.6f };

// 攻撃音の種類
const int Attack_SE{ 3 };

// コンストラクタ
Character::Character() : current_state_{ std::make_shared<CharaState_Null>() } {}

// パラメーターの読み込み
void Character::load_param(GSuint param_r, const std::string& name) {
    // 基本パラメータ－用ファイルの読み込み
    CSVReader file_actor;
    file_actor.load(File_Pos_Front + Parameter_Pos);
    // 基本パラメーターの取得
    ColliderParam collider = ColliderParam{ file_actor.getf(param_r, ParamC_Height),
                                            file_actor.getf(param_r, ParamC_Radius) };
    HitPoint hp = HitPoint{ file_actor.geti(param_r, ParamC_HP) };
    MoveSpeed speed = MoveSpeed{ file_actor.getf(param_r,ParamC_MoveSpeed) };
    BlockPower block = BlockPower{ file_actor.geti(param_r, ParamC_BlockPower) };
    AttackRate rate = AttackRate{ 1.0f };
    float jump = file_actor.getf(param_r, ParamC_JumpPower);

    // 最大コンボ数が0(攻撃しない)アクターの時はここまでで結果を返却
    int combo = file_actor.geti(param_r, ParamC_ComboCount);
    if (combo == 0) {
        parameters_ = CharacterParameters{ collider, hp, speed, block, rate, jump,
        std::vector<ActionData>{}, std::vector<ActionData>{} , ActionData{}, ActionData{}, std::vector<float>{} };
        return;
    }

    // 攻撃系パラメーターファイルの読み込み
    unsigned int file_count = 4;
    std::vector<CSVReader> readers{ file_count, CSVReader{} };
    readers[File_Param].load(File_Pos_Front + name + AtkParam_Pos);
    readers[File_Collider].load(File_Pos_Front + name + AtkCollider_Pos);
    readers[File_Effect].load(File_Pos_Front + name + AtkEffect_Pos);
    readers[File_SE].load(File_Pos_Front + name + AtkSE_Pos);

    // 攻撃データ用vector
    std::vector<ActionData> action_n, action_c;
    action_n.reserve(combo); action_c.reserve(combo); // capacity確保
    ActionData action_s, action_jc;
    // 通常攻撃・チャージ攻撃のデータを取得
    int row;
    GSuint motion_n, motion_c;
    for (int i = 0; i < combo; ++i) {
        row = i + 1;
        motion_n = Motion_AttackNormal1 + i;
        action_n.push_back(action_data(motion_n, row, readers));
        row += combo;
        motion_c = Motion_AttackCharge1 + i;
        action_c.push_back(action_data(motion_c, row, readers));
    }
    // 以下必要に応じて特殊攻撃データを取得
    // 特殊技のデータ
    std::bitset<8> sp_attack = file_actor.get_bit8(param_r, ParamC_SPAttack);
    int sp_row = 1 + combo * 2;
    row = sp_row;
    if (sp_attack.test(SPAttack_Special)) action_s = action_data(Motion_AttackSpecial, row, readers);
    // ジャンプチャージ攻撃データ
    row += 1;
    if (sp_attack.test(SPAttack_JumpC)) action_jc = action_data(Motion_JumpC_Out, row, readers);

    // ノックバック値の取得
    std::vector<float> knockbacks;
    for (int i = 0; i < KnockBacks_Count; ++i) {
        knockbacks.push_back(file_actor.getf(param_r, ParamC_Knockback + i));
    }

    // 取得した値をパラメーターに設定
    parameters_ = CharacterParameters{ collider, hp, speed, block, rate, jump,
                                    action_n, action_c, action_jc, action_s, knockbacks };
}

// パラメーター取得 （const版）
const CharacterParameters& Character::parameters() const {
    return parameters_;
}

// パラメーター取得
CharacterParameters& Character::parameters() {
    return parameters_;
}

// 個別の削除準備
void Character::this_prepare_erase() {
    // アニメーションイベントの削除
    mesh_.delete_event();
    // 状態の消去
    states_.clear();
}

// アニメーションメッシュ取得
const AnimatedMesh& Character::mesh() const {
    return mesh_;
}

// モーションの終了時間を取得
const float Character::current_motion_end_time() const {
    return mesh_.current_motion_end_time();
}

// 現在のアニメーションの再生割合を取得
const float Character::ratio_motion_time() const {
    if (mesh_.current_motion_end_time() == 0) return 0;
    return state_timer_ / mesh_.current_motion_end_time();
}

// 状態の追加
void Character::add_state(const GSuint num, std::shared_ptr<ICharaState> state, IWorld* world) {
    states_[num] = state;
    state->world_ = world;
    state->statenum_ = num;
}

// 現在の状態の取得
const GSuint Character::current_state() const {
    if (current_state_ == nullptr) return (GSuint)-1;
    return current_state_->statenum_;
}

// 状態の変更
void Character::change_state(GSuint state, GSuint motion, bool loop, float speed, bool reset) {
    // モーションの変更
    change_motion(motion, loop, speed);
    // 状態の変更
    if (current_state_ != states_[state] || reset) {
        if (current_state_ != nullptr) current_state_->end();
        current_state_ = states_[state];
        current_state_->start();
    }
}

// モーションの変更(状態変更なし)
void Character::change_motion(GSuint motion, bool loop, float speed) {
    // モーション番号の更新
    motion_ = motion;
    // モーションのループ指定
    motion_loop_ = loop;
    // モーション再生速度の変更
    motion_speed_ = speed;
    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_, motion_speed_);
    // 状態タイマーの初期化
    state_timer_ = 0.0f;
}

// 状態ごとの経過時間取得
const float Character::state_timer() const {
    return state_timer_;
}

// 状態遷移フラグを取得
const Character::StateTrigger& Character::state_trigger() const {
    return state_trigger_;
}

// モーション番号を取得
const GSuint Character::current_motion() const {
    return motion_;
}

// 攻撃を受けるタグを取得
const GSuint Character::damage_tag() const {
    return damage_tag_;
}

// 被ダメージ処理
void Character::take_damage(Actor& other) {
    // 既に死亡しているなら処理しない
    if (is_dead()) return;
    // 制限時間を過ぎている場合は処理しない
    if (world_->player_ui()->timer().is_time_over()) return;

    // ヒットエフェクト再生
    const GSvector3 position = GSvector3{
        transform_.position().x,
        collider_.center().y,
        transform_.position().z
    };
    gsPlayEffect(Effect_Hit, &position);
    // ダメージを受ける
    int atk = other.attack();
    if (name_ == "Ranks") {
        // 兵士のみ、隊長が不在ならダメージ量を現在の体力と同じにする
        if (!static_cast<Ranks*>(this)->is_arive_leader()) atk = parameters_.hp().current();
    }
    parameters_.hp().sub(atk);
    // 体力がなくなった場合専用の処理を行う
    if (is_dead()) down();
    // 特殊技ゲージを増加 ダメージを受ける時は増加量を増やす
    GSuint other_state = static_cast<Character*>(other.owner())->current_state();
    if (other.owner() != nullptr && other_state != State_Special) {
        // 相手の処理
        dynamic_cast<Character*>(other.owner())->add_sp_gauge(other.attack(), SP_Add_Rate_Give);
        // 自分の処理
        add_sp_gauge(other.attack(), SP_Add_Rate_Take);
    }

    // 被ダメージSE再生
    if (is_dead()) death_voice();
    else damage_voice();

    // ダウン中か？
    if (current_state() == State_Down) {
        change_state(State_Down, Motion_Damage_InDown, false, 1.0f);
        return;
    }

    // 被ダメージ後の状態を決定
    GSuint state, motion; GSvector3 direction; float motion_speed;
    bool str_knockback = other.sp_effect().test(SPEffect_KnockBack); // 強ノックバック判定
    bool is_reset = current_state() == State_Damage;
    // ジャンプ中でないか？
    if (current_state() != State_Jump && current_state() != State_JumpAttack) {
        // 吹き飛ばすかでノックバック方向を計算
        bool is_blowoff = other.sp_effect().test(SPEffect_BlowOff);
        if (is_blowoff) {
            // 衝突相手と反対方向に吹き飛ばす
            direction = GSvector3{ other.owner()->transform().position() - transform_.position() }.normalized();
        } else {
            // 衝突相手の正面方向に吹き飛ばす
            if (other.owner() != nullptr) direction = -other.owner()->transform().forward();
            else direction = -other.transform().forward();
        }

        // 死亡しているかでノックバック速度を決定
        float knockback_spd;
        if (is_dead()) knockback_spd = parameters_.knockback_speed(KnockBack_Dead);
        else knockback_spd = parameters_.knockback_speed(str_knockback);
        // 吹き飛ばす場合は速度を減衰させる
        if (is_blowoff) knockback_spd *= BlowOff_Decay_Rate;

        // 最終的な速度を計算
        velocity_ = direction * knockback_spd;

        // 吹き飛ばすか、死亡しているかでダメージ処理後の状態を決定
        if (is_blowoff) {
            state = State_BlowOff; motion = Motion_Damage_BlowOff;
        } else if (is_dead()) {
            state = State_Down; motion = Motion_Down_In_Land;
        } else {
            state = State_Damage;
            motion = other.sp_effect().test(SPEffect_Combo) ? Motion_Damage_Weak : Motion_Damage_Strong;
        }
        // モーション再生速度を設定
        motion_speed = Damage_MotionSpeed;
    } else {
        // 衝突相手と反対方向に吹き飛ばす
        direction = GSvector3{ other.owner()->transform().position() - transform_.position() }.normalized();
        velocity_ = direction * parameters_.knockback_speed(str_knockback);
        // ダメージ処理後の状態を設定
        state = State_Down; motion = Motion_Damage_Air; motion_speed = 1.0f;
    }

    // 処理結果に基づき状態変更
    change_state(state, motion, false, motion_speed, is_reset);
}

// 攻撃判定の生成
void Character::generate_collider(const ActionData& data) {
    // 攻撃判定データの数だけ攻撃判定を生成する
    for (int i = 0; i < data.collider_count; ++i) {
        AttackColliderParam collider_data = data.collider_data[i];
        // 前方向・右方向のベクトルを取得
        GSvector3 direction = GSvector3::zero();
        float colliderRadian = collider_data.direction * (M_PI / 180.0f);
        GSvector3 forward = transform_.forward() * cosf(colliderRadian);
        GSvector3 right = transform_.right() * sinf(colliderRadian);
        direction += forward + right;
        // 出現座標の計算
        GSvector3 position = transform_.position() + direction * collider_data.distance;
        position.y = parameters_.collider().height();
        // 移動速度の計算
        GSvector3 velocity = direction * collider_data.speed;
        velocity.y = 0.0f;
        // 衝突判定生成
        BoundingSphere collider{ collider_data.radius, position };
        // パラメーターの計算
        int attack = data.attack * parameters_.attack_rate().rate(); // 攻撃力
        world_->add_actor(new ActionCollider{ world_, this, collider, name(), attack_tag_,
            data.lifespan, attack, 0, velocity, data.sp_effect });
    }
}

// 火球の生成
void Character::generate_fireball(const ActionData& data) {
    AttackColliderParam collider_data = data.collider_data[0];
    // 前方向・右方向のベクトルを取得
    GSvector3 direction = GSvector3::zero();
    float colliderRadian = DEG_TO_RAD(collider_data.direction);;
    GSvector3 forward = transform_.forward() * cosf(colliderRadian);
    GSvector3 right = transform_.right() * sinf(colliderRadian);
    direction += forward + right;
    // 出現座標の計算
    GSvector3 position = transform_.position() + direction * collider_data.distance;
    position.y += parameters_.collider().height();
    // 移動速度の計算
    GSvector3 velocity = direction * data.collider_data[0].speed;
    velocity.y = 0;
    // 衝突判定生成
    BoundingSphere collider{ collider_data.radius, position };
    // 攻撃力の計算
    int attack = data.attack * parameters_.attack_rate().rate();
    world_->add_actor(new FireBall(world_, this, collider, data.lifespan, attack, velocity, data.effect_data[0], data.sp_effect));
}

// 移動量の計算
GSvector3 Character::calc_velocity(float speed, const GSvector2& direction, bool override_by_self) {
    GSvector3 forward, right;
    // 前方向・右方向のベクトルを取得
    if (override_by_self) {
        forward = transform_.forward();
        right = transform_.right();
    } else {
        forward = world_->camera()->transform().forward();
        right = world_->camera()->transform().right();
    }
    forward.y = 0.0f;
    right.y = 0.0f;

    // 移動方向を取得
    GSvector3 result = GSvector3::zero();
    result += right * direction.x;
    result += forward * direction.y;

    //  移動量を計算
    result = result.normalized() * speed * parameters_.move_speed().rate();
    return result;
}

// アニメーションイベントを追加
void Character::add_events(int combo) {
    // 攻撃時のアニメーションイベントを追加
    for (int i = 0; i < combo; ++i) {
        // 通常攻撃用
        add_event(parameters_.action_normal(AtkN_first + i));
        // チャージ攻撃用
        add_event(parameters_.action_charge(AtkC_first + i));
    }
    // 特殊技のアニメーションイベントを追加
    if (parameters_.action_special() != ActionData{}) add_event(parameters_.action_special());
    // ジャンプチャージ攻撃のアニメーションイベントを追加
    if (parameters_.action_jumpc() != ActionData{}) add_event(parameters_.action_jumpc());
    // 歩行時のアニメーションイベントを追加
    mesh_.add_event(Motion_Run, 12, [=] {event_effect(Effect_GroundDust); });
    mesh_.add_event(Motion_Run, 24, [=] {event_effect(Effect_GroundDust); });
    // ダウン時のアニメーションイベントを追加
    mesh_.add_event(Motion_Down, 8, [=] {event_effect(Effect_GroundSmoke); });
    // 着地時のアニメーションイベントを追加
    mesh_.add_event(Motion_Jump_Out, 2, [=] {event_effect(Effect_GroundSmoke); });
}

// 追加するアニメーションイベントを決定
void Character::add_event(const ActionData& data) {
    GSuint motion = data.motion;
    int timing;
    switch (data.attack_type) {
    case AtkType_Slash:      // 斬撃
    case AtkType_ShieldBash: // 盾突進
    case AtkType_Blow:       // 打撃
        // 攻撃判定の追加
        timing = data.collider_timing;
        mesh_.add_event(motion, timing, [=] {generate_collider(data); });
        // エフェクトの追加
        for (int i = 0, count = data.effect_count; i < count; ++i) {
            timing = data.effect_data[i].appear_frame;
            mesh_.add_event(motion, timing, [=] {play_effect(data.effect_data[i]); });
        }
        break;
    case AtkType_FireBall: // 火球
        // 火球の追加
        timing = data.collider_timing;
        mesh_.add_event(motion, 15, [=] {mesh_.change_speed(1.25f); });
        mesh_.add_event(motion, timing, [=] {generate_fireball(data); });
        // エフェクトの追加
        for (int i = 0, count = data.effect_count; i < count; ++i) {
            timing = data.effect_data[i].appear_frame;
            mesh_.add_event(motion, timing, [=] {play_effect(data.effect_data[i]); });
        }
        break;
    default:
        break;
    }

    // SEの追加
    for (int i = 0, count = data.se_count; i < count; ++i) {
        timing = data.se_data[i].play_frame;
        switch (data.se_data[i].se_id) {
        case SEType_None:
            break;
        case SEType_Slash:
            mesh_.add_event(motion, timing, [=] {play_slash(); });
            break;
        case SEType_Fire:
            mesh_.add_event(motion, 1, [=] {SEManager::get_instance().play_se(SE_Fire_Charge); });
            mesh_.add_event(motion, timing, [=] {SEManager::get_instance().play_se(SE_Fire_Appaer); });
            break;
        case SEType_Blow:
            mesh_.add_event(motion, timing, [=] {SEManager::get_instance().play_se(SE_Attack_Blow); });
            break;
        case SEType_PlayerSP:
            mesh_.add_event(motion, 1, [=] {SEManager::get_instance().play_se(SE_PlayerSP1); });
            mesh_.add_event(motion, timing, [=] {SEManager::get_instance().play_se(SE_PlayerSP2); });
            break;
        case SEType_JumpC:
            mesh_.add_event(motion, timing, [=] {SEManager::get_instance().play_se(SE_Attack_JumpC); });
            break;
        default:
            break;
        }
    }
}

// Effekseerのエフェクトを再生
void Character::play_effect(const EffectParam& param) {
    // 指定した座標・回転角・拡大率の行列を作成する
    GSmatrix4 localMat = GSmatrix4::TRS(param.local_position, GSquaternion::euler(param.local_rotation), param.local_scale);
    // ワールド空間に変換する
    GSmatrix4 worldMat = localMat * transform_.localToWorldMatrix();
    // エフェクト再生
    gsPlayEffectEx(param.effect_id, &worldMat);
}

// Effekseerのエフェクトを再生(自身の座標指定)
void Character::event_effect(GSuint effect) {
    const GSvector3& position = transform_.position();
    gsPlayEffect(effect, &position);
}

// Effekseerのエフェクトを再生(色変更版)
void Character::play_effect_col(const EffectParam& param, const GScolor& color) {
    // 色変更
    gsSetEffectColor(param.effect_id, &color);
    // エフェクト再生
    play_effect(param);
}

// 斬撃音再生
void Character::play_slash() {
    int num = SE_Attack_Miss1 + gsRand(0, Attack_SE - 1);
    SEManager::get_instance().play_se(num);
}

// 接地状態を取得
bool Character::is_land() const {
    Line line;
    line.start_ = transform_.position() + collider_.center_;
    line.end_ = transform_.position() + GSvector3{ 0.0f, -0.08f, 0.0f };
    return world_->field()->is_collide(line);
}

// 状態遷移フラグの初期化
void Character::reset_state_trigger() {
    state_trigger_.Move = false;
    state_trigger_.AttackN = false;
    state_trigger_.AttackC = false;
    state_trigger_.Block = false;
    state_trigger_.Jump = false;
    state_trigger_.React = false;
    state_trigger_.Special = false;
    state_trigger_.BattleWalk = false;
}

// 状態更新を行うか？
bool Character::is_update_state() {
    // プレイヤーが死亡していないか？
    if (static_cast<Character*>(world_->find_actor("Player"))->is_dead()) return false;
    // 時間切れでないか？
    if (world_->player_ui()->timer().is_time_over()) return false;
    return true;
}

// 死亡しているか？
bool Character::is_dead() const {
    return parameters_.hp().current() <= 0;
}

// 攻撃データ取得
ActionData Character::action_data(GSuint motion, int data_r, const std::vector<CSVReader>& readers) {
    ActionData result;
    // 基本データ取得
    const CSVReader& csv_param = readers[File_Param];
    result.motion = motion;
    result.attack_type = csv_param.geti(data_r, AtkDataC_AtkType);
    result.lifespan = csv_param.getf(data_r, AtkDataC_LifeSpan);
    result.attack = csv_param.geti(data_r, AtkDataC_Attack);
    result.collider_timing = csv_param.getf(data_r, AtkDataC_Ratio) * mesh_.motion_end_time(motion);
    result.move_start = csv_param.getf(data_r, AtkDataC_MoveStart);
    result.move_end = csv_param.getf(data_r, AtkDataC_MoveEnd);
    result.move_speed = csv_param.getf(data_r, AtkDataC_MoveSpeed);
    result.move_direction = csv_param.getf(data_r, AtkDataC_MoveDirection);
    result.sp_effect = csv_param.get_bit8(data_r, AtkDataC_SPEffect);
    result.animation_speed = csv_param.getf(data_r, AtkDataC_AnimationSpeed);

    // 攻撃判定データ取得
    const CSVReader& csv_collider = readers[File_Collider];
    result.collider_count = csv_param.geti(data_r, AtkDataC_ColliderCount);
    result.collider_data.reserve(result.collider_count);
    for (int j = 0, count = result.collider_count; j < count; ++j) {
        // 参照する列を計算
        int add_c = j * Add_ColliderData_Column;
        AttackColliderParam atk_collider;
        atk_collider.distance = csv_collider.getf(data_r, ColliderDataC_Distance + add_c);
        atk_collider.radius = csv_collider.getf(data_r, ColliderDataC_Radius + add_c);
        atk_collider.direction = csv_collider.getf(data_r, ColliderDataC_Direction + add_c);
        atk_collider.speed = csv_collider.getf(data_r, ColliderDataC_Speed + add_c);
        result.collider_data.push_back(atk_collider);
    }

    // エフェクトデータ取得
    const CSVReader& csv_effect = readers[File_Effect];
    result.effect_count = csv_param.geti(data_r, AtkDataC_EffectCount);
    result.effect_data.reserve(result.effect_count);
    for (int j = 0, count = result.effect_count; j < count; ++j) {
        // 参照する列を計算
        int add_c = j * Add_EffectData_Column;
        EffectParam effect;
        effect.effect_id = csv_effect.geti(data_r, EffectDataC_ID + add_c);
        effect.appear_frame = csv_effect.geti(data_r, EffectDataC_Timing + add_c);
        effect.local_position = GSvector3{
           csv_effect.getf(data_r, EffectDataC_Position + add_c),
           csv_effect.getf(data_r, EffectDataC_Position + 1 + add_c),
           csv_effect.getf(data_r, EffectDataC_Position + 2 + add_c)
        };
        effect.local_rotation = GSvector3{
           csv_effect.getf(data_r, EffectDataC_Rotation + add_c),
           csv_effect.getf(data_r, EffectDataC_Rotation + 1 + add_c),
           csv_effect.getf(data_r, EffectDataC_Rotation + 2 + add_c)
        };
        effect.local_scale = GSvector3{
           csv_effect.getf(data_r, EffectDataC_Scale + add_c),
           csv_effect.getf(data_r, EffectDataC_Scale + 1 + add_c),
           csv_effect.getf(data_r, EffectDataC_Scale + 2 + add_c)
        };
        result.effect_data.push_back(effect);
    }

    // 使用SEデータ取得
    const CSVReader& csv_se = readers[File_SE];
    result.se_count = csv_param.geti(data_r, AtkDataC_SECount);
    result.se_data.reserve(result.se_count);
    for (int j = 0, count = result.se_count; j < count; ++j) {
        // 参照する列を計算
        int add_c = j * Add_SEData_Column;
        SEParam se;
        se.se_id = csv_se.geti(data_r, SEDataC_ID + add_c);
        se.play_frame = csv_se.geti(data_r, SEDataC_Timing + add_c);
        result.se_data.push_back(se);
    }

    return result;
}
