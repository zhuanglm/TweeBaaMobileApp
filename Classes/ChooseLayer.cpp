#include "ChooseLayer.h"
#include "WelcomeScene.h"
//#include "Login.h"
//#include "CollageGUI/CollageMainTabScene.h"
#include "MemberCenter/RegisterTabScene.h"
#include "global_const.h"
#include "theme/ThemeLayer.h"
#include "ui/CocosGUI.h"
#include "Collage/CollageScene.h"

#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "utilits/pugixml.hpp"

#include "utilits/NetworkUtil.h"
#include "Msgbox/MsgBox.h"
#include "Msgbox/Dialogbox.h"
#include "CCAlert.h"
#include "utilits/SettingScene.h"
#include "EventBoard/EventBoard.h"
#include "LeaderBoardLayer.h"
#include "MemberCenter/RegisterView.h"
#include "utilits/Tutorial.h"
#include "ZhuanPanSystem/TurntableSystem.h"
#include "cocostudio/CocoStudio.h"//for animation
//#include "HelloWorldScene.h"
using namespace cocostudio;


using namespace ui; 
int iLocal[6] = { 0, 0, 0, 0, 0, 0 };

Scene* ChooseLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ChooseLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
ChooseLayer::~ChooseLayer(){

}
void ChooseLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);
    int tag = button->getTag();  
	AppDelegate *app = (AppDelegate*)Application::getInstance();

    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:

	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_BUTTON_CLICK);
	}

		if(tag==2001){

			//ChooseLayer::checkNetwork();
			
			app->iGameMode=MobileGameMode::Collage_Mode;
			auto scene = CollageScene::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
			
		}
		if(tag==2002){
			app->iGameMode=MobileGameMode::Game_Mode;
			auto scene = WelcomeScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);

		}
		if(tag==2003){ //Leader board
			auto scene = LeaderBoardLayer::createScene();	
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->pushScene(transition);
		}
		if (tag == 2004){ //Shopping Cart
////////////////////////
			if (m_DebugFlag)
			{
				auto director = Director::getInstance();
				auto dailyspin = TurntableSystem::create();
				//auto introscene = Login::createScene();
				director->replaceScene(dailyspin);
			}
			else
			{
				Application::getInstance()->openURL("https://www.tweebaa.com/Product/prdSaleAll.aspx");
			}
		}
		if (tag == 2005){ //Event board
			auto scene = EventBoard::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->pushScene(transition);
		}
		if (tag == 2007){ //Register
			auto scene = RegisterView::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->pushScene(transition);
		}
		if (tag == 2008){ //Tips to win
			m_iCurrentGameID = 0;
			auto scene = TutorialScene::create();
			if (scene != nullptr){
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->pushScene(transition);
			}

		}
		if (tag == 2009){
			auto scene = SettingScene::create();
			if(scene != nullptr){
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->pushScene(transition);
			//schedule(schedule_selector(ChooseLayer::updateLabel), 0.5f);
			}
		}
		if (tag == 2010)
		{
			app->iThemeID = 1;//need to be changed if add new story
			m_iCurrentGameID = -2;
			app->iGameMode = MobileGameMode::Theme_Mode;
			string sIsFirst = DataAccess::getParamValue(TutorialStory[app->iThemeID - 1], "\0");
			if (sIsFirst.length() == 0)
			{
				auto scene = TutorialScene::create();
				if (scene != nullptr){
					Director::getInstance()->replaceScene(scene);
				}
			}
			else
			{
				auto scene = ThemeLayer::createScene();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
		}
		if (tag == 8888){

			auto scene = RegisterView::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->pushScene(transition);
		}
		break;
	}
}
bool ChooseLayer::init()
{
	//////////////////////////////  
	// 1. super init first  
	if (!Layer::init())
	{
		return false;
	}

	initialize();
	return true;

}
void ChooseLayer::initialize() {

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sCSD=app->GetLayoutString();
	NetworkType ntType = app->GetNetWorkStatus();

	// app->UpdateUserScore(1, 10);

	//int i = app->GetUserTotalScore();

    _rootNode = CSLoader::createNode("res/MenuChoose1/MenuChooseLayer"+sCSD+".csb");

	addChild(_rootNode);

	//m_UserID = UserDefault::getInstance()->getIntegerForKey("CurrentUserId", -1);
	if (m_UserID > 0)
	{
		_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
		std::string sUsername = app->base64_xxtea_decode(userInfo.strUserName);
		sUsername = Convert(sUsername);
		auto lblREGISTER = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblREGISTER"));
		string sLbl = lblREGISTER->getStringValue();

		auto btnregister = dynamic_cast<Button*>(_rootNode->getChildByName("register"));

		btnregister->setVisible(false);
		lblREGISTER->setFontSize(48);
		lblREGISTER->setText(sUsername);
		lblREGISTER->setColor(ccc3(72, 37, 0));
		auto img = dynamic_cast<ImageView*>(_rootNode->getChildByName("Image_1"));
		img->setVisible(false);
	}


	auto btnCollageMode = dynamic_cast<Button*>(_rootNode->getChildByName("btnCollageMode"));
	btnCollageMode->setTag(2001);
	btnCollageMode->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));

	auto btnGameMode = dynamic_cast<Button*>(_rootNode->getChildByName("btnGameMode"));
	btnGameMode->setTag(2002);
	btnGameMode->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));


	///buttons
	auto btnLeaderboard = dynamic_cast<Button*>(_rootNode->getChildByName("Leaderboard"));
	if (btnLeaderboard){

		btnLeaderboard->setTag(2003);
		btnLeaderboard->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}

	auto btncart = dynamic_cast<Button*>(_rootNode->getChildByName("cart"));
	if (btncart){

		btncart->setTag(2004);
		btncart->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}


	auto btnsetting = dynamic_cast<Button*>(_rootNode->getChildByName("setting"));
	if (btnsetting){

		btnsetting->setTag(2009);
		btnsetting->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}

	
	auto btnregister = dynamic_cast<Button*>(_rootNode->getChildByName("register"));
	if (btnregister){

		btnregister->setTag(2007);
		btnregister->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}
	auto btntiptowin = dynamic_cast<Button*>(_rootNode->getChildByName("tiptowin"));
	if (btntiptowin){

		btntiptowin->setTag(2008);
		btntiptowin->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}
	

	auto btnmessage = dynamic_cast<Button*>(_rootNode->getChildByName("message"));
	if (btnmessage){
		btnmessage->setTag(2005);
		btnmessage->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}

	auto btntheme = dynamic_cast<Button*>(_rootNode->getChildByName("Button_2"));
	if (btntheme){
		btntheme->setTag(2010);
		btntheme->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	}

	auto btbnlogout = dynamic_cast<Button*>(_rootNode->getChildByName("Button_1"));
	if (btbnlogout){
		if (m_DebugFlag)
		{
			btbnlogout->setVisible(true);
			btbnlogout->setTag(8888);
			btbnlogout->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
		}
		else
		{
			btbnlogout->setVisible(false);
			btbnlogout->setEnabled(false);
		}
	}

	//disable collage, Leaderboard and Register feature
	if (ntType == NetworkType::NetworkTypeNone){
		btnCollageMode->setColor(ccc3(128,128,128));
		btnCollageMode->setEnabled(false);
		btnLeaderboard->setColor(ccc3(128,128,128));
		btnLeaderboard->setEnabled(false);
		btnregister->setColor(ccc3(128,128,128));
		btnregister->setEnabled(false);
	}

	//text field
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


	SyncData();

	//DoDailySpin();

	if ( app->getBGMstatus() == BGMusic_flag::ON){
		app->StartBGMusic(BGM_MAIN);
	}

	m_Refresh = 0;
	schedule(schedule_selector(ChooseLayer::updateLabel), 0.5f);

	auto mastco = dynamic_cast<Sprite *> (_rootNode->getChildByName("mascot_5"));
	auto hand = dynamic_cast<Sprite *> (_rootNode->getChildByName("m_hand_6"));
	mastco->setVisible(false);
	hand->setVisible(false);
	auto hill = dynamic_cast<Sprite *>(_rootNode->getChildByName("hill_4"));
	

	ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/1.0/1.0.ExportJson");
	Armature *armature = Armature::create("1.0");
	armature->setAnchorPoint(Vec2(0.5, 0.5));
	armature->setPosition(Vec2(850, 160));
	armature->setTag(888);
	ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/1.0-1/1.0-1.ExportJson");
	Armature *armature1 = Armature::create("1.0-1");
	armature1->setAnchorPoint(Vec2(0.5, 0.5));
	armature1->setPosition(Vec2(867, 158)); 
	armature1->setTag(889);

	hill->setZOrder(1);
	armature1->setZOrder(2);
	btnsetting->setZOrder(2);

	_rootNode->addChild(armature);
	_rootNode->addChild(armature1);
	schedule(schedule_selector(ChooseLayer::PlayAnimation), 5.0f);

