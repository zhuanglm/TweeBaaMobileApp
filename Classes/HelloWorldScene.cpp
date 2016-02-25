#include "HelloWorldScene.h"
//#include "memory_match/memory_match_welcome.h"
#include "ChooseLayer.h"
#include "global_const.h"
//#include "MemberCenter/RegisterTabScene.h"
#include "Login.h"
#include "cocostudio/CocoStudio.h"
#include "AppDelegate.h"
#include "theme/GameRound.h"
#include "ZhuanPanSystem/TurntableScene.h"
#include "LevelSelect/LevelSelectLayer.h"
#include "utilits/NetworkUtil.h"
#include "Msgbox/Dialogbox.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
   AppDelegate *app = (AppDelegate*)Application::getInstance();
   string sCSD=app->GetLayoutString();

	auto rootNode = CSLoader::createNode("res/MainScene"+sCSD+".csb");
		addChild(rootNode);
	


	//set time out
	//scheduleOnce(schedule_selector(HelloWorld::ShowSplash), 3.0f );
	//get them

	//play test
	/*
	Sprite* hello = Sprite::create("HelloWorld.png");
	hello->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	addChild(hello);


	Vector<SpriteFrame*> spriteFrameVec;
	auto spriteFrameCache = SpriteFrameCache::getInstance();

	// ÕâÀïÊ¹ÓÃplistÎÄ¼þ
	spriteFrameCache->addSpriteFramesWithFile("spot_hover.plist", "spot_hover.png");

	char path[256] = { 0 };
	for (int i = 1; i <= 6; ++i)
	{
		sprintf_s(path, "%d.png", i);

		SpriteFrame *pSpriteFrame = spriteFrameCache->getSpriteFrameByName(path);
		spriteFrameVec.pushBack(pSpriteFrame);
	}

	// 0.1ÄÇ¸ö²ÎÊý±ØÐëÉè¶¨£¬¿ÉÒÔÉè¶¨³ýÄ¬ÈÏÖµÒâÍâµÄÈÎºÎÖµ£¬Èç¹ûÄã²»Éè¶¨£¬¸ù±¾¾ÍÎÞ·¨²¥·ÅAnimate¶¯×÷
	// Cocos2d-xµÄ¿Ó»¹²»ÉÙ°¡£¬¸÷Î»Ðè½÷É÷°¡
	auto animation = Animation::createWithSpriteFrames(spriteFrameVec, 0.1);
	AnimationCache::getInstance()->addAnimation(animation, "dogs");

	auto sprite = Sprite::create();

	// ·½·¨1£º´ÓSpriteFrameCacheÖÐ¸ù¾ÝÃû×Ö»ñµÃ¶ÔÓ¦µÄ¾«ÁéÖ¡£¬ÔÙÉèÖÃ
	sprite->setSpriteFrame(spriteFrameCache->getSpriteFrameByName("1.png"));

	// ·½·¨2£º´ÓAnimationÖÐ»ñµÃ¶ÔÓ¦µÄÖ¡£¬ÔÙ½øÐÐÉèÖÃ
	//sprite->setDisplayFrameWithAnimationName("dogs", 0);
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(sprite, 1, 1);


	/*
		SpriteBatchNode* spritebatch = SpriteBatchNode::create("spot_hover.png");

		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("spot_hover.plist");

		auto Sprite1 = Sprite::createWithSpriteFrameName("HelloWorld.png");
		spritebatch->addChild(Sprite1);
		addChild(spritebatch);
*/
		/*
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spot_hover.plist");
		CCAnimation *bearWalkingAnimation = CCAnimation::create();


		CCSpriteFrame* sprite = CCSpriteFrameCache::sharedSpriteFrameCache()>spriteFrameByName("spot_hover.png");
			bearWalkingAnimation->addSpriteFrame(sprite);

		*/
	
	//NetworkType nt_type = NetworkUtil::getNetworkType();
	NetworkType nt_type = app->GetNetWorkStatus();

	//nt_type = NetworkType::NetworkTypeNone;

	//if (nt_type == NetworkType::NetworkTypeNone){

	//	DelayTime* delayAction = DelayTime::create(0.5f);
	//	CallFunc* moveCallback = CallFunc::create(this, callfunc_selector(HelloWorld::checknetwork));
	//	this->runAction(CCSequence::create(delayAction, moveCallback, NULL));
	//	return true;
	//} 
	//else{
		HttpRequest* request = new HttpRequest();
		string sUrl=GameAPIURL;
		sUrl=sUrl+"?action=load_theme";
		char url[100];
		strcpy(url,sUrl.c_str());
		request->setUrl(url);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted,this));
		request->setTag("Load Theme");
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	//}

	return true;
}

