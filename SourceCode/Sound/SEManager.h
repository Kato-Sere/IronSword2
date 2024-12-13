#ifndef SE_MANAGER_H_
#define SE_MANAGER_H_

#include "UserSettings.h"
#include <GStype.h>
#include <gslib.h>
#include <memory>

// SE再生管理クラス
class SEManager {
private:
    // コンストラクタ
    SEManager() {}

public:
    // インスタンス取得
    static SEManager& get_instance();

    // SE再生・通常
    void play_se(GSuint num, float volume = 1.0f);

    // コピー禁止
    SEManager(SEManager const&) = delete;
    SEManager& operator=(SEManager const&) = delete;

private:
    // ユーザー設定
    UserSettings settings_;
};


#endif // !SE_MANAGER_H_
