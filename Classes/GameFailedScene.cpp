#include "GameFailedScene.h"
//#include "memory_match/memory_match_welcome.h"
#include "ChooseLayer.h"
//#include "ShareScene.h"
#include "WelcomeScene.h"
//#include "global_const.h"
#include "utilits/pugixml.hpp"
#include "C2DXShareSDK/C2DXShareSDK.h"
#include "CheckoutScene.h"

#include "cocos-ext.h"

#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "PriceMatch/PriceMatchScene.h"

#include "find_diff/FindDiffScene.h"
#include "zoom/ZoomGameScene.h"
#include "memory_match/GameScene.h"
#include "jigsaw/jigSawScene.h"
#include "MultiPriceMatch/MultiPriceMatchScene.h"

#include "Msgbox/Dialogbox.h"
#include "Msgbox/MsgBox.h"

#include "MemberCenter/RegisterView.h"
#include "utilits/NetworkUtil.h"
#include "theme/ThemeLayer.h"


USING_NS_CC;

USING_NS_CC_EXT;

using namespace ui; 
using namespace cn::sharesdk;

string m_shareLinkID;
extern std::string m_strProductGUID;
extern std::string m_strProductName;
extern std::string m_strImage;

int m_iGameFailed_Load=0;
int m_iShowRegister = 0;

/*
void authResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
           // CCLog("授权成功");
            break;
        case C2DXResponseStateFail:
          //  CCLog("授权失败");
            break;
        default:
            break;
    }
}

void getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *userInfo, CCDictionary *error)
{
    if (state == C2DXResponseStateSuccess)
    {
        //输出用户信息
        try
        {
            CCArray *allKeys = userInfo -> allKeys();
			allKeys->retain();
            for (int i = 0; i < allKeys -> count(); i++)
            {
                CCString *key = (CCString *)allKeys -> objectAtIndex(i);
                CCObject *obj = userInfo -> objectForKey(key -> getCString());
                
               // CCLog("key = %s", key -> getCString());
                if (dynamic_cast<CCString *>(obj))
                {
                   // CCLog("value = %s", dynamic_cast<CCString *>(obj) -> getCString());
                }
                else if (dynamic_cast<CCInteger *>(obj))
                {
                   // CCLog("value = %d", dynamic_cast<CCInteger *>(obj) -> getValue());
                }
                else if (dynamic_cast<CCDouble *>(obj))
                {
                   // CCLog("value = %f", dynamic_cast<CCDouble *>(obj) -> getValue());
                }
            }
			allKeys->release();
        }
        catch(...)
        {
            //CCLog("==============error");
        }
        
    }
}
*/
void GameFailedScene_shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
			cocos2d::MessageBox("Share Successful","Share");
           // CCLog("分享成功");
            break;
        case C2DXResponseStateFail:
			cocos2d::MessageBox("Share Failed","Share");
           // CCLog("分享失败");
            break;
        default:
            break;
    }
}


