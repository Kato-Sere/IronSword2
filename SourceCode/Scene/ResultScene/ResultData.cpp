#include "Assets.h"
#include "ResultData.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// 初期化
void ResultData::clear() {
    // 今回のデータを格納する変数の初期化
    battle_time_ = 0.0f;
    ko_count_ = 0;
    damage_ = 0;
}

// 今回のパラメーターの取得
void ResultData::get_result_param(float time, int ko, int damage) {
    battle_time_ = time;
    ko_count_ = ko;
    damage_ = damage;
}

// 既存データの読み込み
void ResultData::load() {
    // 今回のパラメーターの読み込み
    std::ifstream thistime{ Current_Result };
    thistime >> battle_time_ >> ko_count_ >> damage_;

    // ランキングの読み込み
    std::ifstream ranking{ ranking_file_ };
    // 格納用の配列を初期化
    ranking_kocounts_.resize(show_worst_rank_, 0);
    // ファイルを開けなかったときは撃破数の格納をしない
    if (!ranking) return;
    for (int& count : ranking_kocounts_) ranking >> count;
}

// ランキングの更新
void ResultData::update_ranking() {
    // 今回の順位を求める
    int thistime_rank_ = 0;
    for (int count : ranking_kocounts_) {
        // 今回より多い撃破数の個数を数える
        if (ko_count_ <= count) thistime_rank_++;
    }

    // ランクインした場合
    if (thistime_rank_ < (int)ranking_kocounts_.size()) {
        // 該当箇所に今回の撃破数を挿入
        ranking_kocounts_.insert(ranking_kocounts_.begin() + thistime_rank_, ko_count_);
        // 最下位スコアの削除
        ranking_kocounts_.pop_back();
    }
}

// データの保存
void ResultData::save() {
    // ファイルを開く
    std::ofstream file;
    file.open(ranking_file_, std::ios::out);
    // 撃破数の保存
    for (int count : ranking_kocounts_) file << count << std::endl;
}

// 戦闘時間の取得(string版)
std::string ResultData::battle_time() const {
    int min = (int)battle_time_ / 60;
    float sec = battle_time_ - min * 60;
    std::stringstream ss;
    ss << min << ':' << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << sec;
    return ss.str();
}

// 今回の撃破数の取得
int ResultData::ko_kount() const {
    return ko_count_;
}

// 被ダメージ量を取得
int ResultData::damage() const {
    return damage_;
}

// 表示用撃破数の取得
const std::vector<int>& ResultData::ranking() const {
    return ranking_kocounts_;
}

// 表示する最低順位を取得
int ResultData::show_worst_rank() const {
    return show_worst_rank_;
}

// 今回の撃破数よりランキングの撃破数の方が多いか？
bool ResultData::is_more_ranking_count(int count) {
    return ko_count_ <= count;
}
