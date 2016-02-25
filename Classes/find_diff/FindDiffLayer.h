#pragma once

//#include "../memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
#include "FindDiffScene.h"
#include "time.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "WelcomeScene.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "GameFailedScene.h"
#include "global_const.h"

//#include "base\CCVector.h"

//#include "BirdSprite.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace ui;

#define IMAGE_FRAME 999
#define TIMER_LABEL_TAG 1000
#define DIFF_LABEL_TAG 1001
#define LEVEL_LABEL_TAG 1002
#define SUCCESS_TAG 1003
#define ANSWER_LABEL_TAG 1004
#define PHOTO1_MIN_TAG 1005
#define PHOTO1_MED_TAG 1006
#define PHOTO1_MAX_TAG 1007
#define PHOTO2_MIN_TAG 1008
#define PHOTO2_MED_TAG 1009
#define PHOTO2_MAX_TAG 1010

#define FINDDIFF_CLIPPER_1 1011
#define FINDDIFF_CLIPPER_2 1012
#define FINDDIFF_IMG_1 1013
#define FINDDIFF_IMG_2 1014
#define FINDDIFF_STENCIL_1 1015
#define FINDDIFF_STENCIL_2 1016
#define FINDDIFF_ZOOMIN 1017
#define FINDDIFF_ZOOMOUT 1018
#define FINDDIFF_ERROR_ICON 1019


#define FINDDIFF_IMG1_CIR1 1021
#define FINDDIFF_IMG1_CIR2 1022
#define FINDDIFF_IMG1_CIR3 1023
#define FINDDIFF_IMG1_CIR4 1024
#define FINDDIFF_IMG1_CIR5 1025
#define FINDDIFF_IMG2_CIR1 1026
#define FINDDIFF_IMG2_CIR2 1027
#define FINDDIFF_IMG2_CIR3 1028
#define FINDDIFF_IMG2_CIR4 1029
#define FINDDIFF_IMG2_CIR5 1030

#define GAME_OVER 1040
#define GAME_START 1041
#define GAME_PAUSE 1042

#define SPOT_TAG_EFFECT_MINUSPOINTS 1043

//Design for multiresolution 
#define designWidth 960
#define designHeight 640

#define LETTER_NUM_0 48
#define LETTER_NUM_1 49
#define LETTER_NUM_2 50
#define LETTER_NUM_3 51
#define LETTER_NUM_4 52
#define LETTER_NUM_5 53
#define LETTER_NUM_6 54
#define LETTER_NUM_7 55
#define LETTER_NUM_8 56
#define LETTER_NUM_9 57

#define GAME_TIME_EASY 60*90
#define GAME_TIME_MEDM 60*60
#define GAME_TIME_DIFF 60*30
#define ERROR_TIME 60*2

#define MAX_LINE_LEN 128

#define SPOT_LEVEL_INFO "spot_it/LevelInfo/levels_fd.plist"
#define SPOT_UI_CSB "spot_it/spot_it.csb"
#define SPOT_DIFF_CIRCLE "spot_it/fd_circle.png"
#define SOUND_ERROR "res/Media/Computer_Error.mp3"
#define SPOT_ERROR_ICON "sp_error"

#define SPOT_UI_BTN_TIPS "btnTipsTools"
#define SPOT_UI_TEXT_TIPS "txtTips"
#define SPOT_UI_TEXT_TIMER "lblTimer"
#define SPOT_UI_LEVELBAR "lv_bar_4"
#define SPOT_UI_TEXT_LEVELBAR "txtLvl"
#define SPOT_UI_TEXT_SCORE "txtScore"

#define BACK_FRAME "back_frame"
#define LABEL_TIMER "lblTimer"
#define BUTTON_MAIN "btnMain"
#define BUTTON_SETTING "btnSetting"
#define BUTTON_VOICE "btnVoice"
#define BUTTON_QUIT "btnQuit"
#define BUTTON_HOME "button_Main"
#define BUTTON_TIPS "btnTips"
#define BUTTON_MAGNIFIER "btnMagnifier"
#define BG_IMG1 "img1"
#define BG_IMG2 "img2"
#define BUTTON_LIGHT "btnLight_X"
#define ANIMATION_FILE "res/animations/1.2/1.2.ExportJson"
#define MASCOTT "mascott"
#define ARMATURE "1.2"

