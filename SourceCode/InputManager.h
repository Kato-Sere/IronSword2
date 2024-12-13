#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <gslib.h>

// 入力ボタン管理クラス
class InputManager {
public:
    // 左スティック
    GSvector2 stick_left();
    // 右スティック
    GSvector2 stick_right();

    // Xボタン
    bool button_x();
    // Yボタン
    bool button_y();
    // Aボタン
    bool button_a();
    // Bボタン
    bool button_b();

    // RBボタン
    bool button_rb();
    // LBボタン
    bool button_lb();

    // ABXYいずれかのボタン
    bool button_any();

};


#endif // !INPUT_MANAGER_H_
