#ifndef PREPARATION_ASSETS_H_
#define PREPARATION_ASSETS_H_

#include "CSVReader.h"
#include "World/World.h"

#include <mutex>

// 使用アセットの準備を行うクラス
class PreparationAssets {
    // オブジェクトenum
    enum ObjEnum {
        Obj_WayPoint,     // ウェイポイント(等間隔以外に個別で設置する場合使用)
        Obj_WoodBox,      // 障害物・木箱
        Obj_EnemySpawner, // 敵出現ポイント
        Obj_ItemPot,      // アイテム入り壺
    };

public:
    // 準備実行
    void Run(IWorld* world);
    // 使用リソースの準備
    void prepare_resource(IWorld* world);

    // 使用アセットの読み込み
    void load_assets();
    // アクターの追加
    void add_actors(IWorld* world);
    // オブジェクトの追加
    void add_objects(IWorld* world);
    // デバッグ用アクターの追加
    void debug_add_actors(IWorld* world);
    // 準備終了フラグの設定
    void set_prepare_end(bool frag);
    // 準備終了フラグの取得
    bool get_prepare_end();
    // 準備進捗パラメーターの取得
    const int get_prepared_count() const;

private:
    // ステージデータ読み込み用
    CSVReader csv_;
    // 準備進捗パラメーター
    int prepared_count_{ 0 };
    // 準備終了フラグ
    bool is_prepare_end_{ false };
    // フラグロック用
    std::mutex mutex_;
};

#endif // !PREPARATION_ASSETS_H_
