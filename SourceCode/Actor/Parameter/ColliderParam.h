#ifndef COLLIDER_PARAM_H_
#define COLLIDER_PARAM_H_

// 衝突判定球データ管理クラス
class ColliderParam {
public:
    // コンストラクタ
    ColliderParam(float height, float radius);

    // 高さの取得
    float height() const;
    // 取得
    float radius() const;

private:
    // 衝突判定球の高さ
    float height_{ 0.0f };
    // 衝突判定球の半径
    float radius_{ 0.1f };
};

#endif // !COLLIDER_PARAM_H_