//#define SPOT_MAX_LEVEL 6

static int _gamestatus;
static int _df_found;
static int _df_total;
static int _frame1_x;
static int _frame1_y;
static int _frame2_x;
static int _frame2_y;
static int _frame_width;
static int _frame_height;
static int _image_width;
static int _image_height;
static	bool _zoomin;
static int _curLevel;

static std::vector<Vec2> _diffPositions(10); // differences
static Vector<Sprite*>  _vec_circle(5); //differences sprites in 1st image
static Vector<Sprite*>  _vec_circle2(5); //differences sprites in 2nd image

typedef struct LEVEL_INFO_FD
{
	long timelimit; // TIMELIMIT
	int id; //ID
	int level; //LEVEL
	std::string name; //NAME
	std::string sku; //NAME
	std::string image1; //IMAGE1
	std::string image2; //IMAGE2
	int diffNum; //DIFFNUM
	std::vector <Vec2> diffList; //DIFFLIST
	std::vector <int> found; 
};

class FindDiffLayer : public Layer{

public:
	FindDiffLayer(void);
	~FindDiffLayer(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(FindDiffLayer);

	//Sprite * _pPhoto;



private:
	//cocos2d::Label* labelTouchInfo;
	bool _bTouchable;
	int _nAns;
	int _nCurAns;
	int _nLblLength;
	//float _nZmLevel;
	char _arrAns[40];
	char _arrCorrectAnswer[40];
	bool _isMoving; //is touching the screen
	bool _isResizing;
	Point _startPt; //start touching point
	Point _endPt; //end touching point
	float _new_x;
	float _new_y;
	float _new_x2;
	float _new_y2;
	Rect _rectFrame1;
	Rect _rectFrame2;
	Rect _rectImg1;
	Rect _rectImg2;

	float _fScaleFactor;
	double _scale;
	Size _imgSize;

	Sprite* _img[2]; 
	Sprite* _arrDiff_1[5]; 
	Sprite* _arrDiff_2[5]; 
	Button* _arrlight[10];
	long _gametime;
	double _zmLevel;

    double _dblDistance;     
    double _dblDeltaX;      
    double _dblDeltaY;      
    double _dblScale;     
	Sprite* _spError;
	int _errtimer;


	int _nTlcount; //totol tips can be used
	int _nTlimit; //totol tips can be used
	int _nLvl; //totol tips can be used
	float _fPerToNLvl; //Percent to next level
	long _lTtlScore;

	bool _bTlEnabled; //enable or disable Tool

	LEVEL_INFO_FD _levelinfofd;
	/**
     * The start button has been pressed will call this function
     */
	void menuStartCallback(Object *sender);
 	/**
     * The start button has been pressed will call this function
     */
	void menuKeyCallback(Object *sender);

	 /**
     * The start button has been pressed will call this function
     */
	void btnClick(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void resetTouchEvent();
	/**
     * The start button has been pressed will call this function
     */
	void btnZoomInCallback(Object *sender);
	void btnZoomOutCallback(Object *sender);

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchZoom(Object* pSender, ui::Widget::TouchEventType eEventType,int nSender);

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

	/**
     * This method is make the land have a scroll animation
     */
	void updateLabels(float dt);
	void updateDragMove(float dt);
	void updateprogress();

	void gamestart();
	void QuitGame();
	void gamepause();
	bool loadimages();
	void gamewin();
	void gamelose();
	void resize(float fScale);
	void showHint();
	void ShowWrongPos(Vec2 ErrPos);
	void ShowHintCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void showTutorial();
	void loadFirstTutorial(float dt);

	void showDeductPointEffect(std::string sLabel);
	void enableTools(float dt);
	void delayWinQuit(float dt);
	void delayFailQuit(float dt); 
	void PlayAnimation(float dt);

    //virtual void onEnter();  
    //virtual void onExit();  
	bool loadLevelInfo();
	void saveLevelInfo();
	Size getImageSize(const char* filename);

	void test();
	void test2();
	void test4(float d);
	bool readFile(const char* fileName,const char* levelID);
	bool writeFile(const char* fileName);

protected:
	cocos2d::Node* _UINode;

};