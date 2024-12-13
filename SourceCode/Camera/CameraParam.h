#ifndef CAMERA_PARAM_H_
#define CAMERA_PARAM_H_

#include <gslib.h>

/*メインカメラ*/
const GSvector3 Main_Pos{ 0.0f, 3.18f, -4.8f };   // 出現座標
const GSvector3 Main_At{ 0.0f, 1.93f, 0.0f };     // 注視点

/*プレイヤー特殊技用*/
const GSvector3 PlayerSP_Pos{ 4.0f, 2.0f, 2.0f }; // 出現座標
const GSvector3 PlayerSP_At{ 0.0f, 2.0f, 0.0f };  // 注視点

#endif // !CAMERA_PARAM_H_