Scene* GameFailed::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameFailed::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameFailed::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_strProductName="";
	m_strImage="";
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//float generalscalefactor  = visibleSize.height / 500 ;
  /* 
	Sprite* background = Sprite::create("game_over/game_over_bg.png");
		
	//background->setScale(visiableSize.width / background->getContentSize().width);
	background->setAnchorPoint(Vec2(0.0f,1.0f)) ;
	background->setPosition(Vec2(0, visiableSize.height));
	addChild(background,-5);

	//float generalscalefactor  = visiableSize.width / background->getContentSize().width;
	
	  float x0=visiableSize.width / background->getContentSize().width;
	  float y0=visiableSize.height / background->getContentSize().height;
	  float generalscalefactor  =min(x0,y0);
	  background->setScale(generalscalefactor);

HttpRequest* request = new HttpRequest();
    request->setUrl(PromotionWebGetURL);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(GameFailed::onHttpRequestCompleted,this));
    request->setTag("GET test");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();

		// start ansyc method load the atlas.png
	//Director::getInstance()->getTextureCache()->addImageAsync("MatchBg.png", CC_CALLBACK_1(GameWin::loadingCallBack, this));
	 auto card1 = Sprite::create("match_game_img_1_b.jpg");
	 card1->setPosition(Vec2(visiableSize.width / 2 +88 * generalscalefactor,   visiableSize.height/2 +50 *generalscalefactor));
	 //float zoom_x = visiableSize.width *0.5 / card1->getContentSize().width;
	// float zoom_y = visiableSize.height * 0.5 / card1->getContentSize().height;
	 card1->setScale(generalscalefactor*150/800);
	 //card1->setScaleY(zoom_y);

	  this->addChild(card1,1,1001);

   CCLabelTTF* lblProductName= CCLabelTTF::create ("Kitty Litty, $25.00 now Free Shipping at USA", "bionic.fnt",20,Size(260, 60));
	//lblProductName->setScale(1.5f*generalscalefactor);
	//lblProductName->setAnchorPoint(Vec2(1.0f,1.0f));
	lblProductName->setColor(ccc3(72, 37, 0));
	float h1=card1->getPositionY();
	float h2= card1->getContentSize().height;

	lblProductName->setPosition(Vec2(visiableSize.width / 2 +64 * generalscalefactor,   visiableSize.height/2 -70 *generalscalefactor));
	addChild(lblProductName,-1,2001);

	  Sprite *game_buynow = Sprite::create("public/buy_now_btn.png");
	Sprite *game_buynow_a = Sprite::create("public/buy_now_btn.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game_buynow_a->setPositionY(5);
	auto menuItem__buynow  = MenuItemSprite::create(game_buynow,game_buynow_a,NULL,CC_CALLBACK_1(GameFailed::menuCallbackBuy, this));
	menuItem__buynow->setPosition(Vec2(visiableSize.width / 2  + 50 *generalscalefactor , visiableSize.height / 2 - generalscalefactor*146));

	auto menu_buynow = Menu::create(menuItem__buynow,NULL);
	menu_buynow->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu_buynow,1);
		

		
		Sprite *game_shared = Sprite::create("public/share_btn.png");
		Sprite *game_shared_a = Sprite::create("public/share_btn.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
		game_shared_a->setPositionY(5);
		auto menuItem__shared  = MenuItemSprite::create(game_shared,game_shared_a,NULL,CC_CALLBACK_1(GameFailed::menuCallbackShare, this));
		menuItem__shared->setPosition(Vec2(visiableSize.width/2 + 160 *generalscalefactor , visiableSize.height / 2 - generalscalefactor*146));

		auto menu_shared = Menu::create(menuItem__shared,NULL);
		menu_shared->setPosition(Point(origin.x ,origin.y));
		this->addChild(menu_shared,1);
		

		Sprite *game7 = Sprite::create("public/contiune_btn.png");
		Sprite *game7_a = Sprite::create("public/contiune_btn.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
		game7_a->setPositionY(5);
		auto menuItem_7  = MenuItemSprite::create(game7,game7_a,NULL,CC_CALLBACK_1(GameFailed::menuCallback, this));
		menuItem_7->setPosition(Vec2(visiableSize.width/2 - 260 *generalscalefactor , visiableSize.height / 2 - generalscalefactor*160));

		auto menu7 = Menu::create(menuItem_7,NULL);
		menu7->setPosition(Point(origin.x ,origin.y));
		this->addChild(menu7,1);


		Sprite *game_quit = Sprite::create("public/quit_btn.png");
		Sprite *game_quit_a = Sprite::create("public/quit_btn.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameWByName("button_play"));
		game_quit_a->setPositionY(5);
		auto menuItem__quit  = MenuItemSprite::create(game_quit,game_quit_a,NULL,CC_CALLBACK_1(GameFailed::menuCloseCallback, this));
		menuItem__quit->setPosition(Vec2(visiableSize.width/2 - 150 *generalscalefactor , visiableSize.height / 2 - generalscalefactor*160));

		auto menu_quit = Menu::create(menuItem__quit,NULL);
		menu_quit->setPosition(Point(origin.x ,origin.y));
		this->addChild(menu_quit,1);


		///////////////////////popup layer
*/

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	  std::string sCSD=app->GetLayoutString();
	 _rootNode = CSLoader::createNode("res/game_over/GameOverLayer"+sCSD+".csb");

	 //loginLayer=Layer::create();
	 addChild(_rootNode,1);


	 


	//Get Promotion 
	// Setting HTTP Request Header
 //   if(m_iGameFailed_Load==0){
 //       
 //       m_iGameFailed_Load=1;
	//HttpRequest* request = new HttpRequest();
 //   request->setUrl(PromotionWebGetURL);
 //   request->setRequestType(HttpRequest::Type::GET);
 //   request->setResponseCallback(CC_CALLBACK_2(GameFailed::onHttpRequestCompleted,this));
 //   request->setTag("GET test");
 //   cocos2d::network::HttpClient::getInstance()->send(request);
 //   request->release();
 //   }

	//ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivPromotionImg")); 
	//	 auto card1 = Sprite::create("match_game_img_1_b.jpg");
	// //card1->setPosition(Vec2(0, imgPromotion->getPositionY()+ imgPromotion->getContentSize().height/2));
	//	log("1=%f,%f,%f,%f",imgPromotion->getPositionX(),imgPromotion->getPositionY(),imgPromotion->getContentSize().width,imgPromotion->getContentSize().height);
	//	card1->setAnchorPoint(Vec2(0,0)); 
	//card1->setPosition(Vec2(0, 0));
	// //float zoom_x = visiableSize.width *0.5 / card1->getContentSize().width;
	//// float zoom_y = visiableSize.height * 0.5 / card1->getContentSize().height;
	// card1->setScale(0.38f);
	// //card1->setScaleY(zoom_y);

	//  imgPromotion->addChild(card1,2,1001);

	m_PrmtPrdct = app->getPromoteProduct();

	if(m_PrmtPrdct.m_iIsLoad == 0){
        
		m_PrmtPrdct.m_iIsLoad=1;
		HttpRequest* request = new HttpRequest();
		request->setUrl(PromotionWebGetURL);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(GameFailed::onHttpRequestCompleted,this));
		request->setTag("GET test");
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
    }

	if(_rootNode){
        auto img=_rootNode->getChildByName("ivPromotionImg");
        if(img){
            ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivPromotionImg"));
			auto card1 = Sprite::create(m_PrmtPrdct.m_strImage.c_str());
			card1->setAnchorPoint(Vec2(0,0)); 
			card1->setPosition(Vec2(0, 0));
			card1->setScale((imgPromotion->getBoundingBox().size.width / card1->getContentSize().width));
			imgPromotion->addChild(card1,2,1001);
        }
    }
    if(_rootNode && _rootNode->getChildByName("lblProductName")){
        Text* lblProductName=dynamic_cast<Text*>(_rootNode->getChildByName("lblProductName"));// (CCLabelTTF*)this->getChildByTag(2001);
        lblProductName->setString(m_PrmtPrdct.m_strProductName);
    }


	auto btnContine = dynamic_cast<Button*>(_rootNode->getChildByName("btnContine"));
	btnContine->setTag(1001);
	btnContine->addTouchEventListener(CC_CALLBACK_2(GameFailed::touchEvent, this));	

	auto btnQuit = dynamic_cast<Button*>(_rootNode->getChildByName("btnQuit"));
	btnQuit->setTag(1002);
	btnQuit->addTouchEventListener(CC_CALLBACK_2(GameFailed::touchEvent, this));	

	auto btnBuyNow = dynamic_cast<Button*>(_rootNode->getChildByName("btnBuyNow"));
	btnBuyNow->setTag(1003);
	btnBuyNow->addTouchEventListener(CC_CALLBACK_2(GameFailed::touchEvent, this));	

	auto btnShare = dynamic_cast<Button*>(_rootNode->getChildByName("btnShare"));
	btnShare->setTag(1004);
	btnShare->addTouchEventListener(CC_CALLBACK_2(GameFailed::touchEvent, this));	


	//Show Total User Score
	long l_coins = app->GetUserTotalScore();
	char p[20];
	sprintf(p, "%ld", l_coins);
	string sTotal = p;

	auto lblReward = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblReward"));
	if (lblReward){
		lblReward->setString(sTotal);
	}


	Sprite* imgBackground = Sprite::create(WIN_BACKGROUND);
	imgBackground->setAnchorPoint(Vec2(0, 0));
	imgBackground->setPosition(Vec2(0, 0));
	this->addChild(imgBackground, -1, 222);

	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_TRY_AGAIN);
	}

    return true;
}

