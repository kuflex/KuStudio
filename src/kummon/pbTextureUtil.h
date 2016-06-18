#pragma once

#include "ofMain.h"

class pbTextureUtil
{
public:
	//Вспомогательная функция для рендеринга квадов
	//Точки должны идти по 4x2 на квад.
	//Не ставит цвет!
	//если 3d - то pointP - 4x3 на квад
	static void drawQuads( vector<float> &pointP, vector<float> &textureP, ofTexture &tex, bool _3d = false );

	static void drawQuads( float *pointP, float *textureP, int nQuads, ofTexture &tex, bool _3d = false);


};
