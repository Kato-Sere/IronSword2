#ifndef DICE_ROLL_H_
#define DICE_ROLL_H_

#include<iostream>
using namespace std;

// m面ダイスをn回ロールするクラス
class DiceRoll {
public:
    // 1d100
    bool roll_1d100(float difficulty_level);
    // n面ダイスを1回
    bool roll_1dn(int dice, float difficulty_level);
};

#endif // !DICE_ROLL_H_
