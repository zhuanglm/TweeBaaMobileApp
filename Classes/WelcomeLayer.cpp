#include "WelcomeLayer.h"
#include "memory_match/GameScene.h"
#include "jigsaw/jigSawScene.h"
#include "ChooseLayer.h"
#include "PriceMatch/PriceMatchScene.h"
#include "find_diff/FindDiffScene.h"
#include "zoom/ZoomGameScene.h"
#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "LevelSelect/LevelSelectLayer.h"
#include "MultiPriceMatch/MultiPriceMatchScene.h"
#include "cocostudio/CocoStudio.h"
#include "utilits/Tutorial.h"

#define PANEL_WIDTH 260
#define BLANK_SPACE 100 
#define BLANK_TEXTURE "res/public/bg.png"

const char sButton[GAMG_COUNT][20] = { "btnFind_Diff",
								"btnJigSaw",
								"btnMemoryMatch",
								"btnPriceMatch",
								"btnZoom_Game",
								"btnPriceDrag"};

const char sPanel[GAMG_COUNT][20] = { "panelSpot",
								"panelShuffle",
								"panelFlip",
								"panelMatch",
								"panelZoom",
								"panelLink" };

const char sBoard[GAMG_COUNT][20] = {	"Sprite_1",
										"Sprite_4",
										"Sprite_4_0",
										"Sprite_5",
										"Sprite_3",
										"Sprite6" };

const char sLabel[GAMG_COUNT][20] = {	"lblSpotBeatLevel",
										"lblShuffleBeatLevel",
										"lblFlipBeatLevel",
										"lblMatchBeatLevel",
										"lblZoomBeatLevel",
										"lblLinkBeatLevel" };
using namespace cocos2d;
using namespace ui; 
using namespace cocostudio;

Vector<ui::Button*> sp_vec;//声明一个容器
int iPosX[7];


float fLastOff = 0;

WelcomeLayer::WelcomeLayer(){};
WelcomeLayer::~WelcomeLayer(){};

