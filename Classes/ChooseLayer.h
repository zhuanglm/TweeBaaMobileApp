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
#include "global_const.h"
#include "Stage.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::network;

const char sAni[GAMG_COUNT][100] = { "res/animations/Spot/2.1_animation",
									"res/animations/Shuffle/2.3_animation",
									"res/animations/Flip/2.4_animation",
									"res/animations/Match/2.5_animation",
									"res/animations/Zoom/2.2_animation",
									"res/animations/Link/2.6_animation" };

const char sAniName[GAMG_COUNT][20] = { "2.1_animation","2.3_animation", "2.4_animation", "2.5_animation", "2.2_animation", "2.6_animation" };

const char sIdle[GAMG_COUNT][100] = { "res/animations/Spot/2.1_icon",
										"res/animations/Shuffle/2.3_icon",
										"res/animations/Flip/2.4_icon",
										"res/animations/Match/2.5_icon",
										"res/animations/Zoom/2.2_icon",
										"res/animations/Link/2.6_icon" };

const char sIdleName[GAMG_COUNT][20] = { "2.1_icon", "2.3_icon", "2.4_icon", "2.5_icon", "2.2_icon", "2.6_icon" };

class ChooseLayer : public Layer
{
public:
	static cocos2d::Scene* createScene();
	//ChooseLayer(void): Stage() {};
	~ChooseLayer(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(ChooseLayer);
	void SyncData();
	static void clearLocalFlow();
	std::string ReadLocalFlow(); 
	void onSyncComplete(HttpClient *sender, HttpResponse *response);
    
private:
	void touchButton(Ref* obj, ::ui::Widget::TouchEventType eventType);
	void updateLabel(float dt);
	//void checkNetwork();

	//void yesCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
	//void noCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);

	//void menuCallbackPopupNo(Ref *sender);
	//void menuCallbackPopupYes(Ref *sender);
	//void menuCallbackPopupNo();
	//void menuCallbackPopupYes();
	//void menuCallbackPopupOk();

	//void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	//static size_t writeData(uint8_t* ptr,size_t size,size_t nmemb,void *stream); 

	cocos2d::Node* _rootNode;
	cocos2d::Node* _popNode;
	void ClaimLocalReward(); 
	void PlayAnimation(float dt);
	void PlayAnimation1(float dt);
	std::string Convert(std::string& str);
	///test ----------- should remove it
	/*
	void onHttpSpinRequestCompleted(HttpClient *sender, HttpResponse *response);
	void onHttpSaveRequestCompleted(HttpClient *sender, HttpResponse *response);
	std::string Convert(std::string& str);
	*/
protected:
    virtual void initialize();
};