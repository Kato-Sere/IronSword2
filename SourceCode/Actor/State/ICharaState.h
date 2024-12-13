#ifndef ICHARA_STATE_H_
#define ICHARA_STATE_H_

#include <gslib.h>

// 前方宣言
class IWorld;
class Actor;

// 状態クラス
class ICharaState {
public:
    // デストラクタ
    virtual ~ICharaState() = default;

    // 開始
    virtual void start() = 0;
    // 更新
    virtual void update(float delta_time)  = 0;
    // 終了しているか？
    virtual bool is_end() const = 0;
    // 終了
    virtual void end() = 0;
    // 衝突リアクション
    virtual void react(Actor& other) = 0;

public:
    // ワールド
    IWorld* world_{ nullptr };
    // 状態番号
    GSuint statenum_{ static_cast<GSuint>(-1) };

protected:
    // 終了フラグ
    bool is_end_{ false };
};

#endif // !ICHARA_STATE_H_
