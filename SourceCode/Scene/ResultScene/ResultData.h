#ifndef RESULT_DATA_H_
#define RESULT_DATA_H_

#include <gslib.h>
#include <string>
#include <vector>

// リザルト用データ管理クラス
class ResultData {
public:
    // 初期化
    void clear();

    // 今回のパラメーターの取得
    void get_result_param(float time, int ko, int damage);
    // 既存データの読み込み
    void load();
    // ランキングの更新
    void update_ranking();
    // データの保存
    void save();

    // 戦闘時間の取得(string版)
    std::string battle_time() const;
    // 今回の撃破数の取得
    int ko_kount() const;
    // 被ダメージ量を取得
    int damage() const;
    // 表示用撃破数の取得
    const std::vector<int>& ranking() const;
    // 表示する最低順位を取得
    int show_worst_rank() const;

private:
    // 今回の撃破数よりランキングの撃破数の方が多いか？
    bool is_more_ranking_count(int count);

private:
    // 戦闘時間
    float battle_time_{ 0.0f };
    // 今回の撃破数
    int ko_count_{ 0 };
    // 被ダメージ量
    int damage_{ 0 };

    // 表示用撃破数
    std::vector<int> ranking_kocounts_;
    // ランキングのファイル名
    std::string ranking_file_{ "Assets/Save/KOCountRanking.txt" };
    // 表示(保存)する最低順位
    int show_worst_rank_{ 5 };
};

#endif // !RESULT_DATA_H_
