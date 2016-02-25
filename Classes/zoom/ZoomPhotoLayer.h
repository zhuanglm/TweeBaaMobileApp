#pragma once

//#include "../memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
#include "ZoomGameScene.h"
#include "time.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "WelcomeScene.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "GameFailedScene.h"
#include "global_const.h"
//#include "BirdSprite.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace ui;

//const int START_BUTTON_TAG = 100;
//
#define LETTER_UNDERSCORE 95

#define BUTTON_NUMBER 1000
#define MENU_MAIN 1001
#define MENU_SETTING 1002
#define MENU_VOICE 1003
#define MENU_TIPS 1004

//#define TIMER_LABEL_TAG 9000
#define ANSWER_LABEL_TAG 1005
#define PHOTO_MIN_TAG 1006
#define PHOTO_MED_TAG 1007
#define PHOTO_MAX_TAG 1008
#define CLIPPER_TAG 1009

#define MSG_ERR 1010
#define MSG_WARN 1011
#define MSG_INFO 1012

#define ZOOM_TAG_EFFECT_MINUSPOINTS 1013

#define ZOOM_TAG_PREVIEW 1014
#define ZOOM_UI_PREVIEW_CSB "zoom/Preview.csb"

#define GAME_TIME_EASY 60*90
//#define GAME_INFO_FILE "game_info_lance.plist"
#define ZOOM_LEVEL_INFO "zoom/LevelInfo/levels_zm.plist"
#define	ZOOM_DUMMY_IMAGE "zoom/LevelInfo/zoom_dummy_300x300.png"
#define	ZOOM_UINODE_NAME "zoom/Zoom.csb"
#define	ZOOM_ANSWER_LABEL	"zoom/square_brown.png"
#define	ZOOM_LETTER_NORMAL	"zoom/letters/normal/X.png"
#define	ZOOM_LETTER_PRESS	"zoom/letters/dark/X.png"
#define	ZOOM_LETTER_DISABLE	"Default/Button_Disable.png"
#define	ZOOM_GAME_PICTURE "game_pic"
#define	ZOOM_LETTER_BUTTON	"btnLetter_X_Y"
#define	ZOOM_MSG_TITLE	"Tweebaa App"
#define	ZOOM_MSG_LESSLETTER	"Please complete the answer."

#define	BUTTON_MAIN "button_Main"
#define	BUTTON_SETTING "btnSetting"
#define	BUTTON_VOICE "btnVoice"
#define	BUTTON_TIPS "btnTips"
#define	BUTTON_MAGNIFIER "btnMagnifier"
#define	BUTTON_CLEAR "btnClear"
#define	BUTTON_SUBMIT "btnSubmit"
#define	BACK_FRAME "lblInputArea"
#define	LABEL_TIMER "lblTimer"

#define ZOOM_UI_BTN_TIPS "btnTipsTools"
#define ZOOM_UI_TEXT_TIPS "txtTips"
#define ZOOM_UI_TEXT_TIMER "lblTimer"
#define ZOOM_UI_LEVELBAR "lv_bar_4"
#define ZOOM_UI_TEXT_LEVELBAR "txtLvl"
#define ZOOM_UI_TEXT_SCORE "txtScore"




#define MASCOTT "Image_8"
#define ANIMATION_FILE "res/animations/1.5/1.5.ExportJson"
#define ARMATURE "1.5"
#define FRAME "imgframe"

//static Vector<Vec2>  _vec_InputPos(12); //12 Input
//static Vector<Vec2>  _vec_InputPos(12); //12 Input
typedef struct LEVEL_INFO_ZM
{
	int timelimit;
	std::string id;
	std::string image;
	std::string name;
	std::string answer;
	std::string display;
	std::string sku;
};

typedef struct GAME_INFO
{
	std::string game_name;
	int Levels;
	int current_level;

};



class ZoomPhotoLayer : public Layer{

public:
	static int retMsg;
	Sprite * _pPhoto;
	int _nAns;
	int _nCurAns;
	int _nLblLength;
	float _nZmLevel;
	char _arrCurrAns[40];
	char _arrCorrtAns[40];
	char _arrDspAns[40];
	std::string _strCorrAns;
	std::string _strDspAns;
	std::string _strCurrAns;
	std::vector <ui::Button*> _vec_alphabet;
	std::vector<Vec2> _vec_InputPos;
	std::vector<int> _vec_Opt;
	std::vector <Sprite *> _vec_button;
	std::vector <Sprite *> _vec_answer;

	Vec2 _arrPos[12];

	int _gametime;
	int _iHlpCount;
	bool _bzoomin;
	float _fscalefactor;
	Size _displsyArea;
	Point _imgPosition;
	Point _ptAnswer;
	ui::Button* _arr_btn[2][6];
	cocos2d::ui::Text* _gametimer;


	int _nTlcount; //totol tips can be used
	int _nTlimit; //totol tips can be used
	int _nLvl; //totol tips can be used
	float _fPerToNLvl; //Percent to next level
	long _lTtlScore;

	ZoomPhotoLayer(void);
	~ZoomPhotoLayer(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(ZoomPhotoLayer);

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

    virtual void onTouchLetter(Ref* pSender, ui::Widget::TouchEventType eEventType);
	virtual void onTouchZoom(Ref* pSender, ui::Widget::TouchEventType eEventType);
	//CREATE_FUNC(TouchScene);

	void ShowHintCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void loadFirstTutorial(float dt);
	void PlayAnimation(float dt);

	void LoadPreviewLayer();
	void RemovePreviewLayer();

	int _pause;
	int _nMsgType;
	bool _bTlEnabled;
	bool _bIsPreview;
	int	_nPreviewSec;
	//void myVirtualEventMethod();

private:
	//EventListenerCustom *eventrecievercancel;
	//EventListenerCustom *eventrecieverok;

	LEVEL_INFO_ZM _levelInfoZM;
	std::vector<LEVEL_INFO_ZM> _ttlProductlist;
	cocos2d::Label* labelTouchInfo;
	/**
     * The start button has been pressed will call this function
     */
	void menuStartCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
 	/**
     * The start button has been pressed will call this function
     */
	void menuKeyCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);

	 /**
     * The start button has been pressed will call this function
     */
	void btnClear(Ref *pSender, Widget::TouchEventType type);

	 /**
     * The start button has been pressed will call this function
     */
	void btnSubmit(Ref *pSender, Widget::TouchEventType type);

	/**
     * The start button has been pressed will call this function
     */
	void btnZoomCallback(Ref *sender);
	/**
     * The start button has been pressed will call this function
     */
	void btnBack(Ref* pSender, ui::Widget::TouchEventType eEventType);

	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void okCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void updateTimer(float dt);
	void updateProgress();
	void quitgame();
	void gamewin();
	void gamelose();
	bool loadimage();
	bool loadbuttons();
	bool loadCSB();
	bool loadAnswerSprites();
	bool loadGameProduct(int gamelvl);

	//void loadLevelInfo();
	Size getImageSize(const char* filename);
	bool readFile(const char* fileName,const char* levelID);
	bool writeFile(const char* fileName);
	void showMessage();
	bool updatescore(int iflag,int score);
	void setBGMusic();

	void pause();
	void showHint();
	void playAnimation();
	void showDeductPointEffect(std::string sLabel);
	void enableTools(float dt);
	void delayWinQuit(float dt);
	void delayFailQuit(float dt);
protected:
	cocos2d::Node* _UINode;

};