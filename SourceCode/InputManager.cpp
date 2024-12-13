#include "InputManager.h"
#include <GStype.h>

// これ以下のスティック入力値は無効にする値
const float Dead_Power_L{ 0.1f };
const float Dead_Power_R{ 0.7f };

// 左スティック
GSvector2 InputManager::stick_left() {
    // パッドの左スティック
    GSvector2 pad_left;
    gsXBoxPadGetLeftAxis(0, &pad_left);
    GSvector2 result{ 0, 0 }; // 返却用
    if (pad_left == GSvector2{ 0.0f, 0.0f }) {
        // 入力方向の判定
        if (gsGetKeyState(GKEY_W)) result.y += 1; // 上
        if (gsGetKeyState(GKEY_S)) result.y -= 1; // 下
        if (gsGetKeyState(GKEY_A)) result.x -= 1; // 左
        if (gsGetKeyState(GKEY_D)) result.x += 1; // 右
    } else {
        // 一定の値以下なら入力値はなしとして扱う
        result = ABS(pad_left.length()) > Dead_Power_L ? pad_left : GSvector2::zero();
    }
    return result;
}

// 右スティック
GSvector2 InputManager::stick_right() {
    // パッドの右スティック
    GSvector2 pad_right;
    gsXBoxPadGetRightAxis(0, &pad_right);
    GSvector2 result{ 0, 0 }; // 返却用
    if (pad_right == GSvector2{ 0.0f,0.0f }) {
        // 入力方向の判定
        if (gsGetKeyState(GKEY_UPARROW))    result.y += 1; // 上
        if (gsGetKeyState(GKEY_DOWNARROW))  result.y -= 1; // 下
        if (gsGetKeyState(GKEY_LEFTARROW))  result.x -= 1; // 左
        if (gsGetKeyState(GKEY_RIGHTARROW)) result.x += 1; // 右
    } else {
        // 一定の値以下ならその方向の入力値はなしとして扱う
        result.x = ABS(pad_right.x) > Dead_Power_R ? pad_right.x : 0;
        result.y = ABS(pad_right.y) > Dead_Power_R ? pad_right.y : 0;
    }
    return result;
}

// Xボタン
bool InputManager::button_x() {
    return gsGetKeyTrigger(GKEY_J) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X);
}

// Yボタン
bool InputManager::button_y() {
    return gsGetKeyTrigger(GKEY_K) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_Y);
}

// Aボタン
bool InputManager::button_a() {
    return gsGetKeyTrigger(GKEY_M) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A);
}

// Bボタン
bool InputManager::button_b() {
    return gsGetKeyTrigger(GKEY_L) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B);
}

// RBボタン
bool InputManager::button_rb() {
    return gsGetKeyState(GKEY_U) || gsXBoxPadButtonState(0, GS_XBOX_PAD_RIGHT_SHOULDER);
}

// LBボタン
bool InputManager::button_lb() {
    return gsGetKeyState(GKEY_R) || gsXBoxPadButtonState(0, GS_XBOX_PAD_LEFT_SHOULDER);
}

// ABXYいずれかのボタン
bool InputManager::button_any() {
    return button_a() || button_b() || button_x() || button_y();
}
