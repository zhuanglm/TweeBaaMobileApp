﻿#pragma once

//#include "memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
//#include "GameScene.h"
#include "time.h"
#include "cocos2d.h"
//#include "BirdSprite.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

const int START_BUTTON_TAG = 100;

class TweebaaMobileLayer : public Layer{
public:
	static cocos2d::Scene* createScene();
	TweebaaMobileLayer(void);
	~TweebaaMobileLayer(void);
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(TweebaaMobileLayer);
    
private:
	void menuCallback(Ref *sender);
    
	/**
     * This method is make the land have a scroll animation
     */
	void scrollLand(float dt);
    

};