void HelloWorld::ShowSplash(float dt){
	StartGame();
}
void HelloWorld::AutoCheckVersion(){
	HttpRequest* request = new HttpRequest();
	request->setUrl(VersionCheckURL);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted,this));
	
	cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();

}
void HelloWorld::onHttpRequestLoadRoundCompleted(HttpClient *sender, HttpResponse *response)
{

    if (!response)
    {
		StartGame();
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
    //_labelStatusCode->setString(statusString);
    log("response code: %d", statusCode);   
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
		StartGame();
        return;
    }
	if(statusCode!=200){
		log("response code failed");
        log("error buffer: %s", response->getErrorBuffer());
		StartGame();
        return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	//int iSize = buffer->size()+1;
	char sXml[10000];
	
    //printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        sXml[i]=(*buffer)[i];
    }
	sXml[buffer->size()]='\0';
   // printf("\n");
	//get the theme xml
	log("get round return =%s",sXml);
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	XMLError errorId = pDoc->Parse((const char*)sXml); 
	if (errorId == 0){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		//app->rootRound = pDoc->RootElement();
		XMLElement *rootEle = pDoc->RootElement();
		XMLElement *idError = rootEle->FirstChildElement();


		int e1=atoi(idError->GetText());
		if(e1==0){
			XMLElement *elRound = idError->NextSiblingElement();
			
			XMLElement *gameEle = elRound->FirstChildElement();

			log("aa=%s",gameEle->GetText());
			while ( gameEle ) {
				GameRound* round=new GameRound();

				XMLElement *el_round_id = gameEle;//gameEle->FirstChildElement(); //<round_id>1</round_id>
				XMLElement *el_round_step = el_round_id->NextSiblingElement(); //<round_step>1</round_step>
				XMLElement *el_round_type = el_round_step->NextSiblingElement();
				XMLElement *el_round_info = el_round_type->NextSiblingElement();
				log("aa=%s",el_round_step->GetText());

				round->round_id = atoi(el_round_id->GetText());
				round->round_step = atoi(el_round_step->GetText());
				round->round_type = atoi(el_round_type->GetText());
				if(el_round_info && el_round_info->GetText()){
					round->round_info = el_round_info->GetText();
				}else{
					round->round_info = "";
				}
				log("aa=%s,%s,%s,%s",gameEle->GetText(),el_round_step->GetText(),el_round_type->GetText(), el_round_info->GetText());
				app->sp_GameRound.pushBack(round);

				// next one
				gameEle = el_round_info->NextSiblingElement();
			}

		}
		StartGame();
	}
	delete pDoc;
}
void HelloWorld::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{


    if (!response)
    {
		StartGame();
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
    //_labelStatusCode->setString(statusString);
    log("response code: %d", statusCode);   
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
		StartGame();
        return;
    }
	if(statusCode!=200){
		log("response code failed");
        log("error buffer: %s", response->getErrorBuffer());
		StartGame();
        return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	
	char sXml[300];
	
    //printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        sXml[i]=(*buffer)[i];
    }
	sXml[buffer->size()]='\0';
   // printf("\n");
	//get the theme xml
	log("get theme xml=%s",sXml);
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	XMLError errorId = pDoc->Parse((const char*)sXml); 
	if (errorId == 0){
		XMLElement *rootEle = pDoc->RootElement();

		XMLElement *idError = rootEle->FirstChildElement();


		int e1=atoi(idError->GetText());
		if(e1==0){
				AppDelegate *app = (AppDelegate*)Application::getInstance();
				XMLElement *imgTheme_id = idError->NextSiblingElement();
				app->iThemeID = atoi(imgTheme_id->GetText());
				XMLElement *imgTheme_name = imgTheme_id->NextSiblingElement();
				XMLElement *imgTheme_img = imgTheme_name->NextSiblingElement();
				log("Elements ID= %s IMAGE= %s", imgTheme_name->GetText(),imgTheme_img->GetText());
				app->m_strThemeImg= imgTheme_img->GetText();

				HttpRequest* request = new HttpRequest();
				string sUrl=GameAPIURL;
				sUrl=sUrl+"?action=load_round&theme_id="+imgTheme_id->GetText();
				char url[100];
				strcpy(url,sUrl.c_str());
				request->setUrl(url);
				request->setRequestType(HttpRequest::Type::GET);
				request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestLoadRoundCompleted,this));
				request->setTag("load_round");
				cocos2d::network::HttpClient::getInstance()->send(request);
				request->release();

		}
		/*
		while ( templateEle ) {
			XMLElement *idError = templateEle->FirstChildElement();
			int e1=atoi(idError->GetText());
			if(e1==0){
				XMLElement *imgTheme_name = idError->NextSiblingElement();
				XMLElement *imgTheme_img = imgTheme_name->NextSiblingElement();
				log("Elements ID= %s IMAGE= %s", imgTheme_name->GetText(),imgTheme_img->GetText());
			}

			// next one
			templateEle = templateEle->NextSiblingElement();
		}
		*/

	
	}else{
		StartGame();
	}
	delete pDoc;
}

