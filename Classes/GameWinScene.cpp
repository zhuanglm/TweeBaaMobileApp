#include "GameWinScene.h"
//#include "memory_match/memory_match_welcome.h"
#include "ChooseLayer.h"
//#include "ShareScene.h"
#include "CheckoutScene.h"
#include "WelcomeScene.h"
#include "C2DXShareSDK/C2DXShareSDK.h"
#include "global_const.h"
#include "utilits/pugixml.hpp"
#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ZhuanPanSystem/TurntableSystem.h"

#include "memory_match/GameScene.h"
#include "jigsaw/jigSawScene.h"
#include "PriceMatch/PriceMatchScene.h"
#include "find_diff/FindDiffScene.h"
#include "zoom/ZoomGameScene.h"
#include "MultiPriceMatch/MultiPriceMatchScene.h"
#include <cocostudio/CocoStudio.h>
#include "Msgbox/Dialogbox.h"
#include "Msgbox/MsgBox.h"
#include "MemberCenter/RegisterView.h"

USING_NS_CC;
using namespace ui; 
using namespace cn::sharesdk;

std::string m_strProductGUID;
std::string m_strProductName;
std::string m_strImage;
std::string m_strProductURL;

int m_iIsLoad=0;
bool m_BTNEnabled = false;
int m_iLocal[6] = { 0, 0, 0, 0, 0, 0 };
int iReward;
int iStar;

void Gamewin_shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
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

Scene* GameWin::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameWin::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
void GameWin::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		auto scene = WelcomeScene::create();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void GameWin::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {
		this->removeChildByTag(9000);
	}
}