/*	cocostudio::Armature* armature;
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/game_passed/passed_star.ExportJson");
	armature = cocostudio::Armature::create("passed_star");
	if (armature){
		armature->setPosition(ccp(200, 200));
		armature->getAnimation()->playByIndex(0);
		//armature->setScale(0.25);
		this->addChild(armature); 
	}*/
	



	//auto btnSetting = dynamic_cast<Button*>(_rootNode->getChildByName("setting"));
	//btnSetting->setTag(2009);
	//btnSetting->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));

	/*
	auto btnThemeMode = dynamic_cast<Button*>(_rootNode->getChildByName("btnThemeMode"));
	btnThemeMode->setTag(2003);
	btnThemeMode->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::touchButton, this));
	*/

	/*
	cocostudio::timeline::ActionTimeline* action = CSLoader::createTimeline("res/MenuChoose1/MenuChooseLayer" + sCSD + ".csb");

	_rootNode->runAction(action);

	action->gotoFrameAndPlay(0, true);
	*/
	/*
	if(!Layer::init()){
		return false;
	}
	//get the origin point of the X-Y axis, and the visiable size of the screen
	Size visiableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	::ui::Layout* layout = ::ui::Layout::create();
	layout->setSize(Size(visiableSize.width, visiableSize.height));
	layout->setBackGroundColorType(::ui::LAYOUT_COLOR_SOLID);
	layout->setBackGroundColor(ccc3(4,123,182));
	layout->setPosition(Vec2(0, 0));
	addChild(layout,-5);

	Sprite* background = Sprite::create("MenuChoose1/choose_menu_bg.png");
	
	float scaleRate = visiableSize.width / background->getContentSize().width;
	background->setScale(scaleRate);
	background->setAnchorPoint(Vec2(0.0f,1.0f)) ;
	float height =visiableSize.height- (visiableSize.height - background->getContentSize().height * scaleRate)/2;
	background->setPosition(Vec2(0, height));
	addChild(background,-5);


	//get the current time, the background image will selected by current time day or night: bg_day or bg_night

	//Game Find Difference
	Sprite *game1 = Sprite::create("MenuChoose1/Collage_Mode.png");
	Sprite *game1_a = Sprite::create("MenuChoose1/Collage_Mode.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game1_a->setPositionY(5);
	game1->setScale(scaleRate);
	game1_a->setScale(scaleRate);
	auto menuItem_1  = MenuItemSprite::create(game1,game1_a,NULL,CC_CALLBACK_1(ChooseLayer::menuStartCallback_Game1, this));
	menuItem_1->setPosition(Point(origin.x + visiableSize.width/2 -game1->getContentSize().width*scaleRate/2 -20,origin.y + visiableSize.height/2+100));

    auto menu1 = Menu::create(menuItem_1,NULL);
	menu1->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu1,1);

	//Game Zoom game
	Sprite *game2 = Sprite::create("MenuChoose1/Game_Mode.png");
	Sprite *game2_a = Sprite::create("MenuChoose1/Game_Mode.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game2_a->setPositionY(5);
	game2->setScale(scaleRate);
	game2_a->setScale(scaleRate);
	auto menuItem_2  = MenuItemSprite::create(game2,game2_a,NULL,CC_CALLBACK_1(ChooseLayer::menuStartCallback_Game2, this));
	menuItem_2->setPosition(Point(origin.x + visiableSize.width/2 +game2->getContentSize().width*scaleRate/2+20,origin.y + visiableSize.height/2+100));

    auto menu2 = Menu::create(menuItem_2,NULL);
	menu2->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu2,1);

	//pixel shuffle
	
	return true;
	*/
}


