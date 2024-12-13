#ifndef PLAYER_UI_H_
#define PLAYER_UI_H_

#include "FixedHPGauge.h"
#include "SPGauge.h"
#include "KOCount.h"
#include "TimeLimitTimer.h"
#include "BattleText.h"
#include "Minimap.h"

// UI管理クラス
class PlayerUI : public Actor {
public:
    // コンストラクタ
    PlayerUI(IWorld* world, float time);

    // 更新
    void update(float delta_time) override;
    // GUIの描画
    void draw_gui() const override;
    // 撃破数管理クラスの取得
    KOCount& ko_count();
    // 残り時間管理クラスの取得
    TimeLimitTimer& timer();
    // 戦闘テキスト表示クラスの取得
    BattleText& battle_text();

private:
    // 操作説明用画像の描画
    void draw_button_guid() const;

private:
    // プレイヤー
    Character* player_{ nullptr };
    // 体力ゲージ
    FixedHPGauge hp_gauge_;
    // 特殊技ゲージ
    SPGauge sp_gauge_;
    // 撃破数表示
    KOCount ko_count_;
    // 残り時間表示
    TimeLimitTimer timer_;
    // マップ
    Minimap map_{ nullptr };
    // 戦闘テキスト
    BattleText battle_text_;
};

#endif // !PLAYER_UI_H_