void GameFailed::touchEvent(Object* obj,::ui::Widget::TouchEventType eventType)  
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

		if(tag==1001){ //Continue
			/*
			auto scene = WelcomeScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
			*/
			//back2 game base on current game id
			if (app->iGameMode == MobileGameMode::Theme_Mode)
			{
				app->StartTheme();
			}
			else
			{
				if (app->GetNetWorkStatus() != NetworkType::NetworkTypeNone){
					std::string sGuid = app->GetUserGuid();
					log("GUID is %s", sGuid.c_str());
					if (sGuid.length() == 0){ // not a registered user
						if (m_iShowRegister == 5){
							MsgBoxType m_type = TP_Q_YESNO;;
							std::string title = "Tweebaa";
							std::string msg = MSG_REGISTER_MSG;
							std::string info = "\0";

							MsgBox::setMsgBox(m_type, &title, &msg, &info);
							auto msgbox = MsgBox::create();
							auto btnYES = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
							btnYES->setTitleFontSize(36);
							btnYES->setTitleText("YES");
							btnYES->addTouchEventListener(CC_CALLBACK_2(GameFailed::CallbackRegisterYes, this));
							auto btnNO = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
							btnNO->setTitleText("NOT NOW");
							btnNO->setTitleFontSize(36);
							btnNO->addTouchEventListener(CC_CALLBACK_2(GameFailed::CallbackRegisterNo, this));
							msgbox->setTag(MSGBOX_TAG);
							this->addChild(msgbox, 999);
							//m_iShowRegister = 0;
							break;
						}
						else{
							//m_iShowRegister++;
						}
					}
				}
				switch (m_iCurrentGameID){
				case 1:{
						   auto scene = FindDiffScene::create();
						   TransitionScene *transition = TransitionFade::create(1, scene);
						   Director::getInstance()->replaceScene(transition);
				}
					break;
				case 2:{
						   auto scene = ZoomGameScene::create();
						   TransitionScene *transition = TransitionFade::create(1, scene);
						   Director::getInstance()->replaceScene(transition);
				}
					break;
				case 3:{
						   auto scene = jigSawScene::create();
						   TransitionScene *transition = TransitionFade::create(1, scene);
						   Director::getInstance()->replaceScene(transition);
				}
					break;
				case 4:{
						   auto scene = GameScene::create();
						   TransitionScene *transition = TransitionFade::create(1, scene);
						   Director::getInstance()->replaceScene(transition);
				}
					break;
				case 5:{
						   auto scene = PriceMatchScene::create();
						   TransitionScene *transition = TransitionFade::create(1, scene);
						   Director::getInstance()->replaceScene(transition);
				}
					break;
				case 6:
				{
						  auto scene = MultiPriceMatchScene::create();
						  TransitionScene *transition = TransitionFade::create(1, scene);
						  Director::getInstance()->replaceScene(transition);
				}
				}
			}
		}
		if(tag==1002){ // Quit

			/*
			AppDelegate *app = (AppDelegate*)Application::getInstance();
			 std::string sCSD=app->GetLayoutString();
			_quitNode = CSLoader::createNode("res/quit_game/QuitGameLayer"+sCSD+".csb");

			this->addChild(_quitNode,15,4001);

			auto btnNo = dynamic_cast<Button*>(_quitNode->getChildByName("btnNo"));
			btnNo->setTag(2001);
			btnNo->setTitleText("No");
			btnNo->addTouchEventListener(CC_CALLBACK_2(GameFailed::touchEvent, this));	

			auto btnYes = dynamic_cast<Button*>(_quitNode->getChildByName("btnYes"));
			btnYes->setTag(2002);
			btnYes->setTitleText("Yes");
			btnYes->addTouchEventListener(CC_CALLBACK_2(GameFailed::touchEvent, this));	
			*/

			//MsgBoxType m_type = TP_Q_YESNO;
			//std::string title = MSG_QUIT_TITLE;
			//std::string msg = MSG_QUIT_MSG;

			//MsgBox::setMsgBox(m_type, &title, &msg);
			//auto msgbox = MsgBox::create();
			//msgbox->setTag(9000);
			//this->addChild(msgbox, 999);

			////Add Event to process
			//auto btnYes = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
			//auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
			//btnYes->addTouchEventListener(CC_CALLBACK_2(GameFailed::yesCallback, this));
			//btnNo->addTouchEventListener(CC_CALLBACK_2(GameFailed::noCallback, this));


			if (app->iGameMode == MobileGameMode::Theme_Mode)
			{
				auto scene = ThemeLayer::createScene();
				if (scene == nullptr) break;
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
			else
			{
				auto scene = WelcomeScene::create();
				if (scene == nullptr) break;
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}

		}
		if(tag==1003){ //Buy Now
			/*
			auto scene = CheckoutScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);*/
			//Application::getInstance()->openURL("https://www.tweebaa.com/Product/prdSaleAll.aspx");
			std::string str = m_PrmtPrdct.m_strProductURL;
			str.append(ACTION_BUYNOW);
			Application::getInstance()->openURL(str);
		}
		if(tag==1004){ //share
			std::string str = m_PrmtPrdct.m_strProductURL;
			str.append(ACTION_SHARE);
			Application::getInstance()->openURL(str);
	
			//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			//	CCDictionary *content = CCDictionary::create();
			//	//content -> setObject(CCString::create(m_strProductName), "content");
			//	//content -> setObject(CCString::create(m_strImage), "image");
			//	content -> setObject(CCString::create(AppDelegate::m_PromoteProduct.m_strProductName), "content");
			//	content -> setObject(CCString::create(AppDelegate::m_PromoteProduct.m_strImage), "image");
			//	content -> setObject(CCString::create("Tweebaa App"), "title");
			//	content -> setObject(CCString::create("I win the game"), "description");
			//	content -> setObject(CCString::create("http://tweebaa.com"), "url");
			//	content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
			//	content -> setObject(CCString::create("http://tweebaa.com"), "siteUrl");
			//	content -> setObject(CCString::create("Tweebaa"), "site");
			//	//content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
			//	content -> setObject(CCString::create("extInfo"), "extInfo");
   // 
			//	C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, GameFailedScene_shareResultHandler);
	
			//#endif

		}
		if(tag==2001){
			/*
			Layer* background =(Layer*)this->getChildByTag(4001);
			background->removeFromParentAndCleanup(true);
			*/
			_quitNode->removeFromParentAndCleanup(true);
		}
		if(tag==2002){
			/*
		    Director::getInstance()->end();

			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
			#endif
			*/
			auto scene = WelcomeScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		break;
	}

	//GameFailed::showRegister();


		

}