void GameWin::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    
	
	auto node =(Node*) this->getChildByTag(CHILD_TURNTABLE);
	if (node != nullptr) {
		return;
	}

	AppDelegate *app = (AppDelegate*)Application::getInstance();

	switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  

	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_BUTTON_CLICK);

	}
		if(tag==1001){ //Continue

			if(app->GetNetWorkStatus() != NetworkType::NetworkTypeNone){
				std::string sGuid = app->GetUserGuid();
				log("GUID is %s", sGuid.c_str());
				if (sGuid.length() == 0){ // not a registered user
					if (m_iShowRegister == 5){
						MsgBoxType m_type = TP_Q_YESNO;;
						std::string title="Tweebaa";
						std::string msg = MSG_REGISTER_MSG;
						std::string info = "\0";
				
						MsgBox::setMsgBox(m_type, &title, &msg, &info);
						auto msgbox = MsgBox::create();
						auto btnYES = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
						btnYES->setTitleFontSize(36);
						btnYES->setTitleText("YES");
						btnYES->addTouchEventListener(CC_CALLBACK_2(GameWin::CallbackRegisterYes, this));
						auto btnNO = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
						btnNO->setTitleText("NOT NOW");
						btnNO->setTitleFontSize(36);
						btnNO->addTouchEventListener(CC_CALLBACK_2(GameWin::CallbackRegisterNo, this));
						msgbox->setTag(MSGBOX_TAG);
						this->addChild(msgbox, 999);
						//m_iShowRegister = 0;
						break;
					}else{
						//m_iShowRegister++;
					}
				}
			}

			if(m_iCurrentGameID == 1){
				auto scene = FindDiffScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}else if(m_iCurrentGameID == 2){
				auto scene = ZoomGameScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}else if (m_iCurrentGameID == 3){
				auto scene = jigSawScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}else if (m_iCurrentGameID == 4){
				auto scene = GameScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}else if (m_iCurrentGameID == 5){
				auto scene = PriceMatchScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}else if (m_iCurrentGameID == 6){
				auto scene = MultiPriceMatchScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}else{
				auto scene = WelcomeScene::create();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);			
			}


			//if(m_iCurrentGameID == 3){
			//	auto scene = jigSawScene::create();
			//	//auto scene = LevelSelectLayer::createScene();
			//	TransitionScene *transition = TransitionFade::create(1, scene);
			//	Director::getInstance()->replaceScene(transition);
			//}
			//if(m_iCurrentGameID == 4){
			//	auto scene = GameScene::create();
			//	//auto scene = LevelSelectLayer::createScene();
			//	TransitionScene *transition = TransitionFade::create(1, scene);
			//	Director::getInstance()->replaceScene(transition);
			//}
			//if(m_iCurrentGameID == 5){
			//	auto scene = PriceMatchScene::create();
			//	//auto scene = LevelSelectLayer::createScene();
			//	TransitionScene *transition = TransitionFade::create(1, scene);
			//	Director::getInstance()->replaceScene(transition);
			//}


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
			btnNo->addTouchEventListener(CC_CALLBACK_2(GameWin::touchButton, this));	

			auto btnYes = dynamic_cast<Button*>(_quitNode->getChildByName("btnYes"));
			btnYes->setTag(2002);
			btnYes->setTitleText("Yes");
			btnYes->addTouchEventListener(CC_CALLBACK_2(GameWin::touchButton, this));	
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
			//btnYes->addTouchEventListener(CC_CALLBACK_2(GameWin::yesCallback, this));
			//btnNo->addTouchEventListener(CC_CALLBACK_2(GameWin::noCallback, this));

			auto scene = WelcomeScene::create();
			if(scene == nullptr) break;

			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);

		}
		if(tag==1003){ //Buy Now
			/*
			auto scene = CheckoutScene::create();
			//scene->sProductID= m_strProductGUID;
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
			//	content -> setObject(CCString::create(m_strProductName), "content");
			//	content -> setObject(CCString::create(m_strImage), "image");
			//	content -> setObject(CCString::create("Tweebaa App"), "title");
			//	content -> setObject(CCString::create("I win the game"), "description");
			//	content -> setObject(CCString::create("http://tweebaa.com"), "url");
			//	content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
			//	content -> setObject(CCString::create("http://tweebaa.com"), "siteUrl");
			//	content -> setObject(CCString::create("Tweebaa"), "site");
			//	//content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
			//	content -> setObject(CCString::create("extInfo"), "extInfo");
   // 
			//	C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, Gamewin_shareResultHandler);
	
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

}
// on "init" you need to initialize your instance
bool GameWin::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
   
    Size visiableSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	  std::string sCSD=app->GetLayoutString();
	 _rootNode = CSLoader::createNode("res/game_passed/GamePassLayer"+sCSD+".csb");

	 //loginLayer=Layer::create();
	 addChild(_rootNode,1);

	 /*
	LayerColor *layer1 = LayerColor::create(Color4B(0, 0, 0, 180));
	TurntableSystem* price =new TurntableSystem(layer1);
	layer1->setTag(CHILD_TURNTABLE);
	addChild(layer1,99);*/
	

	/*
	cocostudio::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Animation0.png", "Animation0.plist", "Animation.ExportJson");
	cocostudio::Armature *armature = cocostudio::Armature::create("Animation");
	armature->getAnimation()->playByIndex(0);
	armature->setScale(0.5f);
	armature->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.5));
	this->addChild(armature);
	
	*/

	 /*
	cocostudio::Armature* armature;
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/coin/coin.ExportJson");
	armature = cocostudio::Armature::create("coin");
	if (armature){
		armature->setPosition(ccp(visiableSize.width * 0.8, visiableSize.height * 0.5));
		armature->getAnimation()->playByIndex(0);
		armature->setScale(0.25);
		this->addChild(armature);
	}
	*/
	


	//Get Promotion 
	// Setting HTTP Request Header
 //   if(m_iIsLoad==0){
 //       m_iIsLoad=1;
	//HttpRequest* request = new HttpRequest();
 //   request->setUrl(PromotionWebGetURL);
 //   request->setRequestType(HttpRequest::Type::GET);
 //   request->setResponseCallback(CC_CALLBACK_2(GameWin::onHttpRequestCompleted,this));
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
	// //card1->setScale(0.055f);
	// //card1->setScaleY(zoom_y);
	//card1->setScale(0.38f);
	//  imgPromotion->addChild(card1,2,1001);

	m_PrmtPrdct = app->getPromoteProduct();

	if(m_PrmtPrdct.m_iIsLoad == 0){
        
		m_PrmtPrdct.m_iIsLoad=1;
		HttpRequest* request = new HttpRequest();
		request->setUrl(PromotionWebGetURL);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(GameWin::onHttpRequestCompleted,this));
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
	btnContine->addTouchEventListener(CC_CALLBACK_2(GameWin::touchButton, this));	

	auto btnQuit = dynamic_cast<Button*>(_rootNode->getChildByName("btnQuit"));
	btnQuit->setTag(1002);
	btnQuit->addTouchEventListener(CC_CALLBACK_2(GameWin::touchButton, this));	

	auto btnBuyNow = dynamic_cast<Button*>(_rootNode->getChildByName("btnBuyNow"));
	btnBuyNow->setTag(1003);
	btnBuyNow->addTouchEventListener(CC_CALLBACK_2(GameWin::touchButton, this));	

	auto btnShare = dynamic_cast<Button*>(_rootNode->getChildByName("btnShare"));
	btnShare->setTag(1004);
	btnShare->addTouchEventListener(CC_CALLBACK_2(GameWin::touchButton, this));	


	//Show Total User Score
	long l_coins = app->GetUserTotalScore();
	char p[20];
	sprintf(p, "%ld", l_coins);
	string sTotal = p;
	
	iReward = app->getReward();
	iStar = app->getStar();
	char p1[20];
	sprintf(p1, "%ld", iReward);
	string sReward = p1;
	app->setReward(0,0);

	auto lblReward = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblReward"));
	if (lblReward)
	{
		lblReward->setString(sReward);
	}
	auto lblScore = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblScore"));
	if (lblReward)
	{
		lblScore->setString(sTotal);
	}


	//_rootNode->getChildByName("Img_Box")->setVisible(false);
	auto imgStar = dynamic_cast<ImageView*>(_rootNode->getChildByName(WIN_STAR_FRAME));	
	if (imgStar != nullptr) {
		_rootNode->getChildByName(WIN_STAR_BOX)->setVisible(false);
		if (iStar == 3){
			imgStar->loadTexture(WIN_STAR_3);
		}
		else if (iStar == 2){
			imgStar->loadTexture(WIN_STAR_2);
		}
		else if (iStar == 1){
			imgStar->loadTexture(WIN_STAR_1);
		}
	}
	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_WIN);
	}


		// start ansyc method load the atlas.png
	//Director::getInstance()->getTextureCache()->addImageAsync("MatchBg.png", CC_CALLBACK_1(GameWin::loadingCallBack, this));
	
	/*auto card1 = Sprite::create("match_game_img_1_b.jpg");
	// card1->setPosition(Vec2(visiableSize.width / 2 +88 * generalscalefactor,   visiableSize.height/2 +50 *generalscalefactor));
	 //float zoom_x = visiableSize.width *0.5 / card1->getContentSize().width;
	// float zoom_y = visiableSize.height * 0.5 / card1->getContentSize().height;
	 card1->setScale(150/800);
	 //card1->setScaleY(zoom_y);
	 
	 Image* image = new Image();
	 image->s
	 Texture2D* texture = new Texture2D();
    
	Sprite* sprite = dynamic_cast<Sprite*>(_rootNode->getChildByName("spPromotionImg"));  //spPromotionImg
	sprite->initWithFile("match_game_img_1_b.jpg");

	  this->addChild(card1,1,1001);
	*/


	Sprite* imgBackground = Sprite::create(WIN_BACKGROUND);

	imgBackground->setAnchorPoint(Vec2(0, 0));
	imgBackground->setPosition(Vec2(0, 0));
	this->addChild(imgBackground, -1, 222); 


	syncFlow();//sync with server
	app->PlayEffect(SOUND_GAME_PASS);

    return true;
}


