#ifndef LINE_H_
#define LINE_H_

#include<gslib.h>

// 線分クラス 授業教材使用
class Line {
public:
	// コンストラクタ
	Line() = default;
	Line(const GSvector3& start, const GSvector3& end) : start_{ start }, end_{ end } {}

    // デバッグ用描画
    void draw() {
        glBegin(GL_LINES);
        glVertex3f(start_.x, start_.y, start_.z);
        glVertex3f(end_.x, end_.y, end_.z);
        glEnd();
    }

public:
	// 始点
	GSvector3 start_{ 0.0f,0.0f,0.0f };
	// 終点
	GSvector3 end_{ 0.0f,0.0f,0.0f };
};

#endif // !LINE_H_