void GameFailed::onHttpRequestShareCompleted(HttpClient *sender, HttpResponse *response)
{
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
		//StartGame();
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				CCDictionary *content = CCDictionary::create();
				content -> setObject(CCString::create(m_strProductName), "content");
				content -> setObject(CCString::create(m_strImage), "image");
				content -> setObject(CCString::create("Tweebaa App"), "title");
				content -> setObject(CCString::create("I win the game"), "description");
				content -> setObject(CCString::create("https://tweebaa.com/Product/saleBuy.aspx?id=3d7aeec0-5ece-4fad-975c-a58ef9464af1"), "url");
				content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
				content -> setObject(CCString::create("https://tweebaa.com/Product/presaleBuy.aspx?id=cbda0ab3-6b5d-4e58-9439-8c3fb09cf562"), "siteUrl");
				content -> setObject(CCString::create("tweebaa"), "site");
				//content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
				content -> setObject(CCString::create("extInfo"), "extInfo");
    
				C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, GameFailedScene_shareResultHandler);
	
	#endif

}
void GameFailed::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
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
		//StartGame();
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	/*/
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
    }
    printf("\n");
	*/

	//get the xml
	char sXml[400];

	//printf("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		sXml[i] = (*buffer)[i];
	}
	sXml[buffer->size()] = '\0';
	/*
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml,sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppPromotion");

	std::string strError=nodes.child_value("error");
	*/
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	XMLError errorId = pDoc->Parse((const char*)sXml);
	if (errorId == 0){
		//app->rootRound = pDoc->RootElement();
		XMLElement *rootEle = pDoc->RootElement();
		XMLElement *idError = rootEle->FirstChildElement();
		/*
	- <mobileAppPromotion>
  <error>0</error> 
  <Product_Guid>df8630a7-cb30-492b-b86b-f0fd44aa4b92</Product_Guid> 
  <Promote_Text>Wonder Cap</Promote_Text> 
  <Promote_Image>https://tweebaa.com/UploadImg/big/20150606/20150606130933_9493.jpg</Promote_Image> 
  </mobileAppPromotion>	
		*/

		int e1 = atoi(idError->GetText());
		if (e1 == 0){
			XMLElement *Product_Guid = idError->NextSiblingElement();

			if (Product_Guid && Product_Guid->GetText()){
				m_strProductGUID = Product_Guid->GetText();
			}
			else{
				m_strProductGUID = "";
			}
			XMLElement *Promote_Text = Product_Guid->NextSiblingElement();

			if (Promote_Text && Promote_Text->GetText()){
				m_strProductName = Promote_Text->GetText();
			}
			else{
				m_strProductName = "";
			}

			XMLElement *Promote_Image = Promote_Text->NextSiblingElement();

			if (Promote_Image && Promote_Image->GetText()){
				m_strImage = Promote_Image->GetText();
			}
			else{
				m_strImage = "";
			}



			//get user guid


			//get it
			//string s=CCUserDefault::sharedUserDefault()->getStringForKey("UserGuid");
			if (m_strImage.length() > 0){
                if(m_iCurrentGameID==2 || m_iCurrentGameID==6){
                    //Add by Long As this two game have some problem 
                }else{
				HttpRequest* request = new HttpRequest();
				char p[200];
				strcpy(p, m_strImage.c_str());
				request->setUrl(p);
				request->setRequestType(HttpRequest::Type::GET);
				request->setResponseCallback(CC_CALLBACK_2(GameFailed::onHttpLoadImageCompleted, this));
				//request->setResponseCallback(this, CC_CALLBACK_2(GameWin::onHttpLoadImageCompleted));
				HttpClient::getInstance()->send(request);
				request->release();
                }
                
			}
		}
	}
		delete pDoc;
	/*
	With CCUserDefault you can store integers, floats, doubles, strings, and boolean with just one simple call per data type. For example:
	*/
	/*
	CCUserDefault::sharedUserDefault()->setIntegerForKey(levelsCompleted, _levelsCompleted);
	CCUserDefault::sharedUserDefault()->flush();
	*/

}