/*
void ChooseLayer::menuStartCallback_Game1(Ref *sender){
	//auto scene = Login::createScene();
	
	//auto scene = RegisterTab::createScene();
	
	auto scene = CollageMainTab::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
	
}
*/
//
//size_t ChooseLayer::writeData(uint8_t* ptr,size_t size,size_t number,void *stream)  
//{  
//  
//    log("%s",ptr);  
//    return size*number;//这里一定要返回实际返回的字节数  
//} 
//void ChooseLayer::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
//{
//    if (!response)
//    {
//		//StartGame();
//        return;
//    }   
//
//    // You can get original request type from: response->request->reqType
//    if (0 != strlen(response->getHttpRequest()->getTag()))
//    {
//        log("%s completed", response->getHttpRequest()->getTag());
//    }   
//    int statusCode = response->getResponseCode();
//    char statusString[64] = {};
//    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
//   // _labelStatusCode->setString(statusString);
//    log("response code: %d", statusCode);   
//    if (!response->isSucceed())
//    {
//        log("response failed");
//        log("error buffer: %s", response->getErrorBuffer());
//		//StartGame();
//        return;
//	}
//    // dump data
//    std::vector<char> *buffer = response->getResponseData();
//
//	ClaimLocalReward();
//
//
//}
//void ChooseLayer::checkNetwork(){
//
//	AppDelegate *app = (AppDelegate*)Application::getInstance();
//	NetworkType nt_type = app->GetNetWorkStatus();
//	MsgBoxType m_type = TP_NONE;
//	std::string m;
//
//	if (nt_type == NetworkType3G){
//		m = MSG_NETWORK_3G;
//		m_type = MsgBoxType::TP_Q_YESNO;
//
//
//		//auto eventrecieverno = EventListenerCustom::create(APP_CE_NO,[this](EventCustom* event)
//		//{
//		//	auto voice = (Value*)event->getUserData();
//		//	//log("「%s」", voice->asString().c_str());
//		//	menuCallbackPopupNo();
//		//});
//		//getEventDispatcher()->addEventListenerWithFixedPriority(eventrecieverno, 1);
//
//		//auto eventrecieveryes = EventListenerCustom::create(APP_CE_YES,[this](EventCustom* event)
//		//{
//		//	auto voice = (Value*)event->getUserData();
//		//	//log("「%s」", voice->asString().c_str());
//		//	menuCallbackPopupYes();
//		//});
//		//getEventDispatcher()->addEventListenerWithFixedPriority(eventrecieveryes, 1);
//
//		//auto scene = MessageScene::create();
//		//TransitionScene *transition = TransitionFade::create(1, scene);
//		//Director::getInstance()->replaceScene(transition);
//		//DlgBox::setMsgBox(m_type,&m);
//		//DlgBox *dlg = DlgBox::create();
//		//dlg->setTag(9999);
//		//dlg->setVisible(true);
//		//dlg->setSwallowsTouches(false);
//		//dlg->setTouchEnabled(false);
//
//		MsgBoxType m_type = TP_Q_YESNO;
//		std::string title = "Network";
//		MsgBox::setMsgBox(m_type,&title,&m);
//		auto msgbox = MsgBox::create();
//		msgbox->setTag(9000);
//		this->addChild(msgbox,999);
//
//		auto btnYes = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
//		auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
//		btnYes->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::yesCallback, this));
//		btnNo->addTouchEventListener(CC_CALLBACK_2(ChooseLayer::noCallback, this));
//
//		//_rootNode = CSLoader::createNode("res/MenuChoose1/MenuChooseLayer.csb");
//		//_rootNode->addChild(msgbox, 99999);
//	}else{
//		AppDelegate *app = (AppDelegate*)Application::getInstance();
//		app->iGameMode=MobileGameMode::Collage_Mode;
//		auto scene = CollageScene::createScene();
//		if (scene != nullptr){
//		TransitionScene *transition = TransitionFade::create(1, scene);
//		Director::getInstance()->replaceScene(transition);	
//		}else{
//				MessageBox("ChooseLayer::checkNetwork: Cannot load Collage Scene", "Error - ChooseLayer");
//		}
//	}
//	return;
//}