bool WelcomeLayer::init(){
	if(!Layer::init()){
		return false;
	}
	//get the origin point of the X-Y axis, and the visiable size of the screen
	Size visiableSize = Director::getInstance()->getVisibleSize();
	//Point origin = Director::getInstance()->getVisibleOrigin();

	
	::ui::Layout* layout = ::ui::Layout::create();
	layout->setSize(Size(visiableSize.width, visiableSize.height));
	layout->setBackGroundColorType(::ui::LAYOUT_COLOR_SOLID);
	layout->setBackGroundColor(ccc3(150,150,255));
	layout->setPosition(Vec2(0, 0));
	addChild(layout,-5);

	
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sCSD=app->GetLayoutString();
	
	if ( app->getBGMstatus() == BGMusic_flag::ON){
		app->StartBGMusic(BGM_MAIN);
	}


    _rootNode = CSLoader::createNode("res/game_menu/WelcomeLayer"+sCSD+".csb");

	addChild(_rootNode);

	ListView * listView = static_cast<ListView*>(_rootNode->getChildByName("lvGameNav"));
	//ListView* listView = (ListView*)listView1;
	listView->addEventListenerScrollView(this, cocos2d::ui::SEL_ScrollViewEvent(&WelcomeLayer::onUIScrollViewEvent));
	listView->setTouchEnabled(true);

	
	EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchEnded = CC_CALLBACK_2(ListView::onTouchEnded, listView);
	touchListener->onTouchCancelled = CC_CALLBACK_2(ListView::onTouchCancelled, listView);

	auto panelSpot = dynamic_cast<Layout*>(listView->getChildByName("panelSpot"));
	Button* btnFind_Diff = dynamic_cast<Button*>(panelSpot->getChildByName("btnFind_Diff"));
	if (btnFind_Diff)
	{
		btnFind_Diff->setTag(1001);
		btnFind_Diff->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));
		sp_vec.pushBack(btnFind_Diff);
	}

	auto lblSpotBeatLevel = dynamic_cast<::ui::Text*>(panelSpot->getChildByName("lblSpotBeatLevel"));
	if (lblSpotBeatLevel){
		m_iCurrentGameID = 1;
		int iTotalLevel = app->getTotalPassedLevelInfo();
		char p[20];
		sprintf(p, "%d", iTotalLevel);
		lblSpotBeatLevel->setString(p);

	}

	auto panelShuffle = dynamic_cast<Layout*>(listView->getChildByName("panelShuffle"));
	Button* btnJigSaw = dynamic_cast<Button*>(panelShuffle->getChildByName("btnJigSaw"));
	if (btnJigSaw){
		btnJigSaw->setTag(1003);
		btnJigSaw->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));
		btnJigSaw->setEnabled(false);
		sp_vec.pushBack(btnJigSaw);
	}
	auto lblShuffleBeatLevel = dynamic_cast<::ui::Text*>(panelShuffle->getChildByName("lblShuffleBeatLevel"));
	if (lblShuffleBeatLevel){
		m_iCurrentGameID = 3;
		int iTotalLevel = app->getTotalPassedLevelInfo();
		char p[20];
		sprintf(p, "%d", iTotalLevel);
		lblShuffleBeatLevel->setString(p);
	}

	auto panelFlip = dynamic_cast<Layout*>(listView->getChildByName("panelFlip"));
	Button* btnMemoryMatch = dynamic_cast<Button*>(panelFlip->getChildByName("btnMemoryMatch"));
	if (btnMemoryMatch){
		btnMemoryMatch->setTag(1004);
		btnMemoryMatch->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));
		btnMemoryMatch->setEnabled(false);
		sp_vec.pushBack(btnMemoryMatch);
	}
	auto lblFlipBeatLevel = dynamic_cast<::ui::Text*>(panelFlip->getChildByName("lblFlipBeatLevel"));
	if (lblFlipBeatLevel){
		m_iCurrentGameID = 4;
		int iTotalLevel = app->getTotalPassedLevelInfo();
		char p[20];
		sprintf(p, "%d", iTotalLevel);
		lblFlipBeatLevel->setString(p);
	}

	auto panelMatch = dynamic_cast<Layout*>(listView->getChildByName("panelMatch"));
	Button* btnPriceMatch = dynamic_cast<Button*>(panelMatch->getChildByName("btnPriceMatch"));
	if (btnPriceMatch){
		btnPriceMatch->setTag(1005);
		btnPriceMatch->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));
		btnPriceMatch->setEnabled(false);
		sp_vec.pushBack(btnPriceMatch);
	}
	auto lblMatchBeatLevel = dynamic_cast<::ui::Text*>(panelMatch->getChildByName("lblMatchBeatLevel"));
	if (lblMatchBeatLevel){
		m_iCurrentGameID = 5;
		int iTotalLevel = app->getTotalPassedLevelInfo();
		char p[20];
		sprintf(p, "%d", iTotalLevel);
		lblMatchBeatLevel->setString(p);
	}

	auto panelZoom = dynamic_cast<Layout*>(listView->getChildByName("panelZoom"));
	Button* btnZoom_Game = dynamic_cast<Button*>(panelZoom->getChildByName("btnZoom_Game"));
	if (btnZoom_Game){
		btnZoom_Game->setTag(1002);
		btnZoom_Game->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this)); 
		btnZoom_Game->setEnabled(false);
		sp_vec.pushBack(btnZoom_Game);
	}
	auto lblZoomBeatLevel = dynamic_cast<::ui::Text*>(panelZoom->getChildByName("lblZoomBeatLevel"));
	if (lblZoomBeatLevel){
		m_iCurrentGameID = 2;
		int iTotalLevel = app->getTotalPassedLevelInfo();
		char p[20];
		sprintf(p, "%d", iTotalLevel);
		lblZoomBeatLevel->setString(p);
	}

	auto panelLink = dynamic_cast<Layout*>(listView->getChildByName("panelLink"));
	Button* btnPriceDrag = dynamic_cast<Button*>(panelLink->getChildByName("btnPriceDrag"));
	if (btnPriceDrag){
		btnPriceDrag->setTag(1006); 
		btnPriceDrag->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));
		btnPriceDrag->setEnabled(false);
		sp_vec.pushBack(btnPriceDrag);
	}
	auto lblLinkBeatLevel = dynamic_cast<::ui::Text*>(panelLink->getChildByName("lblLinkBeatLevel"));
	if (lblLinkBeatLevel){
		m_iCurrentGameID = 6;
		int iTotalLevel = app->getTotalPassedLevelInfo();
		char p[20];
		sprintf(p, "%d", iTotalLevel);
		lblLinkBeatLevel->setString(p);
	}

	Button* btnHome = dynamic_cast<Button*>(_rootNode->getChildByName("button_Main"));
	if (btnHome){
		btnHome->setTag(1000);
		btnHome->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));
	}

	original_x = visiableSize.width / 2; //(中心)
	 Button* btnReturn = dynamic_cast<Button*>(_rootNode->getChildByName("btnReturn"));
	 if (btnReturn){
		 btnReturn->setTag(1000);
		 btnReturn->addTouchEventListener(CC_CALLBACK_2(WelcomeLayer::touchButton, this));

	 }
	 
	 auto lblLevel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("txtLvl"));

	 auto lblCoins = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("txtScore"));
	 //auto lblLevel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblLevel"));
	 long l_coins = app->GetUserTotalScore();
	 int i_level = app->GetUserLevel();
	 char p[20];
	 sprintf(p, "%ld", l_coins);
	 string sTotal = p;
	 if (lblCoins) lblCoins->setText(sTotal);
	 sprintf(p, "%d", i_level);
	 sTotal = p;
	 if (lblLevel) lblLevel->setText(sTotal);


	 float _fPerToNLvl; //Percent to next level
	 _fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
	 auto lvlbar = dynamic_cast<Sprite*> (_rootNode->getChildByName("lv_bar_4"));
	 lvlbar->setAnchorPoint(Vec2(0, 0.5));
	 //lvlbar->setPositionX(lvlbar->getPositionX() - lvlbar->getContentSize().width / 2);
	 lvlbar->setScaleX(_fPerToNLvl);


	 //----------------------------------------------------------------------------------------------

	 Layout *panel;
	 Armature *armature1;
	 Armature *armature2;
	 Button *button;
	 int sOffset[6][2] = { { 120, 245 }, { 120, 245 }, { 120, 255 }, { 125, 245 }, { 125, 245 }, { 125, 245 } };

	 for (int i = 0; i < 6; i++)
	 {
		 panel = dynamic_cast<Layout*>(listView->getChildByName(sPanel[i]));
		 armature1 = Armature::create(sAniName[i]);
		 armature2 = Armature::create(sIdleName[i]);
		 armature1->setTag(7000 + i);
		 armature2->setTag(7200 + i);
		 armature1->setPosition(sOffset[i][0], sOffset[i][1]);
		 armature2->setPosition(sOffset[i][0], sOffset[i][1]);
		 panel->addChild(armature1);
		 panel->addChild(armature2);

		 Sprite* board = dynamic_cast<Sprite*>(panel->getChildByName(sBoard[i]));
		 Text* label = dynamic_cast<Text*>(panel->getChildByName(sLabel[i]));
		 button = dynamic_cast<Button*>(panel->getChildByName(sButton[i]));

		 if (i == 0)
		 {
			 armature1->setVisible(true);
			 armature2->setVisible(false);
			 armature1->setScale(1.8);
			 armature1->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
			 board->setPositionY(30);
			 label->setPositionY(30);
			 button->setScale(2.65);
		 }
		 else
		 {
			 armature1->setVisible(false);
			 armature2->setVisible(true);
			 armature2->setScale(1.0);
			 armature2->setColor(ccc3(84,84,84));
			 armature2->getAnimation()->playWithIndex(0, -1, 1);//from the first frame,loop
			 board->setScale(0.6);
			 label->setScale(0.6);
			 board->setPositionY(45);
			 label->setPositionY(45);
			 button->setScale(1.65);
		 }
		 button->setZOrder(1000);
		 button->setEnabled(true);

	 }
	 //----------------------------------------------------------------------------------------

	 ImageView *image;
	 for (int i = 0; i < 7; i++)
	 {
		 image = dynamic_cast<ImageView*>(_rootNode->getChildByTag(1001 + i));
		 if (image)
		 {
			 iPosX[i] = image->getPositionX();
		 }
	 }

	return true;
}
void WelcomeLayer::onUIScrollViewEvent(Ref* sender, ScrollEventType e){
	if (e == cocos2d::ui::SCROLLVIEW_EVENT_SCROLLING) 
	{
		ListView *listView = (ListView*)sender;

		Size visiableSize = Director::getInstance()->getVisibleSize();

		float fOffSet = listView->getInnerContainer()->getPositionX();
		float fMove = 10000;
		int nCount=0;
		for (int i = 0; i < sp_vec.size()+1; i++)
		{
			if (fMove >= abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet))
			{
				fMove = abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet);
				nCount = i;
			}
		}
		auto panel = dynamic_cast<Layout*>(listView->getChildByName("panelComingSoon"));
		Button* Coming = dynamic_cast<Button*>(panel->getChildByName("btnComingSoon"));

		Coming->setScale(1.2 - (abs(6 * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) / 800));

		setAnimationVisible( nCount);
		turnEarth(fOffSet);

		if (fabs(fLastOff - fOffSet)>2)
		{
			fLastOff = fOffSet;
		}
		else
		{
			Bounce(sender,nCount);
		}
	}
}
void WelcomeLayer::Bounce(Ref* sender, int nCount)
{
	int nOffset = int(fabs(fLastOff)+0.5) % (PANEL_WIDTH + BLANK_SPACE);
	PageList *listView = (PageList*)sender;
	if (nOffset != 0)
	{
		listView->bounce(Vec2(-nCount*(PANEL_WIDTH + BLANK_SPACE), 0), 0.2f, true);
		//log("%d:%d", nCount, -nCount*(PANEL_WIDTH + BLANK_SPACE));
		fLastOff = listView->getInnerContainer()->getPositionX();
	}
}
void WelcomeLayer::touchButton(Ref* obj,::ui::Widget::TouchEventType eventType)  
{ 
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
	button->setEnabled(false);
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:
		app->PlayEffect(SOUND_BUTTON_CLICK);

		if(tag==1000){
			auto scene = ChooseLayer::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		if(tag==1001){
			m_iCurrentGameID = 1;
			loadGameWithTutorial(m_iCurrentGameID);
		}
		if(tag==1002){
			m_iCurrentGameID = 2;
			loadGameWithTutorial(m_iCurrentGameID);
		}
		if(tag==1003){
			m_iCurrentGameID = 3;
			loadGameWithTutorial(m_iCurrentGameID);

		}
		if(tag==1004){
			m_iCurrentGameID = 4;
			loadGameWithTutorial(m_iCurrentGameID);

		}
		if(tag==1005){
			m_iCurrentGameID = 5;
			loadGameWithTutorial(m_iCurrentGameID);
		}
		if(tag==1006){
			m_iCurrentGameID = 6;
			loadGameWithTutorial(m_iCurrentGameID);
		}
		this->cleanup();
		sp_vec.clear();

		break;
	}

}

