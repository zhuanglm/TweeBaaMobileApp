#pragma once
#ifndef  _TUTORIAL_H_
#define  _TUTORIAL_H_
//#include "../AtlasLoader.h"
//#include "FindDiffLayer.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "../memory_match/memory_match_AtlasLoader.h"

using namespace cocos2d;
using namespace std;


//#define SPOT_TUTORIAL_PLIST "/res/Games_Tutorial/SPOT/SPOT_tutorial.plist"
//#define SPOT_TUTORIAL_IMAGE "/res/Games_Tutorial/SPOT/Spot_tutorial.jpg"
//#define SPOT_FRAME "SPOT_step%d.jpg"

#define TIPS2WIN_TUTORIAL_IMAGE "res/Games_Tutorial/TipsToWin/Tips_step1.jpg"
#define TIPS2WIN_FRAME "res/Games_Tutorial/TipsToWin/Tips_step%d.jpg"

#define COLLAGE_TUTORIAL_PLIST "res/Games_Tutorial/Collage/Collage_Tutorial.plist"
#define COLLAGE_TUTORIAL_IMAGE "res/Games_Tutorial/Collage/Collage_Tutorial.jpg"
#define COLLAGE_FRAME "res/Games_Tutorial/Collage/Collage_Tutorial_Step%d.jpg"

#define SPOT_TUTORIAL_PLIST "res/Games_Tutorial/SPOT/SPOT_tutorial.plist"
#define SPOT_TUTORIAL_IMAGE "res/Games_Tutorial/SPOT/SPOT_tutorial.jpg"
#define SPOT_FRAME "res/Games_Tutorial/SPOT/SPOT_step%d.jpg"

#define SPOT_TUTORIAL_PRE_PLIST "res/Games_Tutorial/SPOT/SPOT_prestep.plist"
#define SPOT_TUTORIAL_PRE_IMAGE "res/Games_Tutorial/SPOT/SPOT_prestep.jpg"
#define SPOT_PRE_FRAME "res/Games_Tutorial/SPOT/Pre/SPOT_prestep%d.jpg"


#define ZOOM_TUTORIAL_PLIST "res/Games_Tutorial/ZOOM/ZOOM_tutorial.plist"
#define ZOOM_TUTORIAL_IMAGE "res/Games_Tutorial/ZOOM/ZOOM_tutorial.jpg"
#define ZOOM_FRAME "res/Games_Tutorial/ZOOM/ZOOM_step%d.jpg"

#define FLIP_TUTORIAL_PLIST "res/Games_Tutorial/FLIP/FLIP_tutorial.plist"
#define FLIP_TUTORIAL_IMAGE "res/Games_Tutorial/FLIP/FLIP_tutorial.jpg"
#define FLIP_FRAME "res/Games_Tutorial/FLIP/FLIP_step%d.jpg"

#define LINK_TUTORIAL_PLIST "res/Games_Tutorial/LINK/LINK_tutorial.plist"
#define LINK_TUTORIAL_IMAGE "res/Games_Tutorial/LINK/LINK_tutorial.jpg"
#define LINK_FRAME "res/Games_Tutorial/LINK/LINK_step%d.jpg"

#define MATCH_TUTORIAL_PLIST "res/Games_Tutorial/MATCH/MATCH_tutorial.plist"
#define MATCH_TUTORIAL_IMAGE "res/Games_Tutorial/MATCH/MATCH_tutorial.jpg"
#define MATCH_FRAME "res/Games_Tutorial/MATCH/MATCH_step%d.jpg"

#define SHUFFLE_TUTORIAL_PLIST "res/Games_Tutorial/SHUFFLE/SHUFFLE_tutorial.plist"
#define SHUFFLE_TUTORIAL_IMAGE "res/Games_Tutorial/SHUFFLE/SHUFFLE_tutorial.jpg"
#define SHUFFLE_FRAME "res/Games_Tutorial/SHUFFLE/SHUFFLE_step%d.jpg"


#define THEME_FRAME "res/Games_Tutorial/Story/Theme_step%d.jpg"

#define BUTTON_FONTSIZE 40

class TutorialScene : public Scene{
public:
	int _ttl_time;
	int _nTutorial;
	int _nPLayPre;
	static int _nIsIngame;

	cocos2d::Node* _UINode;
	TutorialScene(void);
	~TutorialScene(void);
	bool virtual init();
	CREATE_FUNC(TutorialScene);

	void loadingCallBack(Texture2D *texture);
	void btnQuit(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void btnContinue(Ref* pSender, ui::Widget::TouchEventType eEventType);
	void update(float dt);
	void enableBtn(float dt);

private:
	int _nSteps;
	int _nCurrentStep;

};


#endif //_TUTORIAL_H_