//void ChooseLayer::menuCallbackPopupNo(){
//	//MessageBox("Don't download","menuCallbackPopupNo");
//
//	//AppDelegate *app = (AppDelegate*)Application::getInstance();
//	//app->iGameMode=MobileGameMode::Collage_Mode;
//	//auto scene = CollageScene::createScene();
//	//TransitionScene *transition = TransitionFade::create(1, scene);
//	//Director::getInstance()->replaceScene(transition);
//
//	getEventDispatcher()->removeCustomEventListeners(APP_CE_NO);
//	getEventDispatcher()->removeCustomEventListeners(APP_CE_YES);
//}
//
//void ChooseLayer::menuCallbackPopupYes(){
//	AppDelegate *app = (AppDelegate*)Application::getInstance();
//	app->iGameMode=MobileGameMode::Collage_Mode;
//	auto scene = CollageScene::createScene();
//	if (scene != nullptr){
//		TransitionScene *transition = TransitionFade::create(1, scene);
//		Director::getInstance()->replaceScene(transition);
//	}else{
//		MessageBox("ChooseLayer::menuCallbackPopupYes: Cannot load Collage Scene", "Error - ChooseLayer");
//	}
//	//MessageBox("Download files","menuCallbackPopupYes");
//	getEventDispatcher()->removeCustomEventListeners(APP_CE_NO);
//	getEventDispatcher()->removeCustomEventListeners(APP_CE_YES);
//}
//void ChooseLayer::menuCallbackPopupOk(){
//	//AppDelegate *app = (AppDelegate*)Application::getInstance();
//	//app->iGameMode=MobileGameMode::Collage_Mode;
//	//auto scene = CollageScene::createScene();
//	//TransitionScene *transition = TransitionFade::create(1, scene);
//	//Director::getInstance()->replaceScene(transition);
//
//	//MessageBox("Download files","menuCallbackPopupYes");
//	getEventDispatcher()->removeCustomEventListeners(APP_CE_OK);
//	//getEventDispatcher()->removeCustomEventListeners(APP_CE_YES);
//}

