#pragma once
#ifndef _TurntableSystem_
#define _TurntableSystem_
#include <string>
//#include <stdio>
#include "cocos2d.h"
#include "global_const.h"
#include "AppDelegate.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "../VisibleRect.h"
#include "../Msgbox/Dialogbox.h"
#include "../Msgbox/MsgBox.h"
#include "../utilits/base64.h"
#include "../utilits/xxtea.h"
#include "../utilits/CCMD5.h"
#define TUENTABLE_BACKGROUND "res/public/bg_2.png"

using namespace cocos2d;
using namespace cocos2d::network;
using namespace ui;

USING_NS_CC;
#define LOG_IN 0
#define OFF_LINE 1
#define NOT_LOG_IN 2

class TurntableSystem : public Scene
{
public:

	TurntableSystem();
	~TurntableSystem(void);
	bool virtual init();
	CREATE_FUNC(TurntableSystem);

	int StopNum;
	int startnumber;

	int runnum;
	void LoadTurntableSystem();
	void runTurntableGet(float time);

	void runAct(float time);
	//void ShowPriceAndBack2WinScene(float time);
	void CallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void CallRegister(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	int getReward(int id);

	void changeBox(int boxid,bool change);
	Node* getChildByTagRecursively(int nodeTag,cocos2d::Node* parent);
	void runPlay(Ref* pSender);
	void ShowSpinLucky(Ref* pSender);

	void onHttpSaveRequestCompleted(HttpClient *sender, HttpResponse *response);

	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	void onHttpRequestGetDailyCheckInCompleted(HttpClient *sender, HttpResponse *response);

private:

	std::string UpdateReward(std::string sLocalReward);
	//std::string Decrypt(std::string strSource);
	//std::string Encrypt(std::string strSource);
	void ShowMessage(int iMode);
	void ShowMsgBox(std::string title, std::string msg);
	void SaveReward2Local(int iMode = OFF_LINE);
	std::string Convert(std::string& str);
};

#endif