void GameFailed::onHttpLoadImageCompleted(HttpClient *sender, HttpResponse *response){
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
		//StartGame();
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	Image* image = new Image();
    image->initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());

    Texture2D* texture = new Texture2D();
    texture->initWithImage(image);
    if(_rootNode){
        auto img=_rootNode->getChildByName("ivPromotionImg");
        if(img){
            ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivPromotionImg"));
            Sprite* sprite = (Sprite*)imgPromotion->getChildByTag(1001);
            sprite->setTexture(texture);
        }
    }
    if(_rootNode && _rootNode->getChildByName("lblProductName")){
        Text* lblProductName=dynamic_cast<Text*>(_rootNode->getChildByName("lblProductName"));// (CCLabelTTF*)this->getChildByTag(2001);
        lblProductName->setString(m_strProductName);
    }
}

//Loading Call back
void GameFailed::loadingCallBack(Texture2D *texture){
	auto scene = ChooseLayer::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void GameFailed::menuCloseCallback(Ref* pSender)
{
	Size visiableSize = Director::getInstance()->getVisibleSize();
	

	Sprite* popupLayer = Sprite::create("quit_game/quit_game_bg.png");
	//popupLayer->autorelease();
	
	popupLayer->setAnchorPoint(Vec2(0.0f,1.0f)) ;
	popupLayer->setPosition(Vec2(0, visiableSize.height));
	addChild(popupLayer,15,4001);

	//float generalscalefactor  = visiableSize.width / popupLayer->getContentSize().width;
	  float x0=visiableSize.width / popupLayer->getContentSize().width;
	  float y0=visiableSize.height / popupLayer->getContentSize().height;
	  float generalscalefactor  =min(x0,y0);
	popupLayer->setScale(generalscalefactor);

	 CCLabelTTF* lblInfo= CCLabelTTF::create ("Are you sure you want to quit the game ? ", "bionic.fnt",30,Size(360, 100));
	//lblProductName->setScale(1.5f*generalscalefactor);
	//lblProductName->setAnchorPoint(Vec2(1.0f,1.0f));
	lblInfo->setColor(ccc3(72, 37, 0));


	lblInfo->setPosition(Vec2(visiableSize.width / 2- 164 * generalscalefactor,   visiableSize.height/2 -70 *generalscalefactor));
	popupLayer->addChild(lblInfo);

	Sprite *game_No = Sprite::create("public/no.png");
	Sprite *game_No_a = Sprite::create("public/no.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game_No_a->setPositionY(5);
	auto menuItem__No  = MenuItemSprite::create(game_No,game_No_a,NULL,CC_CALLBACK_1(GameFailed::menuCallbackPopupNo, this));
	menuItem__No->setPosition(Vec2(visiableSize.width / 2  - 230 *generalscalefactor , visiableSize.height / 2 - generalscalefactor*196));

	auto menu_No = Menu::create(menuItem__No,NULL);
	menu_No->setPosition(Point(0 ,0));
	popupLayer->addChild(menu_No);

		  Sprite *game_Yes = Sprite::create("public/yes.png");
	Sprite *game_Yes_a = Sprite::create("public/yes.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	game_Yes_a->setPositionY(5);
	auto menuItem__Yes  = MenuItemSprite::create(game_Yes,game_Yes_a,NULL,CC_CALLBACK_1(GameFailed::menuCallbackPopupYes, this));
	menuItem__Yes->setPosition(Vec2(visiableSize.width / 2  -60 *generalscalefactor , visiableSize.height / 2 - generalscalefactor*196));

	auto menu_Yes = Menu::create(menuItem__Yes,NULL);
	menu_Yes->setPosition(Point(0 ,0));
	popupLayer->addChild(menu_Yes);
	
	
}
void GameFailed::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);

		}	
		auto scene = WelcomeScene::create();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void GameFailed::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);

		}
		this->removeChildByTag(9000);
	}
}

