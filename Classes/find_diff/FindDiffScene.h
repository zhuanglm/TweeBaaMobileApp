#pragma once
//#include "../AtlasLoader.h"
#include "FindDiffLayer.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
//#include "../memory_match/memory_match_AtlasLoader.h"

using namespace cocos2d;
using namespace std;

class FindDiffScene : public Scene{
public:
	FindDiffScene(void);
	~FindDiffScene(void);
	bool virtual init();
	CREATE_FUNC(FindDiffScene);

	void loadingCallBack(Texture2D *texture);
};