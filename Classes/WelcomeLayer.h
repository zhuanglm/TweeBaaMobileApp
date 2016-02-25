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

//const int START_BUTTON_TAG = 100;
typedef enum
{
	SCROLLVIEW_EVENT_SCROLL_TO_TOP,
	SCROLLVIEW_EVENT_SCROLL_TO_BOTTOM,
	SCROLLVIEW_EVENT_SCROLL_TO_LEFT,
	SCROLLVIEW_EVENT_SCROLL_TO_RIGHT,
	SCROLLVIEW_EVENT_SCROLLING,
	SCROLLVIEW_EVENT_BOUNCE_TOP,
	SCROLLVIEW_EVENT_BOUNCE_BOTTOM,
	SCROLLVIEW_EVENT_BOUNCE_LEFT,
	SCROLLVIEW_EVENT_BOUNCE_RIGHT
}ScrollEventType;

class WelcomeLayer : public Layer{
public:
	WelcomeLayer(void);
	~WelcomeLayer(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(WelcomeLayer);
    
private:
	/**
     * The start button has been pressed will call this function
     */
	void onUIScrollViewEvent(Ref* sender, ScrollEventType e);
	cocos2d::Node* _rootNode;
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType) ;
	void setAnimationVisible( int i);
	void Bounce(Ref* sender, int nCount);
	void turnEarth(int iOffset);
	bool loadGameWithTutorial(int nGameId);
	void detectMove(float dt);
	float original_x;
};
using namespace ui;
class PageList :public ListView
{
public:
	void bounce(const Vec2& des, float second, bool attenuated)
	{
		startAutoScrollChildrenWithDestination(des,second,attenuated);
	};
};