void GameWin::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
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
		//SaveReward2Local();
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


	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	XMLError errorId = pDoc->Parse((const char*)sXml);
	if (errorId == 0){
		//app->rootRound = pDoc->RootElement();
		XMLElement *rootEle = pDoc->RootElement();
		XMLElement *idError = rootEle->FirstChildElement();


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
			if (m_strImage.length() > 0){
				HttpRequest* request = new HttpRequest();
				char p[200];
				strcpy(p,m_strImage.c_str());
				request->setUrl(p);
				request->setRequestType(HttpRequest::Type::GET);
				request->setResponseCallback(CC_CALLBACK_2(GameWin::onHttpLoadImageCompleted,this));
				//request->setResponseCallback(this, CC_CALLBACK_2(GameWin::onHttpLoadImageCompleted));
				HttpClient::getInstance()->send(request);
				request->release();
			}
		}
	}
	delete pDoc;

}

void GameWin::onHttpLoadImageCompleted(HttpClient *sender, HttpResponse *response){
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
		//SaveReward2Local();
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	Image* image = new Image();
    image->initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());

	
    Texture2D* texture = new Texture2D();
    texture->initWithImage(image);
    
    if(_rootNode && _rootNode->getChildByName("ivPromotionImg")){
        ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivPromotionImg"));
        Sprite* sprite = (Sprite*)imgPromotion->getChildByTag(1001);
        sprite->setTexture(texture);
    }
    if(_rootNode && _rootNode->getChildByName("lblProductName")){
        Text* lblProductName=dynamic_cast<Text*>(_rootNode->getChildByName("lblProductName"));// (CCLabelTTF*)this->getChildByTag(2001);
        lblProductName->setString(m_strProductName);
    }
}

