#ifndef CSV_READER_H_
#define CSV_READER_H_

#include <vector>
#include <string>
#include <bitset>
#include <GStype.h>

using namespace std;

// CSVリーダークラス
class CSVReader {
public:
	// コンストラクタ
	CSVReader() = default;
	CSVReader(const string& file_name);
	// ファイルの読み込み
	void load(const string& file_name);
	// データの取得(文字列で取得)
    const string& get(const GSuint& row, const GSuint& column) const;
	// データの取得(int型に変換して取得)
	int geti(const GSuint& row, const GSuint& column) const;
	// データの取得(float型に変換して取得)
	float getf(const GSuint& row, const GSuint& column) const;
    // データの取得(2進数に変換して取得)
    std::bitset<8> get_bit8(const GSuint& row, const GSuint& column) const;
	// 行数を返す
	int rows() const;
	// 列数を返す
	int columns(int row = 0) const;

private:
	// 1行
	using Row = vector<string>;
	// 行の配列
	using Rows = vector<Row>;
	// CSVデータ
	Rows rows_;
};

#endif // !CSV_READER_H_