void GameFailed::menuCallbackPopupNo(Ref *sender){
	Sprite* background =(Sprite*)this->getChildByTag(4001);
	background->removeFromParentAndCleanup(true);
}

void GameFailed::menuCallbackPopupYes(Ref *sender){
	  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void GameFailed::menuCallbackBuy(Ref *sender) {

	auto scene = CheckoutScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);

}

void GameFailed::menuCallbackShare(Ref *sender) {

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				CCDictionary *content = CCDictionary::create();
				content -> setObject(CCString::create(m_strProductName), "content");
				content -> setObject(CCString::create(m_strImage), "image");
				content -> setObject(CCString::create("Tweebaa App"), "title");
				content -> setObject(CCString::create("I win the game"), "description");
				content -> setObject(CCString::create("https://tweebaa.com/Product/saleBuy.aspx?id=3d7aeec0-5ece-4fad-975c-a58ef9464af1"), "url");
				content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
				content -> setObject(CCString::create("https://tweebaa.com/Product/presaleBuy.aspx?id=cbda0ab3-6b5d-4e58-9439-8c3fb09cf562"), "siteUrl");
				content -> setObject(CCString::create("tweebaa"), "site");
				//content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
				content -> setObject(CCString::create("extInfo"), "extInfo");
    
				C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, GameFailedScene_shareResultHandler);
	
	#endif
}

void GameFailed::menuCallback(Ref *sender) {

	//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}


void GameFailed::showRegister(){

	auto scene = RegisterView::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->pushScene(transition);

}

void GameFailed::CallbackRegisterYes(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}

		GameFailed::showRegister();
	}
}

void GameFailed::CallbackRegisterNo(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_BUTTON_CLICK);
	}

	switch (m_iCurrentGameID){
			case 1:{
				auto scene = FindDiffScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
				   break;

			case 2:{
				auto scene = ZoomGameScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
				   break;
			case 3:{


				auto scene = jigSawScene::create();

				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
				   break;
			case 4:{
				auto scene = GameScene::create();


				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
				   break;

			case 5:{
				auto scene = PriceMatchScene::create();


				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
				   break;
			case 6:
			{
				auto scene = MultiPriceMatchScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
		}			
		this->removeChildByTag(9000);
	}
}
