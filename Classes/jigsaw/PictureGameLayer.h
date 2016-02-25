#pragma once

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

#include "network/HttpClient.h"

#include "CCNodeExt.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::network;

static  int STATUS_LABEL_TAG = 20;

static  int MOVES_LABEL_TAG = 22;
static  int TILE_NODE_TAG = 23;
 

static  int PAUSE_OVERLAY_TAG = 25;
static bool gameover = false ;

static Vec2 emptyPosition  ;

static int _szSolvable9[16][9] = {	{ 5, 1, 2, 8, 7, 6, 0, 4, 3 },{ 8, 5, 1, 0, 7, 2, 4, 3, 6 },{ 0, 8, 5, 4, 7, 1, 3, 6, 2 },{ 4, 0, 8, 3, 7, 5, 6, 2, 1 },
									{ 3, 4, 0, 6, 7, 8, 2, 1, 5 },{ 6, 3, 4, 2, 7, 0, 1, 5, 8 },{ 2, 6, 3, 1, 7, 4, 5, 8, 0 },{ 1, 2, 6, 5, 7, 3, 8, 0, 4 },
									{ 0, 5, 1, 8, 6, 2, 4, 7, 3 },{ 8, 0, 5, 4, 6, 1, 7, 3, 2 },{ 4, 8, 0, 7, 6, 5, 3, 2, 1 },{ 7, 4, 8, 3, 6, 0, 2, 1, 5 },
									{ 3, 7, 4, 2, 6, 8, 1, 5, 0 },{ 2, 3, 7, 1, 6, 4, 5, 0, 8 },{ 1, 2, 3, 5, 6, 7, 0, 8, 4 },{ 5, 1, 2, 0, 6, 3, 8, 4, 7 }
								};

#define MAX_LEVEL_JIGSAW 23
#define IMAGE_PATH	"res/Level_Image/"
const char sJigsaw[MAX_LEVEL_JIGSAW][100] = {	"1_FL_ZO_SH_LI_PODillow_image.jpg",
												"2_FL_ZO_SH_LI_WonderCap_image.jpg",
												"3_FL_ZO_SH_LI_Tweebot_imageC.jpg",
												"4_SPOT_level7_Kittylitty_imageA.jpg",
												"5_FL_SH_MA_LI_Vase1_imageB.jpg",
												"6_FL_SH_LI_Carpet2_imageC.jpg",
												"7_FL_SH_LI_CeramicJar_image.jpg",
												"13_FL_SH_LI_OrangeSparrow_imageB.jpg",
												"18_FL_SH_ZO_LI_Headlamp_imageA.jpg",
												"19_FL_SH_ZO_LI_Light_imageC.jpg",
												"24_FL_SH_ZO_LI_VidaliaChop_image.jpg",
												"26_FL_SH_ZO_LI_Lappak_image.jpg",
												"27_FL_SH_ZO_LI_Lappak_image.jpg",
												"28_FL_LI_ScroopNGo_image.jpg",
												"30_FL_SH_ZO_LI_Spirooli_image.jpg",
												"31_FL_SH_ZO_LI_VacNGroom_image.jpg",
												"32_FL_SH_ZO_LI_DAPXhorse_image.jpg",
												"33_FL_SH_ZO_LI_Sobokawa_image.jpg",
												"34_FL_SH_LI_DirtyDog_image.jpg",
												"35_FL_SH_LI_SipNDip_image.jpg",
												"36_FL_ZO_LI_EZJumper_image.jpg",
												"37_FL_ZO_LI_Squeeze_image.jpg",
												"FL_ZO_LI_CubeMirror_imageC.jpg"
};

#define _XBlank 15
#define _YBlank 22

 class PictureGameLayer :public Layer {

public:
	int topleft;
	int toppoint ;
	Size screenSize;
	int BlockMoved;
	int moves;
	float generalscalefactor;
	PictureGameLayer(void);
	~PictureGameLayer(void);
	virtual bool init();
	CREATE_FUNC(PictureGameLayer);

	void updateTimeLabel(float dt);
	void generateTiles();

	bool _bTlEnabled;
	void showDeductPointEffect(std::string sLabel);
	void enableTools(float dt);
	
	//virtual void onTouchesBegan(const vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const vector<Touch*>& touches, Event* event);
	//virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);


	void slideCallback(CCNodeExt* thenode);

	void slideTile(int direction, CCNodeExt* thenode, bool move);


	void GameFailed();

	//This method checks if the puzzle has been correctly solved.
	bool checkCorrect();

	cocos2d::Node* _rootNode;
	void touchButton(Ref* obj, ::ui::Widget::TouchEventType eventType); 
	void PlayAnimation(float dt);
	//void WinCallback(Ref *sender);

	//void backCallback(Ref *sender);
 private:
	void UpdateBlockMoveFlag(float dt);
	void GameWinJump(float dt);
	//void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	//void menuCallback(Ref *sender);
	//void menuvoice_Callback(Ref *sender);
	//void menumobile_Callback(Ref *sender);
	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateLabels(float dt);
	void loadFirstTutorial(float dt);

	int _nTlcount; //totol tips can be used
	int _nTlimit; //tips can be used in this level
	int _nLvl; //Lvl
	float _fPerToNLvl; //Percent to next level
	long _lTtlScore;


};

