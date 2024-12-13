#ifndef BGM_MANAGER_H_
#define BGM_MANAGER_H_

#include "UserSettings.h"
#include <GStype.h>
#include <gslib.h>
#include <memory>

// BGM再生管理クラス
class BGMManager {
private:
    // コンストラクタ
    BGMManager() {}

public:
    // インスタンス取得
    static BGMManager& get_instance();

    // 更新
    void update(float delta_time);

    // BGM再生
    void play_bgm(GSuint num, float volume = 1.0f);
    // BGM停止・即時
    void stop_bgm();
    // BGM停止・フェード
    void fade_out_bgm();
    // BGMのフェードアウト処理
    void fade(float delta_time);
    // BGMの音量を取得
    float volume() const;

    // コピー禁止
    BGMManager(BGMManager const&) = delete;
    BGMManager& operator=(BGMManager const&) = delete;

private:
    // インスタンス
    // BGMがフェードアウト中か？
    bool fading_bgm_{ false };
    // BGMの音量
    float volume_bgm_{ 1.0f };
    // ユーザー設定
    UserSettings settings_;
};

#endif // !BGM_MANAGER_H_
