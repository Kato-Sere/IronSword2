#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <GStype.h>
#include <gslib.h>
#include <memory>
#include <string>
#include <unordered_map>

class IScene; //ISceneクラスの前方宣言

//シーン管理クラス
class SceneManager {
public:
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// 終了
	void end();

	// シーンの追加
	void add(const GSuint num, std::shared_ptr<IScene> scene);
	// シーンの変更
	void change(const GSuint& num);
	// シーンの消去
	void clear();
	// コピー禁止
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

private:
    // 複数シーンで使用するアセットの削除
    void clear_assets();

private:
	// シーン
	std::unordered_map<GSuint, std::shared_ptr<IScene>> scenes_;
	// 現在のシーン
	std::shared_ptr<IScene> current_scene_;
};

#endif // !SCENE_MANAGER_H_
