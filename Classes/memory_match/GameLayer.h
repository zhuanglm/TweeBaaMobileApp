#pragma once

#include "cocos2d.h"
#include "global_const.h"
#include "TouchSprite.hpp"
//#include "WelcomeScene.h"
//#include "TweebaaMobileLayer.h"
#include "AppDelegate.h"
#include "GameFailedScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
//#include "SimpleAudioEngine.h"
//#include <cstdlib>
#include "ui/CocosGUI.h"
#include "../utilits/Tutorial.h"
#include "../Msgbox/Dialogbox.h"
#include "../Msgbox/MsgBox.h"


using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

#define min(X,Y) ((X) < (Y) ? (X) : (Y)) 
#define max(X,Y) ((X) > (Y) ? (X) : (Y)) 
#define IMAGE_COUNT 19
#define MUST_HAVE 4
#define IMAGE_PATH	"res/Level_Image/"
const char sGame[IMAGE_COUNT][100] = {
	"1_fl_zo_sh_li_podillow_image.jpg",
	"2_fl_zo_sh_li_wondercap_image.jpg",
	"3_fl_zo_sh_li_tweebot_imagec.jpg",
	"4_spot_level7_kittylitty_imagea.jpg",
	"18_fl_sh_zo_li_headlamp_imagea.jpg",
	"24_fl_sh_zo_li_vidaliachop_image.jpg",
	"25_fl_li_dunbar_image.jpg",
	"26_fl_sh_zo_li_lappak_image.jpg",
	"27_fl_sh_zo_li_lappak_image.jpg",
	"28_fl_li_scroopngo_image.jpg",
	"29_fl_li_extraseat_image.jpg",
	"30_fl_sh_zo_li_spirooli_image.jpg",
	"31_fl_sh_zo_li_vacngroom_image.jpg",
	"32_fl_sh_zo_li_dapxhorse_image.jpg",
	"33_fl_sh_zo_li_sobokawa_image.jpg",
	"34_fl_sh_li_dirtydog_image.jpg",
	"35_fl_sh_li_sipndip_image.jpg",
	"36_fl_zo_li_ezjumper_image.jpg",
	"37_fl_zo_li_squeeze_image.jpg"
};

const int MaxFind = 6;


class GameLayer : public Layer{
public:
	GameLayer();
	~GameLayer();
	bool virtual init();
	CREATE_FUNC(GameLayer);
	cocos2d::Node* _rootNode;
	void updateLabels(float dt);
	void updateTimeLabel(float dt);
	void GameFailed();
	void menuCallback(Ref *sender);
	bool _bTlEnabled;
	void showDeductPointEffect(std::string sLabel);
	void enableTools(float dt);
private:
	void randomArray(int a[], int n);
	void PlayAnimation(float dt);

private:
	int _nTlcount; //totol tips can be used
	int _nTlimit; //tips can be used in this level
	int _nLvl; //Lvl
	float _fPerToNLvl; //Percent to next level
	long _lTtlScore;
	void touchButton(Object* obj, ::ui::Widget::TouchEventType eventType);
	static void CheckWin();//called in lambda, has to be static
	void UseTips();
	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void loadFirstTutorial(float dt);

};