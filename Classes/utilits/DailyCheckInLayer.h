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

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::network;


class DailyCheckInLayer : public cocos2d::Sprite
{
public:
	//static cocos2d::Scene* createScene();
	//DailyCheckInLayer(void): Stage() {};
	DailyCheckInLayer(Layer* layer);
	~DailyCheckInLayer(void);
	cocos2d::Node* _rootNode;
	//virtual bool init();
	// implement the "static create()" method manually
	
	//CREATE_FUNC(DailyCheckInLayer);
    
private:
	void menuCallbackPopupNo();
	void menuCallbackYes(Ref *sender);
	void menuCallbackNo(Ref *sender);
	void onCheckinCompleted(HttpClient *sender, HttpResponse *response);
	int dayofweek(int d, int m, int y);
	void ShowMsgBox(std::string title, std::string msg);


	///test ----------- should remove it
	/*
	void onHttpSpinRequestCompleted(HttpClient *sender, HttpResponse *response);
	void onHttpSaveRequestCompleted(HttpClient *sender, HttpResponse *response);
	std::string Convert(std::string& str);
	*/
protected:
    //virtual void initialize();
};