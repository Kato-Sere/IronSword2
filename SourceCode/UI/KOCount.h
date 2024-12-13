#ifndef KO_COUNT_H_
#define KO_COUNT_H_

#include <GStype.h>

// 撃破数管理クラス
class KOCount {
public:
    // コンストラクタ
    KOCount();
    // デストラクタ
    ~KOCount();

    // 描画
    void draw() const;

    // 撃破数の加算
    void add();
    // 撃破数の取得
    int get() const;
    // 撃破数のクリア
    void clear();

private:
    // 撃破数
    int ko_count_{ 0 };
};

#endif // !KO_COUNT_H_