//Loading Call back
void GameWin::loadingCallBack(Texture2D *texture){
	auto scene = ChooseLayer::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void GameWin::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameWin::menuCallbackQuitGame(Ref *sender) {
	
}

void GameWin::menuCallbackPopupNo(Ref *sender){

}

void GameWin::menuCallbackPopupYes(Ref *sender){

}
void GameWin::menuCallbackBuy(Ref *sender) {

	//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());

}

void GameWin::menuCallbackShare(Ref *sender) {

}

void GameWin::menuCallback(Ref *sender) {

	//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());

}


void GameWin::syncFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string  strFlow= updateFlow();//read local and add current

	NetworkType Networ = app->GetNetWorkStatus();
	if (Networ != NetworkTypeNone)
	{
		if (m_UserID > 0 && strFlow.length()>0)
		{
			_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
			HttpRequest* request = new HttpRequest();
			request->setUrl(UCAPIWebPostURL);
			request->setRequestType(HttpRequest::Type::POST);
			request->setResponseCallback(CC_CALLBACK_2(GameWin::onSyncCompleted, this));

			//char p[10];
			//sprintf(p, "%s", strFlow);
			std::string sPost = "action=save_user_info&EncryptData=";

			long lUserDiff = app->GetUserScoreDiff();
			int iLevel = app->GetUserLevel();


			char p1[10];
			char p2[10];
			char p3[10];
			sprintf(p1, "%d", lUserDiff);
			sprintf(p2, "%d", iLevel);
			string s1 = p1;
			string s2 = p2;

			//std::string sPost = "action=sync_point&EncryptData=";
			int iTools = app->GetToolsDiff();
			sprintf(p3, "%d", iTools);
			string s4 = p3;

			string s3 = "score = " + strFlow + "&local_point = " + s1 + "&LV = " + s2 + "&UserID = " + userInfo.strUserGuid+"&VirtualPops=" + s4;
			s3 = app->encode_data_for_http(s3);
			sPost = sPost + s3;
			sPost = sPost + "&" + app->GetAccessString();

			const char* postData = sPost.c_str();
			request->setRequestData(postData, strlen(postData));
			request->setTag("save_ranking");
			cocos2d::network::HttpClient::getInstance()->send(request);
			request->release();
		}
		else updateLocalFlow();//not login or read local error
	}
	else
	{
		updateLocalFlow();//no network
	}
}

std::string GameWin::updateFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string strLocal=DataAccess::getParamValue("LocalFlow");
	if (strLocal.length() > 0)
	{
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
				m_iLocal[i] = atoi(sSub.c_str());
			}
			else return "\0";//wrong 
		}
		m_iLocal[m_iCurrentGameID - 1] += iReward;
	}
	else  m_iLocal[m_iCurrentGameID - 1] = iReward;
	char cFlow[20] = "\0";
	std::string strFlow = "";
	for (int i = 0; i < 6; i++)
	{
		if (m_iLocal[i]>0)
		{
			sprintf(cFlow, "%d:%d,\0", i + 1, m_iLocal[i]);
			strFlow += cFlow;
		}
	}
	return strFlow;
}

void GameWin::updateLocalFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	char cFlow[20] = "\0";
	std::string strLocal = "";
	for (int i = 0; i < 6; i++)
	{
		sprintf(cFlow, "%d,\0",  m_iLocal[i]);
		strLocal += cFlow;
	}
	strLocal = app->base64_xxtea_encode(strLocal); 
	DataAccess::setParamValue("LocalFlow", strLocal);
	DataAccess::setParamValue("SyncFlag", "1");
}

void GameWin::clearLocalFlow()
{
	DataAccess::setParamValue("LocalFlow", "\0");
}


void GameWin::onSyncCompleted(HttpClient *sender, HttpResponse *response){
	if (!response)
	{
		updateLocalFlow();
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
		updateLocalFlow();
		return;
	}
	if (statusCode != 200){
		log("response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		updateLocalFlow();
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
	int iScore = atoi(p);
	if (iScore==0)
	{
		//get TotalPoints
		 strResult = nodes.child_value("TotalPoints");
		 app->SaveUserScoreDiff(atoi(strResult.c_str()));
		//get TotalTools
		strResult = nodes.child_value("TotalTools");
		app->SaveUserToolsDiff(atoi(strResult.c_str()));
	}

}

void GameWin::showRegister(){

	auto scene = RegisterView::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->pushScene(transition);

}

void GameWin::CallbackRegisterYes(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}

		GameWin::showRegister();
	}
}

void GameWin::CallbackRegisterNo(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
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

