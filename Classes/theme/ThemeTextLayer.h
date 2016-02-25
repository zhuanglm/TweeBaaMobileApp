#pragma once

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

class ThemeTextLayer : public Layer{
public:
	static cocos2d::Scene* createScene();
	ThemeTextLayer(void);
	~ThemeTextLayer(void);
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);
	void playEffect(float dt);
	virtual bool init();
	void enableBtn(float dt);
	// implement the "static create()" method manually
	CREATE_FUNC(ThemeTextLayer);
    
private:
	cocos2d::Node* _rootNode;
    

};