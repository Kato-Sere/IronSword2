#ifndef CHARA_STATE_NULL_H_
#define CHARA_STATE_NULL_H_

#include "ICharaState.h"

// NULL状態 nullptrの状態確認を省くためSceneNullを参考に作成
class CharaState_Null : public ICharaState {
public:
    // 開始
    virtual void start() override {}
    // 更新
    virtual void update(float delta_time) override {}
    // 終了しているか？
    virtual bool is_end() const override { return false; }
    // 終了
    virtual void end() override {}
    // 衝突リアクション
    virtual void react(Actor& other) override {}
};

#endif // !CHARA_STATE_NULL_H_