void WelcomeLayer::setAnimationVisible( int iNum)
{
	ListView * listView = static_cast<ListView*>(_rootNode->getChildByName("lvGameNav"));
	Layout *panel;
	Armature *armature1;
	Armature *armature2;
	Sprite* board;
	Text* label;

	float fOffSet = listView->getInnerContainer()->getPositionX();

	for (int i = 0; i < 6; i++)
	{
		panel = dynamic_cast<Layout*>(listView->getChildByName(sPanel[i]));
		armature1 = dynamic_cast<Armature*>(panel->getChildByTag(7000 + i));
		armature2 = dynamic_cast<Armature*>(panel->getChildByTag(7200 + i));

		Button* button = dynamic_cast<Button*>(panel->getChildByName(sButton[i]));
		button->setZOrder(1000);
		button->setEnabled(true);

		if (i == iNum)
		{
			armature1->setVisible(true);
			armature2->setVisible(false);
			armature1->setScale(1.8 - (abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) / 1000));
			armature1->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
			button->setScale(2.65);
			button->setEnabled(true);
		}
		else
		{
			armature1->setVisible(false);
			armature2->setVisible(true);
			armature2->setScale(1.65 - (abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) / 800));
			armature2->setColor(ccc3(84,84,84));
			armature2->getAnimation()->playWithIndex(0, -1, 1);//from the first frame,loop
			//button->setScale(1);
			button->setScale(1.65);
			button->setEnabled(false);
		}
		board = dynamic_cast<Sprite*>(panel->getChildByName(sBoard[i]));
		label = dynamic_cast<Text*>(panel->getChildByName(sLabel[i]));

		board->setScale(1 - (abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) / 800));
		label->setScale(1 - (abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) / 800));

		board->setPositionY(30 + (abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) /5));
		label->setPositionY(30 + (abs(i * (PANEL_WIDTH + BLANK_SPACE) + fOffSet) / 5));
	}
}

