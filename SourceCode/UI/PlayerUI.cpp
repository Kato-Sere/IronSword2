#include "Assets.h"
#include "PlayerUI.h"
#include "Actor/ActorEnum.h"
#include "World/IWorld.h"

// 体力ゲージを表示する座標
const GSvector2 HP_Gauge_Position{ 50, 641 };
// 体力ゲージの幅
const int HP_Gauge_Width{ 300 };
// 体力ゲージの高さ
const int HP_Gauge_Height{ 20 };
// 体力ゲージの描画色
const GScolor HP_Gauge_Color{ 0.0f, 1.0f, 0.0f, 1.0f };

// 特殊技ゲージを表示する座標
const GSvector2 SP_Gauge_Position{ 50, 661 };
// 特殊技ゲージの幅
const int SP_Gauge_Width{ 200 };
// 特殊技ゲージの高さ
const int SP_Gauge_Height{ 14 };
// 特殊技ゲージの描画色
const GScolor SP_Gauge_Color{ 1.0f, 1.0f, 0.0f, 1.0f };

// 操作説明を描画する座標
const GSvector2 Button_Guid_Position{ 988.0f, 315.0f };

// コンストラクタ
PlayerUI::PlayerUI(IWorld* world, float time) {
    world_ = world;
    name_ = "PlayerUI";
    tag_ = Tag_UI;
    // プレイヤーを検索
    player_ = static_cast<Character*>(world_->find_actor("Player"));
    // 体力ゲージを追加
    hp_gauge_ = FixedHPGauge{ player_, HP_Gauge_Position, HP_Gauge_Width, HP_Gauge_Height, HP_Gauge_Color };
    // 特殊技ゲージを追加
    GSvector2 sp_pos = GSvector2{ HP_Gauge_Position.x, HP_Gauge_Position.y + HP_Gauge_Height };
    sp_gauge_ = SPGauge{ player_, sp_pos, SP_Gauge_Width, SP_Gauge_Height, SP_Gauge_Color };
    // 残り時間の設定
    timer_.set(time);
    // マップにワールドを設定
    map_ = Minimap{ world_ };
}

// 更新
void PlayerUI::update(float delta_time) {
    // 特殊技ゲージの更新
    sp_gauge_.update(delta_time);
    // 残り時間の更新
    timer_.update(delta_time);
    // マップの更新
    map_.update(delta_time);
    // 戦闘テキストの更新
    battle_text_.update(delta_time);
}

// GUIの描画
void PlayerUI::draw_gui() const {
    // 体力ゲージの描画
    hp_gauge_.draw();
    // 特殊技ゲージの描画
    sp_gauge_.draw();
    // 撃破数の描画
    ko_count_.draw();
    // 残り時間の描画
    timer_.draw();
    // マップの描画
    map_.draw();
    // 戦闘テキストの描画
    battle_text_.draw();
    // 操作説明の画像の描画
    draw_button_guid();
}

// 撃破数管理クラスの取得
KOCount& PlayerUI::ko_count() {
    return ko_count_;
}

// 残り時間管理クラスの取得
TimeLimitTimer& PlayerUI::timer() {
    return timer_;
}

// 戦闘テキスト表示クラスの取得
BattleText& PlayerUI::battle_text() {
    return battle_text_;
}

// 操作説明用画像の描画
void PlayerUI::draw_button_guid() const {
    gsDrawSprite2D(Texture_Button_Guid, &Button_Guid_Position, NULL, NULL, NULL, NULL, 0.0f);
}