void HelloWorld::StartGame(){

	//auto scene = ChooseLayer::createScene();
	//auto scene = RegisterTab::createScene();
	auto scene = ChooseLayer::createScene();
	//auto scene = TurntableScene::createScene();
	//auto scene = LevelSelectLayer::createScene();
	
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}
//delegate
void HelloWorld::onOk(CCAlert* pSender){
    CCLOG("ok");
}

void HelloWorld::onCancel(CCAlert* pSender){
    CCLOG("cancel");
	StartGame();
}
//Loading Call back
void HelloWorld::loadingCallBack(Texture2D *texture){
	auto scene = ChooseLayer::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onEnter()
{
    CCLOG("%s: FirstScene", __FUNCTION__);
    
    Layer::onEnter();

}
void HelloWorld::onEnterTransitionDidFinish(){
	Layer::onEnterTransitionDidFinish();

}


void HelloWorld::checknetworkdelay(float dt){
	//log("HelloWorld::checknetworkdelay-> checknetwork(); before");
	//MessageBox("HelloWorld::checknetworkdelay-> checknetwork(); ", "checknetworkdelay");
	////HelloWorld::checknetwork();
	//log("HelloWorld::checknetworkdelay-> checknetwork(); after");

}
void HelloWorld::checknetwork(){
		MessageBox(MSG_NETWORK_NONETWORK,"No Network");
		StartGame();
}
void HelloWorld::menuCallbackPopupNo(){
	//MessageBox("Don't download","menuCallbackPopupNo");
	auto scene = ChooseLayer::createScene();
	//auto scene = TurntableScene::createScene();
	//auto scene = LevelSelectLayer::createScene();
	
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);

	getEventDispatcher()->removeCustomEventListeners(APP_CE_NO);
	getEventDispatcher()->removeCustomEventListeners(APP_CE_YES);
}

void HelloWorld::menuCallbackPopupYes(){
	auto scene = ChooseLayer::createScene();
	//auto scene = TurntableScene::createScene();
	//auto scene = LevelSelectLayer::createScene();
	
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
	//MessageBox("Download files","menuCallbackPopupYes");
	getEventDispatcher()->removeCustomEventListeners(APP_CE_NO);
	getEventDispatcher()->removeCustomEventListeners(APP_CE_YES);
}
void HelloWorld::menuCallbackPopupOk(){
	auto scene = ChooseLayer::createScene();
	//auto scene = TurntableScene::createScene();
	//auto scene = LevelSelectLayer::createScene();
	
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
	//MessageBox("Download files","menuCallbackPopupYes");
	getEventDispatcher()->removeCustomEventListeners(APP_CE_OK);
	//getEventDispatcher()->removeCustomEventListeners(APP_CE_YES);
}

