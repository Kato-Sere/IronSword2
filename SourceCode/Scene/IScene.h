#ifndef ISCENE_H_
#define ISCENE_H_

#include <GStype.h>

// シーン抽象インターフェース 授業教材使用
class IScene {
public:
    // 仮想デストラクタ
    virtual ~IScene() = default;
    // 開始
    virtual void start() = 0;
    // 更新
    virtual void update(float delta_time) = 0;
    // 描画
    virtual void draw() const = 0;
    // 終了しているか？
    virtual bool is_end() const = 0;
    // このシーンの番号を返す
    virtual GSuint current() const = 0;
    // 次のシーン名を返す
    virtual GSuint next() const = 0;
    // 終了
    virtual void end() = 0;

private:
    // アセット読み込み
    virtual void load_assets() = 0;
    // アセット削除
    virtual void delete_assets() = 0;
};

#endif // !ISCENE_H_

