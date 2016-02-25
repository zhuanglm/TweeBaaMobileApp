#pragma once

//#include "memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
//#include "GameScene.h"
#include "time.h"
#include "cocos2d.h"
//#include "BirdSprite.h"
#include "ui/CocosGUI.h"
#define STAR "res/theme/theme-star.png"
#define WOOD "res/theme/wood_level.png"
#define HOLO "res/theme/theme-star-d.png"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

//const int START_BUTTON_TAG = 100;

class ThemeLayer : public Layer{
public:
	static cocos2d::Scene* createScene();
	ThemeLayer(void);
	~ThemeLayer(void);
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(ThemeLayer);
    
private:
	cocos2d::Node* _rootNode;

    

};