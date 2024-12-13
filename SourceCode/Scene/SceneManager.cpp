#include "Assets.h"
#include "SceneNull.h"
#include "SceneManager.h"

#include <imgui/imgui.h>

// コンストラクタ
SceneManager::SceneManager() : current_scene_{ std::make_shared<SceneNull>() } {
}

// デストラクタ
SceneManager::~SceneManager() {
    clear();
}

// 更新
void SceneManager::update(float delta_time) {
    // シーンの更新
    current_scene_->update(delta_time);
    // シーンが終了しているか？
    if (current_scene_->is_end()) {
        // シーンを変更する
        change(current_scene_->next());
    }
}

// 描画
void SceneManager::draw() const {
    // 現在のシーンを描画
    current_scene_->draw();
}

// 終了
void SceneManager::end() {
    // 現在のシーンを終了
    current_scene_->end();
    // nullシーンにしておく
    current_scene_ = std::make_shared<SceneNull>();
}

// シーンの追加
void SceneManager::add(const GSuint num, std::shared_ptr<IScene> scene) {
    scenes_[num] = scene;
}

// シーンの変更
void SceneManager::change(const GSuint& name) {
    // 現在のシーンを(変更前)終了
    end();
    // 現在のシーン(変更前→変更後)を変更
    current_scene_ = scenes_[name];
    // 現在のシーン(変更後)を開始
    current_scene_->start();
}

// シーンの消去
void SceneManager::clear() {
    // 念のためシーン終了
    end();
    // unordered_mapを削除
    scenes_.clear();
    // 複数シーンで使用するアセットの削除
    clear_assets();
}

// 複数シーンで使用するアセットの削除
void SceneManager::clear_assets() {
    gsDeleteTexture(Texture_Fade);
    gsDeleteTexture(Texture_Number);
}
