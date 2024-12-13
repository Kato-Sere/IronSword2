#include "Assets.h"
#include "Scene/SceneManager.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"
#include "Scene/ResultScene.h"
#include <GSgame.h>
#include <GSeffect.h>

#include <chrono>
#include <imgui/imgui.h>

// ゲームクラス
class MyGame : public gslib::Game {
    // シーンマネージャー
    SceneManager manager_;

public:
    MyGame() : gslib::Game(Screen_Size.x, Screen_Size.y, false) {};

    // 開始
    void start() override {
        // エフェクトの初期化
        gsInitEffect();
        // マウスカーソル非表示
        gsHideMouseCursor();
        // シーンの追加
        manager_.add(Title_Scene, std::make_shared<TitleScene>());
        manager_.add(Game_Scene, std::make_shared<GameScene>());
        manager_.add(Result_Scene, std::make_shared<ResultScene>());
        // タイトルシーンから開始
        manager_.change(Title_Scene);
    }

    // 更新
    void update(float delta_time) override {
#if _DEBUG
        chrono::system_clock::time_point update_start, update_end;
        update_start = chrono::system_clock::now();
#endif // _DEBUG
        // シーンの更新
        manager_.update(delta_time);
#if _DEBUG
        update_end = chrono::system_clock::now();
        float update_time = static_cast<float>(chrono::duration_cast<chrono::microseconds>(update_end - update_start).count() / 1000.0f);
        ImGui::Begin("time");
        ImGui::Value("update_time", update_time);
        ImGui::End();
#endif // _DEBUG

    }

    // 描画
    void draw() override {
#if _DEBUG
        chrono::system_clock::time_point draw_start, draw_end;
        draw_start = chrono::system_clock::now();
#endif // _DEBUG
        // シーンの描画
        manager_.draw();
#if _DEBUG
        draw_end = chrono::system_clock::now();
        float draw_time = static_cast<float>(chrono::duration_cast<chrono::microseconds>(draw_end - draw_start).count() / 1000.0f);
        ImGui::Begin("time");
        ImGui::Value("draw_time", draw_time);
        ImGui::End();
#endif // _DEBUG

    }

    // 終了
    void end() override {
        // シーンの終了
        manager_.end();
        // エフェクトの終了
        gsFinishEffect();
    }

    // 実行中か？(コントローラーから終了できるように上書き)
    bool is_running() override {
        return (gsGetKeyState(GKEY_ESCAPE) ||
            (gsXBoxPadButtonState(0, GS_XBOX_PAD_BACK) && gsXBoxPadButtonState(0, GS_XBOX_PAD_START))) == 0;
    }
};

int main() {
    return MyGame().run();
}
