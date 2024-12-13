#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

// コンストラクタ
CSVReader::CSVReader(const string& file_name) {
	load(file_name);
}

// ファイルの読み込み
void CSVReader::load(const string& file_name) {
	//ファイルオープン
	ifstream file{ file_name };
	if (!file) throw runtime_error("ＣＳＶファイルがオープンできませんでした。");
	rows_.clear();                           // 古いデータを削除
	string line;                             // 改行文字までの1行分の文字列
	while (getline(file, line)) {            // 改行区切りで1行分読み込み
		stringstream ss{ line };             // 1行分データをstringstreamに格納
		string value;                        // 各セルのデータ
		Row row;                             // 1行分のデータ
		while (getline(ss, value, ',')) {    // カンマ区切りで読み込み
			row.push_back(value);            // 各セルのデータを行に追加
		}
		rows_.push_back(row);                // 1行分のデータを追加
	}
}


// データの取得(文字列で取得)
const string& CSVReader::get(const GSuint& row, const GSuint& column) const {
    return rows_[row][column];
}

// データの取得(int型に変換して取得)
int CSVReader::geti(const GSuint& row, const GSuint& column) const {
	return stoi(get(row, column));
}

// データの取得(float型に変換して取得)
float CSVReader::getf(const GSuint& row, const GSuint& column) const {
	return stof(get(row, column));
}

// データの取得(2進数に変換して取得)
std::bitset<8> CSVReader::get_bit8(const GSuint& row, const GSuint& column) const {
    return std::bitset<8>(geti(row, column));
}

// 行数を返す
int CSVReader::rows() const {
	return (int)rows_.size();
}

// 列数を返す
int CSVReader::columns(int row) const {
	return (int)rows_[row].size();
}