//void ChooseLayer::yesCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
//	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
//		AppDelegate *app = (AppDelegate*)Application::getInstance();
//		app->iGameMode=MobileGameMode::Collage_Mode;
//		auto scene = CollageScene::createScene();
//		if (scene != nullptr){
//			TransitionScene *transition = TransitionFade::create(1, scene);
//			Director::getInstance()->replaceScene(transition);
//		}else{
//			MessageBox("ChooseLayer::menuCallbackPopupYes: Cannot load Collage Scene", "Error - ChooseLayer");
//		}
//	}
//	return;
//}
//void ChooseLayer::noCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
//
//	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
//		this->getChildByTag(9000)->removeFromParent();
//	}
//	return;
//}
/*
void ChooseLayer::menuStartCallback_Game2(Ref *sender){
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);

}
*/


void ChooseLayer::PlayAnimation(float dt)
{
	Armature *armature = dynamic_cast<Armature *>(_rootNode->getChildByTag(888));
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	//armature->getAnimation()->setSpeedScale(0.5);
	scheduleOnce(schedule_selector(ChooseLayer::PlayAnimation1), 0.1f);
}

void ChooseLayer::PlayAnimation1(float dt)
{
	Armature *armature1 = dynamic_cast<Armature *>(_rootNode->getChildByTag(889));
	armature1->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	//armature1->getAnimation()->setSpeedScale(0.5);
}

void ChooseLayer::ClaimLocalReward()
{

}


std::string ChooseLayer::Convert(std::string& str)
{
	std::string converted;
	for (short i = 0; i < str.size(); ++i)
		converted += toupper(str[i]);
	return converted;
}