void WelcomeLayer::turnEarth(int iOffset)
{
	ImageView *image;
	for (int i = 0; i < 7; i++)
	{
		image = dynamic_cast<ImageView*>(_rootNode->getChildByTag(1001 + i));
		if (image)
		{
			image->setPositionX(iPosX[i] + (iOffset )*(i*0.1+1)/5);
		}
	}
}
bool WelcomeLayer::loadGameWithTutorial(int nGameId){

	bool bRet = false;

	do{
		string sKey = "";
		string sValue = "";
		string sIsFirst = "";

		sIsFirst = DataAccess::getParamValue(TutorialIsFirstTime[m_iCurrentGameID], "0");

		if (atoi(sIsFirst.c_str()) == 0)
		{
			auto scene = TutorialScene::create();
			TutorialScene::_nIsIngame = 0;

			if (scene != nullptr){
				Director::getInstance()->replaceScene(scene);
			}
			//load->setStringForKey(TutorialIsFirstTime[nGameId],"1");
		}else{
			if (nGameId == 1){
				auto scene = FindDiffScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			} else if (nGameId == 2){
				auto scene = ZoomGameScene::create();
				//Avoid crash on loading scene, Added by Lance
				if(scene == nullptr){
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			} else if (nGameId == 3){
				auto scene = jigSawScene::create();
				if(scene == nullptr){	//Avoid crash on loading scene, Added by Lance
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			} else if (nGameId == 4){
				auto scene = GameScene::create();
				//auto scene = LevelSelectLayer::createScene();
				if(scene == nullptr){	//Avoid crash on loading scene, Added by Lance
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			} else if (nGameId == 5){
				auto scene = PriceMatchScene::create();
			
				if(scene == nullptr){	//Avoid crash on loading scene, Added by Lance
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			} else if (nGameId == 6){
				auto scene = MultiPriceMatchScene::create();
				if(scene == nullptr){ //Avoid crash on loading scene, Added by Lance
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			} 
		}
		bRet = true;
	}while(0);
	return bRet;
}

