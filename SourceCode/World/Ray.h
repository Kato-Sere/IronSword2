#ifndef RAY_H_
#define RAY_H_

#include<gslib.h>

// レイクラス 授業教材使用
class Ray {
public:
	// コンストラクタ
	Ray() = default;
	// デストラクタ
	Ray(const GSvector3& position, const GSvector3& direction) :
		position_{ position }, direction_{ direction }{}

public:
	// 座標
	GSvector3 position_{ 0.0f,0.0f,0.0f };
	// 方向
	GSvector3 direction_{ 0.0f,0.0f,0.0f };
};

#endif // !RAY_H_
