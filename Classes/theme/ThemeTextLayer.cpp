#include "ThemeTextLayer.h"
#include "../memory_match/GameScene.h"
#include "../jigsaw/jigSawScene.h"
#include "WelcomeScene.h"
#include "../MemberCenter/RegisterTabScene.h"
#include "Login.h"
#include "AppDelegate.h"


#include "cocostudio/ActionTimeline/CSLoader.h"


ThemeTextLayer::ThemeTextLayer(){};

ThemeTextLayer::~ThemeTextLayer(){};


Scene* ThemeTextLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ThemeTextLayer::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



bool ThemeTextLayer::init(){
	if(!Layer::init()){
		return false;
	}
	//get the origin point of the X-Y axis, and the visiable size of the screen
	/*
	Size visiableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	float generalscalefactor  = visiableSize.height / 500 ;
	AppDelegate *app = (AppDelegate*)Application::getInstance();

	GameRound* round =app->sp_GameRound.at(app->iCurrentStep);

	CCLabelTTF* lblInfo= CCLabelTTF::create (round->round_info, "bionic.fnt",30,Size(visiableSize.width*0.8, visiableSize.height*.7));
	//lblProductName->setScale(1.5f*generalscalefactor);
	//lblProductName->setAnchorPoint(Vec2(1.0f,1.0f));
	lblInfo->setColor(ccc3(72, 37, 0));
	lblInfo->setPosition(Vec2(visiableSize.width/2,visiableSize.height/2));
	this->addChild(lblInfo);

	
	Sprite *game7 = Sprite::create("public/contiune_btn.png");
	Sprite *game7_a = Sprite::create("public/contiune_btn.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game7_a->setPositionY(5);
	auto menuItem_7  = MenuItemSprite::create(game7,game7_a,NULL,CC_CALLBACK_1(ThemeTextLayer::menuCallback, this));
	menuItem_7->setPosition(Vec2(visiableSize.width/2.0f , visiableSize.height/2.0f));

	auto menu7 = Menu::create(menuItem_7,NULL);
	menu7->setPosition(Vec2(origin.x ,origin.y));
	this->addChild(menu7,1);
	*/

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	  std::string sCSD=app->GetLayoutString();
	 _rootNode = CSLoader::createNode("res/theme/ThemeLayer.csb");

	 //loginLayer=Layer::create();
	 addChild(_rootNode);

	 auto btnContinue = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("Button_1"));
	 	 btnContinue->setTag(1001);
		 string strPic = app->sp_GameRound.at(app->iCurrentStep).round_info;
		 btnContinue->loadTextures(strPic, strPic, strPic);
	btnContinue->addTouchEventListener(CC_CALLBACK_2(ThemeTextLayer::touchButton, this));
	btnContinue->setEnabled(false);


	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		string strPlayingFile = UserDefault::getInstance()->getStringForKey("BGM_FILE","\0");//whats playing now
		if (strcmp(strPlayingFile.c_str(), BGM_THEME) != 0)
		{
			app->StartBGMusic(BGM_THEME);
		}

		if (app->sp_GameRound.at(app->iCurrentStep).round_effect.length() > 5)
		{
			app->PlayEffect(app->sp_GameRound.at(app->iCurrentStep).round_effect.c_str());
		}
	}
	this->scheduleOnce(schedule_selector(ThemeTextLayer::enableBtn), 1.0f);
		
	
	//auto txtTheme = dynamic_cast<::ui::TextField*>(_rootNode->getChildByName("txtTheme"));

	//AppDelegate *app = (AppDelegate*)Application::getInstance();

	//stGameRound round = app->sp_GameRound.at(app->iCurrentStep);

//	CCLabelTTF* lblInfo= CCLabelTTF::create (round->round_info, "bionic.fnt",30,Size(text_container->getContentSize().width*0.9, text_container->getContentSize().height*.9));
	//lblProductName->setScale(1.5f*generalscalefactor);
	//lblProductName->setAnchorPoint(Vec2(1.0f,1.0f));
	//txtTheme->setColor(ccc3(72, 37, 0));
	//lblInfo->setPosition(Vec2(text_container->getContentSize().width/2,text_container->getContentSize().height));
	//text_container->addChild(lblInfo);
	//txtTheme->setString(round.round_info);
	return true;
}
void ThemeTextLayer::enableBtn(float dt)
{
	auto btnContinue = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("Button_1"));
	btnContinue->setEnabled(true);
}
void ThemeTextLayer::playEffect(float dt)
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
}

void ThemeTextLayer::touchButton(Ref* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  
		if (tag == 1001){
			auto btnContinue = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("Button_1"));
			btnContinue->setEnabled(false);
			AppDelegate *app = (AppDelegate*)Application::getInstance();
			app->GameRunRoundNext();
		}
		break;
	}

}




