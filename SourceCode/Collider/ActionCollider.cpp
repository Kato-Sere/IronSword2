#include "Assets.h"
#include "ActionCollider.h"
#include "Actor/ActorEnum.h"
#include "Sound/SEManager.h"

#include <imgui/imgui.h>

// コンストラクタ
ActionCollider::ActionCollider(IWorld* world, Actor* owner, const BoundingSphere& collider,
    const std::string& owner_name, const GSuint& tag, float lifespan, int attack, float delay,
    const GSvector3& velocity, const std::bitset<8>& sp_effect) :
    lifespan_timer_{ lifespan }, delay_timer_{ delay }, owner_name_{ owner_name } {
    // ワールドの設定
    world_ = world;
    // 親の設定
    owner_ = owner;
    // 衝突判定を初期化
    collider_ = BoundingSphere{ collider.radius_ };
    // タグ名の設定
    tag_ = tag;
    // 名前の設定
    name_ = "ActionCollider";
    // 攻撃力の設定
    attack_ = attack;
    // 速度の設定
    velocity_ = velocity;
    // ノックバック強度の設定
    sp_effect_ = sp_effect;
    // 座標の初期化
    transform_.position(collider.center_);
    // 衝突判定を無効にする
    enable_collider_ = false;
}

// 更新
void ActionCollider::update(float delta_time) {
#if _DEBUG
    //debug_update(delta_time);
#endif // _DEBUG
    // 遅延時間の更新
    delay_timer_ -= delta_time;
    // 遅延時間が経過したら衝突判定を有効にする
    if (delay_timer_ <= 0.0f) {
        // 衝突判定を有効にする
        enable_collider_ = true;
        // 寿命が尽きたら死亡
        if (lifespan_timer_ <= 0.0f) prepare_erase();
        // 寿命の更新
        lifespan_timer_ -= delta_time;
    }

    // 移動する
    if (owner_ != nullptr) {
        GSvector3 velocity = velocity_ == GSvector3::zero() ? owner_->velocity_ : GSvector3::zero();
        if (delay_timer_ <= 0.0f) velocity += velocity_;
        transform_.translate(velocity, GStransform::Space::World);
    } else {
        transform_.translate(velocity_, GStransform::Space::World);
    }
}

// 描画
void ActionCollider::draw() const {
    // 衝突判定のデバッグ表示(必要ないときはコメントアウト)
    //if (delay_timer_ <= 0.0f) collider().draw();
    //collider().draw();
}

// 衝突リアクション
void ActionCollider::react(Actor& other) {
    // 自分と同じ名前か親と同じタグなら衝突しない
    if (other.name() == name_ || other.tag() == owner_->tag()) return;
    // 不可視の壁とは衝突しない
    if (other.name() == "InvisibleWall") return;
    // 衝突相手に応じたSEを再生
    play_hit_sound(&other);
    // 衝突したら死亡
    prepare_erase();
}

// 衝突相手に合わせたSEを再生する
void ActionCollider::play_hit_sound(Actor* other) {
    // 再生するSEとその系統の種類
    int sound = -1; int count = 0;
    // 衝突相手のタグと名前を取得
    GSuint tag = other->tag();
    std::string name = other->name();
    // キャラクター→鎧に当たる音
    if (tag == Tag_Player || tag == Tag_Enemy) {
        // 特殊技発動時は処理しない
        GSuint state = static_cast<Character*>(other)->current_state();
        if (state == State_Special) return;
        // ダウン状態かつ特定のモーションではないときは処理しない
        GSuint motion = static_cast<Character*>(other)->current_motion();
        if (state == State_Down && motion != Motion_Down_In_Land) return;
        sound = SE_Attack_Hit1; count = 3;
    }
    // 木箱→木箱を攻撃する音
    else if (name == "WoodBox") {
        sound = SE_Attack_Wood; count = 1;
    }
    // アイテム壺→壺にひびが入る音
    else if (other->tag() == Tag_Item) {
        if (name != "ItemPot") return;
        sound = SE_Attack_Pot1; count = 3;
    }

    // 想定外の数値になっていないか？
    if (sound == -1 || count == 0) {
        OutputDebugString(L"攻撃判定のSE再生処理が不正です\n");
        return;
    }
    // SE再生
    SEManager::get_instance().play_se(gsRand(sound, sound + count - 1));
}

// デバッグ用更新
void ActionCollider::debug_update(float delta_time) {
    ImGui::Begin("actionCollider");
    ImGui::Value("life", lifespan_timer_);
    ImGui::DragFloat3("pos", transform_.position());
    ImGui::Value("delay", delay_timer_);
    ImGui::DragFloat3("vel", velocity_);
    ImGui::End();
}
