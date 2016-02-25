#pragma once
#ifndef  _TWB_INTROSCENE_H_
#define  _TWB_INTROSCENE_H_
//#include "../AtlasLoader.h"
//#include "FindDiffLayer.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
//#include "../memory_match/memory_match_AtlasLoader.h"
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;

#define NAME_CSB "res/Media/IntroScene.csb"

class TwbIntroScene : public Scene{
public:
	int _ttl_time;
	cocos2d::Node* _UINode; 
	cocos2d::Node* _rootNode;
	TwbIntroScene(void);
	~TwbIntroScene(void);
	bool virtual init();
	void init2(float dt);
	void DoDailySpin();
	void Sync();
	CREATE_FUNC(TwbIntroScene);
	void aSyncAnimation(); 
	//void loadAnimationComplete(float dt);
	void loadAni(float dt);
	void loadIdle(float dt);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void videoEventCallback(Ref*, cocos2d::experimental::ui::VideoPlayer::EventType);
#endif

	void loadingCallBack(Texture2D *texture);
	void menuCallbackPopupOk();
	void update(float dt);
	void initTheme();
};


#endif //_TWB_INTROSCENE_H_