#include "TwbIntroScene.h"
//#include "HelloWorldScene.h"
#include "ChooseLayer.h"
#include "Collage/CollageScene.h"
#include "utilits/NetworkUtil.h"
#include "AppDelegate.h"
#include "global_const.h"
#include "Gif/InstantGif.h"
#include "Gif/CacheGif.h"
#include "ZhuanPanSystem/TurntableSystem.h"
#include "ZhuanPanSystem/SpinLuckyLayer.h"
#include "utilits/VideoPlayer.h"
#include <stdio.h>
#include <stdlib.h>
#include "DataAccess/DataAccess.h"


//#include "befool/SimpleVideoEngine.h"
std::string sLastSpin="\0";
std::string sCurrent="\0";


TwbIntroScene::TwbIntroScene(){};

TwbIntroScene::~TwbIntroScene(){ };

int _count1;
int _count2;

bool TwbIntroScene::init(){
	bool bRet = false;

	char name[] = "product_videos/LogoVideo.gif\0";
	//name = FileUtils::getInstance()->fullPathForFilename(name.c_str());
	log("%s",name);
	_rootNode = this;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Size size = Director::getInstance()->getVisibleSize();
	auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
	videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
	videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	videoPlayer->setContentSize(Size(size.width, size.height));
	videoPlayer->setFullScreenEnabled(false);
	videoPlayer->setEnabled(false);
	videoPlayer->setTouchEnabled(false);
	videoPlayer->setTag(100);
	this->addChild(videoPlayer);
	if (videoPlayer)
	{
		videoPlayer->setFileName(VIDEO_INTRO);
		videoPlayer->play();
	}
	this->schedule(schedule_selector(TwbIntroScene::init2), 5.0f);
	//videoPlayer->addEventListener(CC_CALLBACK_2(TwbIntroScene::videoEventCallback, this));
#else
	auto gif2 = CacheGif::create(name);
	if (gif2)
	{
		log("got gif2\n");
		gif2->setPosition(Point(0, 0));
		gif2->setAnchorPoint(Point(0, 0));

		Size visibleSize = Director::getInstance()->getVisibleSize();
		gif2->setScale(visibleSize.width / 640);

		this->addChild(gif2);

		this->schedule(schedule_selector(TwbIntroScene::init2), 4.9f);
	}
	else
	{
		log("not got\n");
		this->schedule(schedule_selector(TwbIntroScene::init2), 0.1f);
	}
#endif
	bRet = true;
	//do{
		//_ttl_time = 0;
		//CC_BREAK_IF(!Scene::init());
		 //befool::SimpleVideoEngine::getShared()->playVideo("renai-circulation.mp4");




		/*
		_UINode = CSLoader::createNode(UINODE_NAME_CSB);
		//CC_ASSERT( _UINode != NULL, "_UINode is NULL!" );

		if (!_UINode) {
			bRet = false;
			break;
		}
		this->addChild(_UINode);
		*/

	//}while(0);
	return bRet;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void TwbIntroScene::videoEventCallback(cocos2d::Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
	if (eventType ==cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED)
	{
		_rootNode->schedule(schedule_selector(TwbIntroScene::init2),0.00001f);
	}
}
#endif
void TwbIntroScene::init2(float dt)
{
	bool bRet = false;
	this->unschedule(schedule_selector(TwbIntroScene::init2));
	do{
	_ttl_time = 0;
	CC_BREAK_IF(!Scene::init());

	_UINode = CSLoader::createNode(NAME_CSB);
	//CC_ASSERT( _UINode != NULL, "_UINode is NULL!" );

	if (!_UINode) {
	bRet = false;
	break;
	}
	this->addChild(_UINode);
	this->schedule(schedule_selector(TwbIntroScene::update), 0.01f);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	this->removeChildByTag(100);
#endif
	aSyncAnimation();
	DataAccess::initDB();
	DataAccess::loadProductList();
	//initTheme();

	}while(0);
	return ;
}
void TwbIntroScene::aSyncAnimation()
{
	ArmatureDataManager* admanager = ArmatureDataManager::getInstance();
	_count1 = 0;
	_count2 = 0;

	string strAni = sAni[_count1];
	strAni += "/";
	strAni += sAniName[_count1];
	strAni += ".ExportJson";
	admanager->addArmatureFileInfoAsync(strAni, this, schedule_selector(TwbIntroScene::loadAni));//ASync Armature for welcom scene

	string strIdle = sIdle[_count2];
	strIdle += "/";
	strIdle += sIdleName[_count2];
	strIdle += ".ExportJson";
	admanager->addArmatureFileInfoAsync(strIdle, this, schedule_selector(TwbIntroScene::loadIdle));//ASync Armature for welcom scene
	/*
	for (int i = 0; i < GAMG_COUNT; i++)
	{
		string strAni = sAni[i];
		strAni += "/";
		strAni += sAniName[i];
		strAni += ".ExportJson";
		log(strAni.c_str());
		admanager->addArmatureFileInfoAsync(strAni, this, schedule_selector(TwbIntroScene::loadAnimationComplete));//ASync Armature for welcom scene
	}
	for (int i = 0; i < GAMG_COUNT; i++)
	{
		string strIdle = sIdle[i];
		strIdle += "/";
		strIdle += sIdleName[i];
		strIdle += ".ExportJson";
		log("%s", strIdle.c_str());
		admanager->addArmatureFileInfoAsync(strIdle, this, schedule_selector(TwbIntroScene::loadAnimationComplete));//ASync Armature for welcom scene
	}*/
}

void TwbIntroScene::loadAni(float dt)
{
	log("load ani %d",_count1);
	_count1++;
	if (_count1 < GAMG_COUNT)
	{
		string strAni = sAni[_count1];
		strAni += "/";
		strAni += sAniName[_count1];
		strAni += ".ExportJson";
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(strAni, this, schedule_selector(TwbIntroScene::loadAni));
	}
}
void TwbIntroScene::loadIdle(float dt)
{
	log("load idle %d", _count1);
	_count2++;
	if (_count2 < GAMG_COUNT)
	{
		string strIdle = sIdle[_count2];
		strIdle += "/";
		strIdle += sIdleName[_count2];
		strIdle += ".ExportJson";
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(strIdle, this, schedule_selector(TwbIntroScene::loadIdle));
	}
}
/*
void TwbIntroScene::loadAnimationComplete(float dt)
{
	//log("%d", _count);
	//_count++;
}*/

void TwbIntroScene::menuCallbackPopupOk(){
	auto director = Director::getInstance();
	auto scene = CollageScene::createScene();
	director->replaceScene(scene);
	//getEventDispatcher()->removeCustomEventListeners(APP_CE_OK);
}
void TwbIntroScene::loadingCallBack(Texture2D *texture){
	//AtlasLoader::getInstance()->loadAtlas("zoom_atlas.txt", texture);

}
void TwbIntroScene::update(float dt) {
	_ttl_time = _ttl_time + 1;
	int ttlseconds = _ttl_time / 60;
	//int seconds = ttlseconds % 60;
	
	if (_count1 + _count2 >= 12){

		this->unschedule(schedule_selector(TwbIntroScene::update));
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		NetworkType nt_type = app->GetNetWorkStatus();
		if (nt_type == NetworkType::NetworkTypeNone){
			MessageBox(MSG_NETWORK_NONETWORK,"No Network");
		}

		m_UserID = UserDefault::getInstance()->getIntegerForKey("CurrentUserId", -1);
		//app->SyncPoint();

		//UserDefault *load = UserDefault::getInstance();
		//sLastSpin = load->getStringForKey("LastSpin");
		//string  sUsername = UserDefault::getInstance()->getStringForKey("tweebaa_username");


		if (m_UserID>0)
		{
			
			char *pszCurrTime = (char*)malloc(sizeof(char)* 20);
			memset(pszCurrTime, 0, sizeof(char)* 20);
			time_t now;
			time(&now);
			strftime(pszCurrTime, 20, "%Y/%m/%d %H:%M:%S", localtime(&now));
			sCurrent = pszCurrTime;
			sLastSpin = DataAccess::getParamValue("LastSpin","***************");
			if (strcmp(sLastSpin.substr(0, 10).c_str(), sCurrent.substr(0, 10).c_str()) != 0)
			{
				DataAccess::setParamValue("LastSpin", sCurrent);
				DoDailySpin();
				return;
			}
		}
		auto director = Director::getInstance();
		auto scene = ChooseLayer::createScene();
		director->replaceScene(scene);

		//auto director = Director::getInstance();
		//auto scene = ChooseLayer::createScene();
		//director->replaceScene(scene);
		//this->unschedule(schedule_selector(VideoScene::update));


	}
}


void TwbIntroScene::DoDailySpin()
{
	auto director = Director::getInstance();
	auto dailyspin = TurntableSystem::create();
	//auto introscene = Login::createScene();
	director->replaceScene(dailyspin);

}

void TwbIntroScene::initTheme()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	app->iThemeID = atoi(DataAccess::getParamValue("ThemeID", "1").c_str());
	app->iRound = atoi(DataAccess::getParamValue("RoundID", "1").c_str());
	app->iCurrentStep = atoi(DataAccess::getParamValue("StepID", "1").c_str());

	DataAccess::initTheme();
}