#include "Actor/ActorEnum.h"
#include "Light.h"

// コンストラクタ
Light::Light(IWorld* world) {
	world_ = world;
	name_ = "Light";
	tag_ = Tag_Light;
}

// 描画
void Light::draw() const {
	// ライトマップとリフレクションプローブを使用するときは環境光の明るさは0にする
	const float ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const float specular[4] = { 1.0f, 1.0f, 1.0f,1.0f };
	const float position[4] = { 100.0f, 100.0f, 100.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// シャドウマップ用のライトの位置を設定
	GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);
}
