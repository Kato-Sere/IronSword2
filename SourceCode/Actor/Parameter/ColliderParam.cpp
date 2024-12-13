#include "ColliderParam.h"

// コンストラクタ
ColliderParam::ColliderParam(float height, float radius) {
    height_ = height;
    radius_ = radius;
}

// 高さの取得
float ColliderParam::height() const {
    return height_;
}

// 半径の取得
float ColliderParam::radius() const {
    return radius_;
}