void ChooseLayer::SyncData()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string  strFlow = ReadLocalFlow();//read local and add current

	NetworkType Networ = app->GetNetWorkStatus();
	if (Networ != NetworkTypeNone)
	{
		if (m_UserID > 0 )
		{
			_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);

			HttpRequest* request = new HttpRequest();
			request->setUrl(UCAPIWebPostURL);
			request->setRequestType(HttpRequest::Type::POST);
			request->setResponseCallback(CC_CALLBACK_2(ChooseLayer::onSyncComplete, this));

			//char p[10];
			//sprintf(p, "%s", strFlow);
			std::string sPost = "action=save_user_info&EncryptData=";
			
			long lUserDiff = app->GetUserScoreDiff();
			int iLevel = app->GetUserLevel();
			int iTools = app->GetToolsDiff();

			char p1[10];
			char p2[10];
			char p3[10];
			sprintf(p1, "%d", lUserDiff);
			sprintf(p2, "%d", iLevel);
			sprintf(p3, "%d", iTools);
			string s1 = p1;
			string s2 = p2;
			string s3 = p3;
			//std::string sPost = "action=sync_point&EncryptData=";

			string s4 = "score = " + strFlow + "&local_point = " + s1 + "&LV = " + s2 + "&UserID = " + userInfo.strUserGuid + "&VirtualPops=" + s3;
			s4 = app->encode_data_for_http(s4);
			sPost = sPost + s4;
			sPost = sPost + "&" + app->GetAccessString();

			const char* postData = sPost.c_str();
			request->setRequestData(postData, strlen(postData));
			request->setTag("save_ranking");
			cocos2d::network::HttpClient::getInstance()->send(request);
			request->release();
		}
	}
	return;
}


std::string ChooseLayer::ReadLocalFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();

	std::string strSyncFlag = DataAccess::getParamValue("SyncFlag", "0");
	if (strSyncFlag.length()>0 && atoi(strSyncFlag.c_str()) != 0)
	{
		string strLocal = DataAccess::getParamValue("LocalFlow", "0:0");
		strLocal = app->base64_xxtea_decode(strLocal);//format:x,x,x,x,x,x,
		string::size_type pos;
		string sSub;
		for (int i = 0; i < 6; i++)
		{
			pos = strLocal.find(",");
			if (pos != string::npos)//found
			{
				sSub = strLocal.substr(0, pos);
				strLocal = strLocal.substr(pos + 1, strLocal.size() - 1);
				iLocal[i] = atoi(sSub.c_str());
			}
			else return "0:0";//wrong 
		}
	}
	else   return "0:0";
	char cFlow[20] = "\0";
	std::string strFlow = "";
	for (int i = 0; i < 6; i++)
	{
		if (iLocal[i]>0)
		{
			sprintf(cFlow, "%d:%d,\0", i + 1, iLocal[i]);
			strFlow += cFlow;
		}
	}
	return strFlow;
}


void ChooseLayer::clearLocalFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();

	string strLocal = "0,0,0,0,0,0,";
	strLocal = app->base64_xxtea_encode(strLocal);
	DataAccess::setParamValue("LocalFlow", strLocal);
	DataAccess::setParamValue("SyncFlag", "0");

}

