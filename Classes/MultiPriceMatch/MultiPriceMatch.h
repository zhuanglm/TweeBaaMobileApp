#pragma once

//#include "../memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
#include "time.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "WelcomeScene.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "GameFailedScene.h"
#include "global_const.h"
#include "MultiPriceMatchScene.h"

//#include "base\CCVector.h"

//#include "BirdSprite.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace ui;

#define GAME_OVER 1040
#define GAME_START 1041
#define GAME_PAUSE 1042

#define PRODUCT_IMG_1 1043
#define PRODUCT_IMG_2 1044
#define PRODUCT_IMG_3 1045
#define LBL_PRICE_1 1046
#define LBL_PRICE_2 1047
#define LBL_PRICE_3 1048
#define BTN_PRICE_1 1049
#define BTN_PRICE_2 1050
#define BTN_PRICE_3 1051
#define SORT_UI_NODE 1052
#define SORT_UI_PRODUCTPAGE 1053

#define LINK_TAG_EFFECT_MINUSPOINTS 1054

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
#define BUTTON_MAIN "button_Main"
#define BUTTON_SETTING "btnSetting"
#define BUTTON_VOICE "btnVoice"
#define BUTTON_QUIT "btnTips"
#define BUTTON_TIPS "btnTips"
#define LBL_SP_PDX "sp_pdX"
#define IMG_PD_PRICE "img_pdX_price"
#define BTN_PD_PRICE "btn_pdX_price"
#define TXT_TIMER "lblTimer"
#define BTN_CLEAR "btn_clear"
#define BTN_SUBMIT "btn_submit"
#define LBL_TAKEN "taken"

#define LINK_UI_BTN_TIPS "btnTipsTools"
#define LINK_UI_TEXT_TIPS "txtTips"
#define LINK_UI_TEXT_TIMER "lblTimer"
#define LINK_UI_LEVELBAR "lv_bar_4"
#define LINK_UI_TEXT_LEVELBAR "txtLvl"
#define LINK_UI_TEXT_SCORE "txtScore"

#define LINK_PRODUCT_CSB "price_the_pic/price_the_pic2.csb"
#define LINK_PRODUCT_IMG "Image_2"
#define LINK_PRODUCT_NAME "Text_1"
#define LINK_PRODUCT_DESCRIPTION "Text_19"
#define LINK_PRODUCT_BTN "Quit"




#define PD_ID "ID"
#define PD_NAME "NAME"
#define PD_IMAGE "IMAGE"
#define PD_PRICE "PRICE"
#define PD_DESCRIPTION "DESCRIPTION"
#define PD_SKU "SKU"


#define SORT_LEVEL_INFO "price_the_pic/LevelInfo/levels_sp.plist"
#define SORT_PRODUCT_INFO "price_the_pic/LevelInfo/link_product_info.plist"
#define SORT_UINODE_CSB "price_the_pic/price_the_pic.csb"
#define LINK_MAX_LEVEL 3


#define ANIMATION_FILE "res/animations/1.6/1.6.ExportJson"
#define ARMATURE "1.6"
#define BOARD "Sprite_1"


//#define GAME_INFO_FILE games_lance/levels_fd.plist


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

static std::vector<Vec2> _diffPositions(10); // differences
static Vector<Sprite*>  _vec_circle(5); //differences sprites in 1st image
static Vector<Sprite*>  _vec_circle2(5); //differences sprites in 2nd image

typedef struct PRODUCT_INFO
{
	std::string id;
	std::string name;
	std::string image;
	std::string price;
	std::string description;
	std::string sku;
};

typedef struct LEVEL_INFO
{
	int timelimit;
	String level;
	std::vector <PRODUCT_INFO*> productlist;
	int productid[3];
};

typedef struct GAME_INFO
{
	char game_name[32];
	int Levels;
	int current_level;

};


class MultiPriceMatch : public Layer{

public:
	MultiPriceMatch(void);
	~MultiPriceMatch(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(MultiPriceMatch);

	//Sprite * _pPhoto;



private:
	//cocos2d::Label* labelTouchInfo;
	std::vector<PRODUCT_INFO> _product_list;
	LEVEL_INFO _levelinfosp;
	//PRODUCT_INFO  _product_list[3];

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

	Vector <Sprite* > _vec_product;
	//Vector <Sprite* > _vec_lblPrice;
	std::vector <Sprite* > _vec_lblPrice;
	//Vector <Button* > _vec_btnPrice;
	std::vector <Button* > _vec_btnPrice;
	std::vector <float> _vec_Price;
	std::vector <float> _vec_Price_original;
	std::vector <int> _vec_Price_index;
	std::vector <Vec2> _vec_originalPos;
	std::vector <Vec2> _vec_currentPos;

	int _nTlcount; //totol tips can be used
	int _nTlimit; //totol tips can be used
	int _nLvl; //totol tips can be used
	float _fPerToNLvl; //Percent to next level
	long _lTtlScore;
	bool _bTlEnabled;

	int _sel;
	int _drop;
	/**
     * The start button has been pressed will call this function
     */
	void menuStartCallback(Object *sender);
 	/**
     * The start button has been pressed will call this function
     */
	void menuKeyCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void ShowHintCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void ShowProductCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void ShowTutorialCallback(Ref* pSender, ui::Widget::TouchEventType eEventType);

	 /**
     * The start button has been pressed will call this function
     */
	void btnBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnPriceCallback(Ref* pSender, ui::Widget::TouchEventType eEventType); 
	/**
     * The start button has been pressed will call this function
     */
	void btnZoomInCallback(Object *sender);
	void btnZoomOutCallback(Object *sender);


	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void PDQuitCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	/**
     * This method is make the land have a scroll animation
     */
	void updateTimeLabel(float dt);
	void updateprogress();

	void gamestart();
	void gameover();
	void gamepause();
	void gamewin();
	void gamelose();
	void MovetoPriceLabel(Button* button);
	void btnClear(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void btnSubmit(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void test();
	bool showHint();
	//void showCurrency(double dv);
	bool gameresult();

	bool readFile(const char* fileName,const char* levelID);
	bool writeFile(const char* fileName);
	bool loadProductPage(string sName,string sImg,string sDes);

	//void loadLevelInfo();
	void saveLevelInfo();
	void saveProductInfo();
	bool loadProductInfo();
	void loadFirstTutorial(float dt);

	void showDeductPointEffect(std::string sLabel);
	void enableTools(float dt);
	void delayWinQuit(float dt);
	void delayFailQuit(float dt); 
	void PlayAnimation(float dt);
 //   virtual void onEnter();  
 //   //virtual void onExit();  
	//virtual void onEnterTransitionDidFinish();
  
protected:
	cocos2d::Node* _SORT_UINode;
	Point _originalPt;

};