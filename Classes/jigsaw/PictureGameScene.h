#pragma once
//#include "AtlasLoader.h"
#include "SlidingMenuLayer.h"
#include "PictureGameLayer.h"

//#include "BackgroundLayer.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class PictureGameScene : public Scene{
public:
	//std::string jigSawFile;
	PictureGameScene(void);
	~PictureGameScene(void);
	bool virtual init();
	CREATE_FUNC(PictureGameScene);
};