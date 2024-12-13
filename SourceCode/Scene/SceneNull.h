#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include "Assets.h"
#include "IScene.h"

// NULLシーン nullptrのシーンチェックを省くために作成 授業教材使用
class SceneNull : public IScene {
public:
	// 開始
	virtual void start() override {}
	// 更新
	virtual void update(float delta_time) override {}
	// 描画
	virtual void draw() const override {}
	// 終了しているか？
	virtual bool is_end() const override { return false; }
    // このシーンの番号を返す
    virtual GSuint current() const override { return Null_Scene; }
	// 次のシーンを返す
	virtual GSuint next() const override { return Null_Scene; }
	// 終了
	virtual void end() override {}

private:
    // アセット読み込み
    virtual void load_assets() override {};
    // アセット削除
    virtual void delete_assets() override {};
};

#endif // !SCENE_NULL_H_
