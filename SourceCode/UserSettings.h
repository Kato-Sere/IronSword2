#ifndef USER_SETTINGS_H_
#define USER_SETTINGS_H_

// 設定用
struct UserSettings {
    // 敵の頭上に体力ゲージを描画するか？
    bool Draw_Enemies_HP_Gauge{ true };
    // 上下のカメラ操作を反転するか？
    bool Reverse_Camera_UB{ true };
    // 左右のカメラ操作を反転するか？
    bool Reverse_Camera_LR{ false };
    // BGMの音量
    float Volume_BGM{ 1.0f };
    // SEの音量
    float Volume_SE{ 1.0f };
};

#endif // !USER_SETTINGS_H_
