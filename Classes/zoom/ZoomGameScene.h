#pragma once
//#include "../AtlasLoader.h"
#include "ZoomPhotoLayer.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
//#include "../memory_match/memory_match_AtlasLoader.h"

using namespace cocos2d;
using namespace std;

class ZoomGameScene : public Scene{
public:
	ZoomGameScene(void);
	~ZoomGameScene(void);
	bool virtual init();
	CREATE_FUNC(ZoomGameScene);

	void loadingCallBack(Texture2D *texture);
};