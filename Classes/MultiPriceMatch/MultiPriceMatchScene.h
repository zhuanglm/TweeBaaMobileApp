#pragma once
//#include "../AtlasLoader.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
//#include "../memory_match/memory_match_AtlasLoader.h"

using namespace cocos2d;
using namespace std;

class MultiPriceMatchScene : public Scene{
public:
	MultiPriceMatchScene(void);
	~MultiPriceMatchScene(void);
	bool virtual init();
	CREATE_FUNC(MultiPriceMatchScene);

	void loadingCallBack(Texture2D *texture);
};