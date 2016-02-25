#pragma once
//#include "AtlasLoader.h"
#include "PriceMatchLayer.h"
//#include "BackgroundLayer.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class PriceMatchScene : public Scene{
public:
	PriceMatchScene(void);
	~PriceMatchScene(void);
	bool virtual init();
	CREATE_FUNC(PriceMatchScene);
};