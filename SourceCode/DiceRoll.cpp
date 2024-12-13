#include "DiceRoll.h"

// 1d100
bool DiceRoll::roll_1d100(float difficulty_level) {
    return (std::rand() % 100 + 1) < difficulty_level;
}

// n面ダイスを1回
bool DiceRoll::roll_1dn(int dice, float difficulty_level) {
    return (std::rand() % dice + 1) < difficulty_level;
}
