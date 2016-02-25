#pragma once
#ifndef  _SETTINGSCENE_H_
#define  _SETTINGSCENE_H_
//#include "../AtlasLoader.h"
//#include "FindDiffLayer.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "../memory_match/memory_match_AtlasLoader.h"

using namespace cocos2d;
using namespace std;

#define SETTING_UI_CSB "res/setting/setting.csb"
#define SETTING_TEAM_UI_CSB "res/setting/credits.csb"
#define SETTING_TEAM_NODE_TAG 1991
#define SETTING_TEAM_BUTTON "Button_1"
#define SETTING_BUTTON_CLOSE "close"
#define SETTING_BUTTON_ON "sound_open"
#define SETTING_BUTTON_OFF "sound_close"
#define SETTING_BUTTON_MORE "List"
#define SETTING_BUTTON_TEAM "List_0"
#define SETTING_BUTTON_TERM "List_0_0"
#define SETTING_BUTTON_CLEARCACHE "btnClearCache"
#define SETTING_BACKGROUND "res/public/bg_2.png"
#define SETTING_PATH_DIRECTORY "Cache/"
#define SETTING_CLEAR_CACHE "TERMS OF USE_0"

class SettingScene : public Scene{
public:
	int _ttl_time;
	cocos2d::Node* _UINode;
	SettingScene(void);
	~SettingScene(void);
	bool virtual init();
	CREATE_FUNC(SettingScene);

	void loadingCallBack(Texture2D *texture);
	void btnQuitCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnContinueCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnSoundONCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnSoundOFFCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnTeamCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnMoreCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnTermCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnTeamBackCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnClearCacheCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void updateCallBack(float dt);

	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	int _nSteps;
	int _nCurrentStep;

};


#endif //_SETTINGSCENE_H_