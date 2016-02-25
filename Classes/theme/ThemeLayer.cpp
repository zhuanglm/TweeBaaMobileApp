#include "ThemeLayer.h"
#include "../memory_match/GameScene.h"
#include "../jigsaw/jigSawScene.h"
#include "WelcomeScene.h"
#include "../MemberCenter/RegisterTabScene.h"
#include "Login.h"
#include "AppDelegate.h"
#include "DataAccess/DataAccess.h"
#include "ChooseLayer.h"

#include "cocostudio/ActionTimeline/CSLoader.h"


ThemeLayer::ThemeLayer(){};

ThemeLayer::~ThemeLayer(){};


Scene* ThemeLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ThemeLayer::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool ThemeLayer::init(){
	if(!Layer::init()){
		return false;
	}
	//get the origin point of the X-Y axis, and the visiable size of the screen

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	  std::string sCSD=app->GetLayoutString();
	 _rootNode = CSLoader::createNode("res/theme/ThemeMenu.csb");

	 //loginLayer=Layer::create();
	 addChild(_rootNode);

	 m_iCurrentGameID = -2;//theme mode

	auto btnRound1 = dynamic_cast<Button*>(_rootNode->getChildByName("Button_1"));
	btnRound1->setTag(1001);
	btnRound1->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));

	auto btnRound2 = dynamic_cast<Button*>(_rootNode->getChildByName("Button_2"));
	btnRound2->setTag(1002);
	btnRound2->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));


	auto btnRound3 = dynamic_cast<Button*>(_rootNode->getChildByName("Button_3"));
	btnRound3->setTag(1003);
	btnRound3->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));

	auto btnRound4 = dynamic_cast<Button*>(_rootNode->getChildByName("Button_4"));
	btnRound4->setTag(1004);
	btnRound4->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));

	auto btnRound5 = dynamic_cast<Button*>(_rootNode->getChildByName("Button_5"));
	btnRound5->setTag(1005);
	btnRound5->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));

	auto Sprite2 = dynamic_cast<Sprite*>(_rootNode->getChildByName("Sprite_2"));
	auto Sprite3 = dynamic_cast<Sprite*>(_rootNode->getChildByName("Sprite_3"));
	auto Sprite4 = dynamic_cast<Sprite*>(_rootNode->getChildByName("Sprite_4"));
	auto Sprite5 = dynamic_cast<Sprite*>(_rootNode->getChildByName("Sprite_5"));


	string strPassLv1 = DataAccess::getParamValue("PassTheme1_1", "\0");
	{
		if (strPassLv1.length() > 0)
		{
			btnRound1->loadTextures(STAR, STAR, STAR);
		}
		else
		{
			btnRound1->loadTextures(HOLO, HOLO, HOLO);
			btnRound2->setEnabled(false);
		}
	}


	string strPassLv2 = DataAccess::getParamValue("PassTheme1_2", "\0");
	{
		if (strPassLv2.length() > 0)
		{
			btnRound2->loadTextures(STAR, STAR, STAR);
			Sprite2->setTexture(WOOD);
		}
		else
		{
			btnRound3->setEnabled(false);
			if (strPassLv1.length() > 0)
			{
				btnRound2->loadTextures(HOLO, HOLO, HOLO);
				Sprite2->setTexture(WOOD);
			}
		}
	}


	string strPassLv3 = DataAccess::getParamValue("PassTheme1_3", "\0");
	{
		if (strPassLv3.length() > 0)
		{
			btnRound3->loadTextures(STAR, STAR, STAR);
			Sprite3->setTexture(WOOD);
		}
		else
		{
			btnRound4->setEnabled(false);
			if (strPassLv2.length() > 0)
			{
				btnRound3->loadTextures(HOLO, HOLO, HOLO);
				Sprite3->setTexture(WOOD);
			}
		}
	}


	string strPassLv4 = DataAccess::getParamValue("PassTheme1_4", "\0");
	{
		if (strPassLv4.length() > 0)
		{
			btnRound4->loadTextures(STAR, STAR, STAR);
			Sprite4->setTexture(WOOD);
		}
		else
		{
			btnRound5->setEnabled(false);
			if (strPassLv3.length() > 0)
			{
				btnRound4->loadTextures(HOLO, HOLO, HOLO);
				Sprite4->setTexture(WOOD);
			}
		}
	}

	string strPassLv5 = DataAccess::getParamValue("PassTheme1_5", "\0");
	{
		if (strPassLv5.length() > 0)
		{
			btnRound5->loadTextures(STAR, STAR, STAR);
			Sprite5->setTexture(WOOD);
		}
		else
		{
			if (strPassLv4.length() > 0)
			{
				btnRound5->loadTextures(HOLO, HOLO, HOLO);
				Sprite5->setTexture(WOOD);
			}
		}
	}

	auto btnBack = dynamic_cast<Button*>(_rootNode->getChildByName("button_Main"));
	if (btnBack){
		btnBack->setTag(1006);
		btnBack->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));
	}

	auto btnTips = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTips"));
	if (btnTips){
		btnTips->setTag(2001);
		btnTips->addTouchEventListener(CC_CALLBACK_2(ThemeLayer::touchButton, this));
	}
	app->LogAccess("ThemeLayer");
	return true;
}

void ThemeLayer::touchButton(Ref* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if(tag==1001){
			app->iRound = 1;
		}
		if (tag == 1002){
			app->iRound = 2;
		}
		if (tag == 1003){
			app->iRound = 3;
		}
		if (tag == 1004){
			app->iRound =4;
		}
		if (tag == 1005){
			app->iRound = 5;
		}
		if (tag == 1006)
		{
			auto scene = ChooseLayer::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
			return;
		}
		if (tag == 2001){ //Tutorial
			auto scene = TutorialScene::create();
			if (scene != nullptr){
				TransitionScene *transition = TransitionFade::create(0, scene);
				Director::getInstance()->pushScene(transition);
			}
			return;
		}
		app->iCurrentStep = 0;
		//app->iGameMode = MobileGameMode::Theme_Mode;
		DataAccess::initTheme();
		app->StartTheme();
		break;
	}

}
