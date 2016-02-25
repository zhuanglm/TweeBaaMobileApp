#pragma once

//#include "memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
//#include "GameScene.h"
#include "time.h"
#include "cocos2d.h"
//#include "BirdSprite.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "Stage.h"
#include "cocos-ext.h"
#include "../ZhuanPanSystem/SpinLucky.h"


using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::network;

USING_NS_CC;
USING_NS_CC_EXT;

/*
enum Table
{
	Table_Left = 0,
	Table_Center,
	Table_Right
};
*/

class SpinLayer : public Layer{
public:
	static cocos2d::Scene* createScene();
	SpinLayer();
	~SpinLayer(void);
	virtual bool init();

	// implement the "static create()" method manually
	//CREATE_FUNC(SpinLayer);
    

};