void ChooseLayer::updateLabel(float dt)
{
	//if (m_UserID < 0){
	//	if (m_Refresh){
	//		m_Refresh = 0;
	//		auto lblREGISTER = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblREGISTER"));
	//		string sLbl = "REGISTER";
	//		lblREGISTER->setFontSize(32);
	//		lblREGISTER->setColor(ccc3(255, 248, 220));
	//		lblREGISTER->setText(sLbl);

	//		auto btnregister = dynamic_cast<Button*>(_rootNode->getChildByName("register"));
	//		btnregister->setVisible(true);
	//		auto img = dynamic_cast<ImageView*>(_rootNode->getChildByName("Image_1"));
	//		img->setVisible(true);


	//		long l_coins = app->GetUserTotalScore();
	//		auto lblCoins = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("txtScore"));
	//		string sTtl = lblCoins->getStringValue();

	//		if (l_coins != atol(sTtl.c_str()))
	//		{
	//			auto lblLevel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("txtLvl"));

	//			int i_level = app->GetUserLevel();

	//			char p[20];
	//			sprintf(p, "%ld", l_coins);
	//			string sTotal = p;
	//			if (lblCoins) lblCoins->setText(sTotal);

	//			sprintf(p, "%d", i_level);
	//			sTotal = p;
	//			if (lblLevel) lblLevel->setText(sTotal);

	//			float _fPerToNLvl; //Percent to next level
	//			_fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
	//			auto lvlbar = dynamic_cast<Sprite*> (_rootNode->getChildByName("lv_bar_4"));
	//			lvlbar->setAnchorPoint(Vec2(0, 0.5));
	//			lvlbar->setScaleX(_fPerToNLvl);
	//		}
	//	}
	//}

	if (m_UserID != 0){
		AppDelegate* app = (AppDelegate*)Application::getInstance();
		
		if (m_Refresh)
		{
			m_Refresh = 0;
			if(m_UserID > 0){
				_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
				std::string sUsername = app->base64_xxtea_decode(userInfo.strUserName);
				sUsername = Convert(sUsername);
				auto lblREGISTER = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblREGISTER"));
				string sLbl = lblREGISTER->getStringValue();
				auto btnregister = dynamic_cast<Button*>(_rootNode->getChildByName("register"));
			
				auto img = dynamic_cast<ImageView*>(_rootNode->getChildByName("Image_1"));

				btnregister->setVisible(false);
				img->setVisible(false);
				lblREGISTER->setFontSize(48);
				lblREGISTER->setText(sUsername);
				lblREGISTER->setColor(ccc3(72, 37, 0));

				SyncData();

			}else if(m_UserID < 0){
				auto lblREGISTER = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblREGISTER"));
				string sLbl = "REGISTER";
				lblREGISTER->setFontSize(32);
				lblREGISTER->setColor(ccc3(255, 248, 220));
				lblREGISTER->setText(sLbl);

				auto btnregister = dynamic_cast<Button*>(_rootNode->getChildByName("register"));
				btnregister->setVisible(true);
				auto img = dynamic_cast<ImageView*>(_rootNode->getChildByName("Image_1"));
				img->setVisible(true);
			}
		}

		long l_coins = app->GetUserTotalScore();

		auto lblCoins = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("txtScore"));
		string sTtl = lblCoins->getStringValue();

		int iNewsRead = atoi(DataAccess::getParamValue("NewsRead", "0").c_str());
		{
			auto btnmessage = dynamic_cast<Button*>(_rootNode->getChildByName("message"));
			if (iNewsRead == 0)
			{
				btnmessage->loadTextures("res/MenuChoose1/message_pop.png", "res/MenuChoose1/message_pop.png", "res/MenuChoose1/message_pop.png");
			}
			else
			{
				btnmessage->loadTextures("res/MenuChoose1/message.png", "res/MenuChoose1/message_p.png", "res/MenuChoose1/message_p.png");
			}

		}

		if (l_coins != atol(sTtl.c_str()))
		{
			auto lblLevel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("txtLvl"));

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
			lvlbar->setScaleX(_fPerToNLvl);
		}

	}
}

void ChooseLayer::onSyncComplete(HttpClient *sender, HttpResponse *response)
{
	//_rootNode->scheduleOnce(schedule_selector(ChooseLayer::updateLabel), 0.1f);
	//m_Refresh = 1;
	
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	// _labelStatusCode->setString(statusString);
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	if (statusCode != 200){
		log("response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	clearLocalFlow();

	// dump data
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::vector<char> *buffer = response->getResponseData();
	string sXml(buffer->begin(), buffer->end());
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml, sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppSyncPoint");

	std::string strResult = nodes.child_value("result");
	char p[100];
	strcpy(p, strResult.c_str());
	int iError = atoi(p);
	if (iError == 0)
	{
		//get TotalPoints
		strResult = nodes.child_value("TotalPoints");
		long UserScoreDiff = atol(strResult.c_str());
		if (UserScoreDiff >= 0 && UserScoreDiff < 1000000)
		{
			app->SaveUserScoreDiff(UserScoreDiff);
			m_iUserTotalScore = UserScoreDiff;
		}
		else {

			app->SaveUserScoreDiff(0);
			m_iUserTotalScore = 0;
		}
		//get TotalTools
		strResult = nodes.child_value("TotalTools");
		int ToolsDiff = atoi(strResult.c_str());
		if (ToolsDiff >= 0 && ToolsDiff < 1000)
		{
			app->SaveUserToolsDiff(atoi(strResult.c_str()));
		}
	}
	
}
