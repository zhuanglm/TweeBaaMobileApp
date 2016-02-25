#include "AppDelegate.h"
#include "HelloWorldScene.h"
//#include "ShareScene.h"
#include "theme/ThemeTextLayer.h"
#include "Collage/CollageScene.h"

#include "PriceMatch/PriceMatchScene.h"
#include "find_diff/FindDiffScene.h"
#include "zoom/ZoomGameScene.h"
#include "memory_match/GameScene.h"
#include "jigsaw/jigSawScene.h"
#include "MultiPriceMatch/MultiPriceMatchScene.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "TwbIntroScene.h"
#include "utilits/DataCenter.h"
#include "utilits/pugixml.hpp"

//#include "Msgbox\MessageScene.h"
//#include "utilits/tea.h"
//#include "utilits/util.h"
//#include "utilits/RSA_Crypt.h"
//#include "global_const.h"
#include "utilits/xxtea.h"
#include "utilits/base64.h"
#include "utilits/VideoPlayer.h"
#include "utilits/DailyCheckInLayer.h"
#include "Login.h"

#define GameAPIPOSTURL  "http://67.224.94.82/mobileApp/GameAPI.ashx"
//#define GameAPIPOSTURL  "http://localhost:2339/mobileApp/GameAPI.ashx"

USING_NS_CC;
using namespace cn::sharesdk;

int m_iCurrentGameID;
int m_iCurrentGameLevel; //current game level, < max game levels
long m_iTotalPassedGameLevel; //total levels passed, accumulated, might greater than max levels, 
long m_iUserTotalScore;
int m_iBGMFlag;
int m_iToolNum;
int m_Reward;
int m_Star;
int m_Refresh;
PromoteProduct m_PromoteProduct;

//数据加密
//int key1[10] = { 23, 12, 34, 23, 45, 66, 23, 9, 434, 12 };// , 423, 565, 32, 5, 34, 34, 23, 12, 23, 5423, 23, 23, 65, 34, 234, 64, 34, 65, 34, 63, 34, 234, 364, 34, 643, 34, 634, 23, 64, 88, 12, 33, 44, 4, 8, 9, 56, 234, 34, 34, 2, 21, 33 };
//int key2[10] = { 9, 23, 23, 5, 2, 42, 54, 57, 7, 5 };// 34, 33, 444, 346, 745, 342, 34, 64, 34, 34, 23, 64, 34, 346, 546, 436, 346, 73, 346, 34623, 25, 23, 546, 2345, 6243, 234, 234, 234, 64, 64, 64, 32, 23, 55, 234, 123, 53, 12, 454, 23, 677, 234, 666, 12, 33, 5, 44, 77, 45, 21, 234, 77, 888, 14, 66, 77 };
/*
const string keyStr("3A DA 75 21 DB E2 DB B3 11 B4 49 01 A5 C6 EA D4");
const int SIZE_IN = 8, SIZE_OUT = 8, SIZE_KEY = 16;
byte plain[SIZE_IN], crypt[SIZE_OUT], key[SIZE_KEY];
*/


NetworkType m_networktype;

AppDelegate::AppDelegate() {
	iCurrentStep=0;
	iGameMode=0;
	m_iCurrentGameID=1;
	m_iCurrentGameLevel=1;
	m_iUserTotalScore = 0;
	NetworkType m_networktype = NetworkType::NetworkTypeNone;
	m_iTotalPassedGameLevel = 0;
	m_iToolNum = 0;

	//initialize PromoteProduct
	m_PromoteProduct.m_iIsLoad = 1; //0: load from Server 1:read from local storage
	m_PromoteProduct.m_nCurrid = 1;
	m_PromoteProduct.m_strProductGUID = "";
	m_PromoteProduct.m_strProductName = "";
	m_PromoteProduct.m_strProductURL = "";
	m_PromoteProduct.m_strImage = "";
}

AppDelegate::~AppDelegate() 
{
	sp_GameRound.clear();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

std::string AppDelegate::GetAccessString(){
	std::string sPlatForm = "UnKnow";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	sPlatForm = "Android";
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
	sPlatForm = "iOS";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
	sPlatForm = "Win32";
#endif
	char sPost[1024];
	std::string m_strLayerName = "";
	if (m_iCurrentGameID == 1) m_strLayerName = "Find_difference";
	if (m_iCurrentGameID == 2) m_strLayerName = "Zoom_game";
	if (m_iCurrentGameID == 3) m_strLayerName = "Pixel_Shuffle";
	if (m_iCurrentGameID == 4) m_strLayerName = "Memory_Match";
	if (m_iCurrentGameID == 5) m_strLayerName = "Price_Right";
	if (m_iCurrentGameID == 6) m_strLayerName = "Sort_Price";
	Size screenSize = Director::getInstance()->getVisibleSize();
	sprintf(sPost, "layer=%s&plat_form=%s&screen=%dx%d",  m_strLayerName.c_str(), sPlatForm.c_str(), (int)screenSize.width, (int)screenSize.height);
	return string(sPost);

}
void AppDelegate::LogAccess(std::string m_strLayerName){

	/*
#define CC_PLATFORM_UNKNOWN            0
#define CC_PLATFORM_IOS                1
#define CC_PLATFORM_ANDROID            2
#define CC_PLATFORM_WIN32              3
#define CC_PLATFORM_MARMALADE          4
#define CC_PLATFORM_LINUX              5
#define CC_PLATFORM_BADA               6
#define CC_PLATFORM_BLACKBERRY         7
#define CC_PLATFORM_MAC                8
#define CC_PLATFORM_NACL               9
#define CC_PLATFORM_EMSCRIPTEN        10
#define CC_PLATFORM_TIZEN             11
#define CC_PLATFORM_QT5               12
#define CC_PLATFORM_WP8               13
#define CC_PLATFORM_WINRT             14
	*/
	/*
	std::string sPlatForm="UnKnow";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	sPlatForm="Android";
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
	sPlatForm="iOS";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
	sPlatForm="Win32";
#endif
	char sPost[1024];
	 Size screenSize = Director::getInstance()->getVisibleSize();
	sprintf(sPost,"%s?action=save_tracking&layer=%s&plat_form=%s&screen=%dx%d",GameAPIPOSTURL,m_strLayerName.c_str(),sPlatForm.c_str(),(int)screenSize.width,(int)screenSize.height);

	//action=save_tracking&layer=find_diff&plat_form=android&screen=1024x768

	HttpRequest* request = new HttpRequest();
	//string sUrl=GameAPIURL;
	//sUrl=sUrl+"?action=load_theme";
	//char url[100];
	//strcpy(url,sUrl.c_str());
    request->setUrl(sPost);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(AppDelegate::onHttpRequestCompleted,this));
    request->setTag("Save Tracking");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
	*/

}
void AppDelegate::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response){

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
	if(statusCode!=200){
		log("response code failed");
        log("error buffer: %s", response->getErrorBuffer());

        return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
}
void AppDelegate::GameJump(int i){
	switch(i){
		case 1: //This is a Find_difference
			{
					m_iCurrentGameID = 1;
			auto scene1 = FindDiffScene::create();
			TransitionScene *transition1 = TransitionFade::create(1, scene1);
			Director::getInstance()->replaceScene(transition1);
			}
			break;
		case 2: //This is a Zoom_game
		{
					m_iCurrentGameID = 2;
			auto scene2 = ZoomGameScene::create();
			TransitionScene *transition2 = TransitionFade::create(1, scene2);
			Director::getInstance()->replaceScene(transition2);
			}
			break;
		case 3: //This is a Pixel_Shuffle
		{
					m_iCurrentGameID = 3;
			auto scene3 = jigSawScene::create();
			TransitionScene *transition3 = TransitionFade::create(1, scene3);
			Director::getInstance()->replaceScene(transition3);
			}
			break;
		case 4: //This is a Memory_Match
		{
					m_iCurrentGameID = 4;
			auto scene4 = GameScene::create();
			TransitionScene *transition4 = TransitionFade::create(1, scene4);
			Director::getInstance()->replaceScene(transition4);
			}
			break;
		case 5: //This is a Price_Right
		{
					m_iCurrentGameID = 5;
			auto scene5 = PriceMatchScene::create();
			TransitionScene *transition5 = TransitionFade::create(1, scene5);
			Director::getInstance()->replaceScene(transition5);
			}
			break;
		case 6: //This is a Link
		{
					m_iCurrentGameID = 6;
				auto scene6 = MultiPriceMatchScene::create();
				TransitionScene *transition6 = TransitionFade::create(1, scene6);
				Director::getInstance()->replaceScene(transition6);
			}
			break;
	}
}
void AppDelegate::GameRunRoundNext(){
	
	log("runing round next .................");
	if(iCurrentStep < sp_GameRound.size()-1){
		//没有结束所有步骤
		stGameRound round = sp_GameRound.at(iCurrentStep);
		stGameRound roundNext = sp_GameRound.at(iCurrentStep + 1);
		if(round.round_id==roundNext.round_id){
			iCurrentStep++;
			StartTheme();
		}else{

			//GameRoundWinScene::createScene()
			auto scene1 = GameRoundWinScene::createScene();
			TransitionScene *transition1 = TransitionFade::create(1, scene1);
			Director::getInstance()->replaceScene(transition1);
		}

	}else{

		auto scene1 = GameRoundWinScene::createScene();
		TransitionScene *transition1 = TransitionFade::create(1, scene1);
		Director::getInstance()->replaceScene(transition1);
	}
	

}
void  AppDelegate::StartTheme(){
	stGameRound round = sp_GameRound.at(iCurrentStep);
	//iCurrentStep++;
	switch(round.round_type){
		case -2: //This is a text information
			{
					 auto scene1 = ThemeTextLayer::createScene();
					 TransitionScene *transition1 = TransitionFade::create(1, scene1);
					 if (iCurrentStep == 0)
					 {
						 Director::getInstance()->replaceScene(transition1);
					 }
					 else
					 {
						 Director::getInstance()->replaceScene(scene1);
					 }
			}
			break;
		case -1: //This is a random game
			{
				srand(time(NULL));
				srand((unsigned)time(0));
				//create a random number
				int random = rand();
			//int random = CCRANDOM_0_1() * 100;
			//int k= random % 5 +1;
				int k= random % 6 +1; //
				GameJump(k);
			}
			break;
		case 0: //This is a Collage
			{
				
			auto scene2 = CollageScene::createScene();
			TransitionScene *transition2 = TransitionFade::create(1, scene2);
			Director::getInstance()->replaceScene(transition2);
			}
			break;
		case 1: //This is a Find_difference
			GameJump(round.round_type);
			break;
		case 2: //This is a Zoom_game
			GameJump(round.round_type);
			break;
		case 3: //This is a Pixel_Shuffle
			GameJump(round.round_type);
			break;
		case 4: //This is a Memory_Match
			GameJump(round.round_type);
			break;
		case 5: //This is a Price_Right
			GameJump(round.round_type);
			break;
		case 6: //This is a Price_Right
			GameJump(round.round_type);
			break;

		
	}

}
std::string AppDelegate::GetLayoutString(){
	  Size visibleSize = Director::getInstance()->getVisibleSize();
	if(visibleSize.width < 960){
		return "_Small";
	}else{
		return "";
	}
}
bool AppDelegate::applicationDidFinishLaunching() {

	m_strLangCode=Application::getInstance()->getCurrentLanguageCode();


	    //初始化ShareSDK
    //C2DXShareSDK::open(CCString::create("20c019acd98b"), false);
     cn::sharesdk::C2DXShareSDK::open(CCString::create("628539549638"), false);
   
	 /*
	 char *text = "Hello World! 你好，中国！";
	 char *key = "1234567890";
	 size_t len;
	// const void *text1 = static_cast<unsigned char*>(text);

	 unsigned char *encrypt_data = (unsigned char *)xxtea_encrypt(text, strlen(text), (unsigned char*)key, &len);
	 char * base64_data = base64_encode(encrypt_data, len);

	 char *decrypt_data = (char*)xxtea_decrypt(encrypt_data, len, key, &len);
	 if (strncmp(text, decrypt_data, len) == 0) {
		 printf("success!\n");
	 }
	 else {
		 printf("fail!\n");
	 }
	 free(encrypt_data);
	 free(decrypt_data);
	 */

	 /*
	 std::string s1 = "1 2 3 4";
	 size_t size_in = hexStringToBytes(s1, plain);
	 size_t size_key = hexStringToBytes(keyStr, key);

	 if (size_in != SIZE_IN || size_key != SIZE_KEY)
		 return -1;

	 cout << "Plain: " << bytesToHexString(plain, size_in) << endl;
	 cout << "Key  : " << bytesToHexString(key, size_key) << endl;

	 TEA tea(key, 16, true);
	 tea.encrypt(plain, crypt);
	 cout << "Crypt: " << bytesToHexString(crypt, SIZE_OUT) << endl;

	 tea.decrypt(crypt, plain);
	 cout << "Plain: " << bytesToHexString(plain, SIZE_IN) << endl;
	 */

	 /*
	 RSA_Crypt rsa;
	 std::string s1 = "1234";
	 rsa.RSAKey();
	 string s2 = rsa.tencrypto(key1, key2, (char*)s1.c_str());

	 //decode it
	 string s3 = rsa.tdecrypto(key1, key2, s2);
	 */
	 

    //初始化社交平台信息
    this -> initPlatformConfig();

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	
    if(!glview) {
        glview = GLViewImpl::create("Tweebaa");
		//glview->setFrameSize(1024.f, 768.f);
		//glview->setFrameSize(800.f, 400.f);
        director->setOpenGLView(glview);
		//For debug
		
    }
	screenSize=director->getVisibleSize();
	if(screenSize.width < 960){
		director->getOpenGLView()->setDesignResolutionSize(800, 400, ResolutionPolicy::EXACT_FIT);
	}else{
		director->getOpenGLView()->setDesignResolutionSize(960, 640, ResolutionPolicy::EXACT_FIT);
	}

	/*
    if(!glview) {
        glview = GLViewImpl::createWithRect("Tweebaa App", Rect(0, 0, 800, 400));
        director->setOpenGLView(glview);
    }

	director->getOpenGLView()->setDesignResolutionSize(800, 400, ResolutionPolicy::FIXED_WIDTH);
	*/

	FileUtils::sharedFileUtils()->addSearchPath("res");// .addSearchPath("res");
	FileUtils::sharedFileUtils()->addSearchPath("res/Collage");
	FileUtils::sharedFileUtils()->addSearchPath("res/games_lance");
	//FileUtils::sharedFileUtils()->addSearchPath("zoom_game");
    // turn on/off display FPS
	director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

   //// create a scene. it's an autorelease object

 //   auto scene = HelloWorld::createScene();
	//director->runWithScene(scene);	



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif


	//------------------------------------------------------------------------
	//auto introvideo = VideoPlayer::createScene(VIDEO_INTRO, 0);
	//TransitionScene *transition = TransitionFade::create(1, scene);
	//Director::getInstance()->replaceScene(transition);
	//director->runWithScene(introvideo);
	//----------------------------------------------------------------------------

	
	auto introscene = TwbIntroScene::create();
	//auto introscene = Login::createScene();
	director->runWithScene(introscene);	
	//auto scene = ShareSceneLayer::createScene();
	// run
	//auto scene2 = MessageScene::create();

	//NetworkType t = NetworkType::NetworkTypeNone;
	//if (t == NetworkType::NetworkTypeNone){
	//    director->runWithScene(scene2);
	//}else {

	//}

	//auto scene2 = ZoomGameScene::create();
	//director->runWithScene(scene2);
	//TransitionScene *transition2 = TransitionFade::create(1, scene2);
	//Director::getInstance()->replaceScene(transition2);

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("res/Media/loop_94.wav");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_networktype = NetworkUtil::getNetworkType();
#else
	m_networktype = NetworkType::NetworkTypeWifi;
#endif
	//m_iBGMFlag = BGMusic_flag::ON;

	if (!FileUtils::getInstance()->isFileExist(BGM_MAIN)){
		MessageBox("Music File not found", "Music Error");
	}
	if (!FileUtils::getInstance()->isFileExist(BGM_GAME)){
		MessageBox("Music File not found", "Music Error");
	}
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BGM_MAIN);

	//AppDelegate::initGameLevelPassedInfo();
	initGameDefault();
	//AppDelegate::saveGameLevelPassedInfo();
	//int nPass = AppDelegate::getGameLevelPassedInfo();
	//StartBGMusic(BGM_MAIN);//start bgm
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

//bool AppDelegate::initGameLevelPassedInfo(){
//	
//	int nExisted = 0;
//	UserDefault *load = UserDefault::getInstance();
//	string str = load->getStringForKey("LevelInfoFlag", "0");
//	nExisted = std::atol(str.c_str());
//
//	if (nExisted == 0){
//		DataCenter *data = DataCenter::getInstance();
//		ValueVector values;
//		for(int i = 1; i < 7; i++){
//			for (int j = 0; j < g_maxLevel[i]+1;j++){
//				values.push_back(Value(0));
//			}
//			data->setValueVectorForKey(values,LevelInfoKey[i]);
//			values.clear();
//		}
//		load->setStringForKey("LevelInfoFlag", "1");
//	}
//
//	return true;
//}
////same current game as passed
//bool AppDelegate::saveGameLevelPassedInfo(){
//	DataCenter *data = DataCenter::getInstance();
//	ValueVector values;
//
//	values = data->getValueVectorByKey(LevelInfoKey[m_iCurrentGameID]);
//	values.at(m_iCurrentGameLevel) = Value(1);
//	data->setValueVectorForKey(values,LevelInfoKey[m_iCurrentGameID]);
//	return true;
//}
//
////get current level is passed or not
//int AppDelegate::getGameLevelPassedInfo(){
//	DataCenter *data = DataCenter::getInstance();
//	ValueVector values;
//	Value* val;
//	int nRet;
//
//	values = data->getValueVectorByKey(LevelInfoKey[m_iCurrentGameID]);
//
//	//for (int i = 0; values.size(); i++){
//	if (values.size() > m_iCurrentGameLevel){
//		nRet = values.at(m_iCurrentGameLevel).asInt();
//	} else {
//		UserDefault *load = UserDefault::getInstance();
//		load->setStringForKey("LevelInfoFlag", "0");
//		initGameLevelPassedInfo();
//		nRet = 0;
//	}
//	return nRet;
//}

//Move to next level whether player passes or not
void AppDelegate::moveToNextGameLevel(){
	//UserDefault *save = UserDefault::getInstance();
	////save current game level
	//save->setIntegerForKey(PlayerPassLevelCountKey[m_iCurrentGameID],getCurrentGameLevel());

	int iLvl = getCurrentGameLevel();
	char sLvl[10];
	sprintf(sLvl, "%d", iLvl);
	string strLevel = sLvl;
	DataAccess::setParamValue(PlayerPassLevelCountKey[m_iCurrentGameID],strLevel);

	//save total passed level
	//save->setIntegerForKey(TotalPassLevelCountKey[m_iCurrentGameID],m_iTotalPassedGameLevel+1); 
	m_iShowRegister++;
	if (m_iShowRegister > 5){
		m_iShowRegister = 0;
	}
}

void AppDelegate::SaveGamePassLevel(){
//	UserDefault *save = UserDefault::getInstance();
	//save current game level
	//save->setIntegerForKey(PlayerPassLevelCountKey[m_iCurrentGameID],m_iCurrentGameLevel); 
	//save total passed level
	//save->setIntegerForKey(TotalPassLevelCountKey[m_iCurrentGameID],getTotalPassedLevelInfo()+1); 

	int iPass = getTotalPassedLevelInfo()+1;
	char sPass[10];
	sprintf(sPass, "%d", iPass);
	string strPass = sPass;
	DataAccess::setParamValue(TotalPassLevelCountKey[m_iCurrentGameID],strPass);
	//saveGameLevelPassedInfo();
}

long AppDelegate::GetUserTotalScore(){
	string sScore =DataAccess::getParamValue("UserTotalScore", "\0");
	if (sScore.length() > 0)
	{
		m_iUserTotalScore = std::atol(this->base64_xxtea_decode(sScore).c_str());
	}
	else m_iUserTotalScore = 0;

	return m_iUserTotalScore;
}

int AppDelegate::GetUserLevel(){
	/*
	1	1	5.0 %
		2	100	5.5 %
		3	200	6.0 %
		4	500	6.5 %
		5	1000	7.0 %
		6	2000	7.5 %
		7	5000	8.0 %
		8	10000	8.5 %
		9	20000	9.0 %
		10	40000
	*/

	long iScore = GetUserTotalScore();
	int iLevel = 1;
	if (iScore < 100){
		iLevel = 1;
	}
	else if (iScore >= 100 && iScore < 200){
		iLevel = 2;
	}
	else if (iScore >= 200 && iScore < 500){
		iLevel = 3;
	}
	else if (iScore >= 500 && iScore < 1000){
		iLevel = 4;
	}
	else if (iScore >= 1000 && iScore < 2000){
		iLevel = 5;
	}
	else if (iScore >= 2000 && iScore < 5000){
		iLevel = 6;
	}
	else if (iScore >= 5000 && iScore < 10000){
		iLevel = 7;
	}
	else if (iScore >= 10000 && iScore < 20000){
		iLevel = 8;
	}
	else if (iScore >= 20000 && iScore < 40000){
		iLevel = 9;
	}
	else if (iScore >=40000){
		iLevel = 10;
	}
	return iLevel;
}

float AppDelegate::GetPercentToNextLevel(){
	/*
	1	1	5.0 %
		2	100	5.5 %
		3	200	6.0 %
		4	500	6.5 %
		5	1000	7.0 %
		6	2000	7.5 %
		7	5000	8.0 %
		8	10000	8.5 %
		9	20000	9.0 %
		10	40000
	*/
	float fRet = 0.0f;
	long iScore = GetUserTotalScore();

	if (iScore < 100){
		fRet = float(iScore) / 100;
	}
	else if (iScore >= 100 && iScore < 200){
		fRet = float(iScore - 100) / 100;
	}
	else if (iScore >= 200 && iScore < 500){
		fRet = float(iScore - 200) / 300;
	}
	else if (iScore >= 500 && iScore < 1000){
		fRet = float(iScore - 500) / 500;
	}
	else if (iScore >= 1000 && iScore < 2000){
		fRet = float(iScore - 1000) / 1000;
	}
	else if (iScore >= 2000 && iScore < 5000){
		fRet = float(iScore - 2000) / 3000;
	}
	else if (iScore >= 5000 && iScore < 10000){
		fRet = float(iScore - 5000) / 5000;
	}
	else if (iScore >= 10000 && iScore < 20000){
		fRet = float(iScore - 10000) / 10000;
	}
	else if (iScore >= 20000 && iScore < 40000){
		fRet = float(iScore - 20000) / 20000;
	}
	else if (iScore >=40000){
		fRet = 1.0f;
	}
	return fRet;
}
int AppDelegate::GetToolsDiff(){
	//取最后更新
	
	int lLastUpdate;
	int lTotal;
	string slastUpdate = DataAccess::getParamValue("UserLastUpdateTools", "\0");
	if (slastUpdate.length() > 0)
	{
		lLastUpdate = std::atoi(this->base64_xxtea_decode(slastUpdate).c_str());
	}
	else lLastUpdate = 0;

	string sTotal = DataAccess::getParamValue("Total_Tool_Num", "\0");
	if (sTotal.length() > 0)
	{
		lTotal = std::atoi(this->base64_xxtea_decode(sTotal).c_str());
	}
	else if (m_UserID<0)
	{
		lTotal = 10;//
	}
	else
	{
		lTotal = 0;
	}

	return lTotal - lLastUpdate;

}

void AppDelegate::SaveUserToolsDiff(int iScore)
{

	char sTotal[10];
	sprintf(sTotal, "%d", iScore);
	string strTool = sTotal;
	strTool = this->base64_xxtea_encode(strTool);
	DataAccess::setParamValue("Total_Tool_Num", strTool);
	DataAccess::setParamValue("UserLastUpdateTools", strTool);
}
long AppDelegate::GetUserScoreDiff(){

	long lLastUpdate;
	long lTotal;
	string slastUpdate = DataAccess::getParamValue("UserLastUpdateScore", "\0");
	if (slastUpdate.length() > 0)
	{
		lLastUpdate = std::atoi(this->base64_xxtea_decode(slastUpdate).c_str());
	}
	else lLastUpdate = 0;

	string sTotal = DataAccess::getParamValue("UserTotalScore", "\0");
	if (sTotal.length() > 0)
	{
		lTotal = std::atoi(this->base64_xxtea_decode(sTotal).c_str());
	}
	else
	{
		lTotal = 0;
	}

	return lTotal - lLastUpdate;
}
void AppDelegate::SaveUserScoreDiff(int iScore )
{

	char sTotal[10];
	sprintf(sTotal, "%d", iScore);
	string strScore = sTotal;
	strScore = this->base64_xxtea_encode(strScore);
	DataAccess::setParamValue("UserTotalScore", strScore);
	DataAccess::setParamValue("UserLastUpdateScore", strScore);

}
void AppDelegate::UpdateUserScore(int iFlag, int iScore){
	if (iFlag == 1){
		//Add
		m_iUserTotalScore = m_iUserTotalScore + iScore;
	}
	else{
		//Sub
		m_iUserTotalScore = m_iUserTotalScore - iScore;
	}
	char sTotal[20];
	sprintf(sTotal, "%ld", m_iUserTotalScore);
	string strScore = sTotal;
	strScore = this->base64_xxtea_encode(strScore);
	DataAccess::setParamValue("UserTotalScore", strScore);

	

}
void AppDelegate::initPlatformConfig()
{
	/*
    //新浪微博
    CCDictionary *sinaConfigDict = CCDictionary::create();
    sinaConfigDict -> setObject(CCString::create("568898243"), "app_key");
    sinaConfigDict -> setObject(CCString::create("38a4f8204cc784f81f9f0daaf31e02e3"), "app_secret");
    sinaConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    //腾讯微博
    CCDictionary *tcConfigDict = CCDictionary::create();
    tcConfigDict -> setObject(CCString::create("801307650"), "app_key");
    tcConfigDict -> setObject(CCString::create("ae36f4ee3946e1cbb98d6965b0b2ff5c"), "app_secret");
    tcConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTencentWeibo, tcConfigDict);
    
    //短信
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
    
    //QQ空间
    CCDictionary *qzConfigDict = CCDictionary::create();
    qzConfigDict -> setObject(CCString::create("100371282"), "app_id");
    qzConfigDict -> setObject(CCString::create("aed9b0303e3ed1e27bae87c33761161d"), "app_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //微信
    CCDictionary *wcConfigDict = CCDictionary::create();
    wcConfigDict -> setObject(CCString::create("wx4868b35061f87885"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
    
    //QQ
    CCDictionary *qqConfigDict = CCDictionary::create();
    qqConfigDict -> setObject(CCString::create("100371282"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
    */
    //Facebook
    /*
    CCDictionary *fbConfigDict = CCDictionary::create();
    fbConfigDict -> setObject(CCString::create("107704292745179"), "api_key");
    fbConfigDict -> setObject(CCString::create("38053202e1a5fe26c80c753071f0b573"), "app_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFacebook, fbConfigDict);
    
    //Twitter
    CCDictionary *twConfigDict = CCDictionary::create();
    twConfigDict -> setObject(CCString::create("mnTGqtXk0TYMXYTN7qUxg"), "consumer_key");
    twConfigDict -> setObject(CCString::create("ROkFqr8c3m1HXqS3rm3TJ0WkAJuwBOSaWhPbZ9Ojuc"), "consumer_secret");
    twConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTwitter, twConfigDict);
    */
    //Facebook
    CCDictionary *fbConfigDict = CCDictionary::create();
    fbConfigDict -> setObject(CCString::create("1626882964218368"), "api_key");
    fbConfigDict -> setObject(CCString::create("9ffda9de2c0ddef4fd6899e588681e4f"), "app_secret");
    cn::sharesdk::C2DXShareSDK::setPlatformConfig(cn::sharesdk::C2DXPlatTypeFacebook, fbConfigDict);
    
    //Twitter
    CCDictionary *twConfigDict = CCDictionary::create();
    twConfigDict -> setObject(CCString::create("YEH4b8DoaJOFoYjbzTDvaCNdp"), "consumer_key");
    twConfigDict -> setObject(CCString::create("foPJPRY7xRiwQZU3cwL8qvFENK6ZrEa4RZsOk8ZA4cvJeQ3Gv9"), "consumer_secret");
    twConfigDict -> setObject(CCString::create("http://mob.com"), "redirect_uri");
    cn::sharesdk::C2DXShareSDK::setPlatformConfig(cn::sharesdk::C2DXPlatTypeTwitter, twConfigDict);
    
    //Google+
    CCDictionary *gpConfigDict = CCDictionary::create();
    gpConfigDict -> setObject(CCString::create("232554794995.apps.googleusercontent.com"), "client_id");
    gpConfigDict -> setObject(CCString::create("PEdFgtrMw97aCvf0joQj7EMk"), "client_secret");
    gpConfigDict -> setObject(CCString::create("http://localhost"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeGooglePlus, gpConfigDict);
    
	/*
    //人人网
    CCDictionary *rrConfigDict = CCDictionary::create();
    rrConfigDict -> setObject(CCString::create("226427"), "app_id");
    rrConfigDict -> setObject(CCString::create("fc5b8aed373c4c27a05b712acba0f8c3"), "app_key");
    rrConfigDict -> setObject(CCString::create("f29df781abdd4f49beca5a2194676ca4"), "secret_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeRenren, rrConfigDict);
    
    //开心网
    CCDictionary *kxConfigDict = CCDictionary::create();
    kxConfigDict -> setObject(CCString::create("358443394194887cee81ff5890870c7c"), "api_key");
    kxConfigDict -> setObject(CCString::create("da32179d859c016169f66d90b6db2a23"), "secret_key");
    kxConfigDict -> setObject(CCString::create("http://www.sharesdk.cn/"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeKaixin, kxConfigDict);
    */
    //邮件
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeMail, NULL);
    
    //打印
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeAirPrint, NULL);
    
    //拷贝
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeCopy, NULL);
    /*
    //搜狐微博
    CCDictionary *shwbConfigDict = CCDictionary::create();
    shwbConfigDict -> setObject(CCString::create("SAfmTG1blxZY3HztESWx"), "consumer_key");
    shwbConfigDict -> setObject(CCString::create("yfTZf)!rVwh*3dqQuVJVsUL37!F)!yS9S!Orcsij"), "consumer_secret");
    shwbConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "callback_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSohuWeibo, shwbConfigDict);
    
    //网易微博
    CCDictionary *neConfigDict = CCDictionary::create();
    neConfigDict -> setObject(CCString::create("T5EI7BXe13vfyDuy"), "consumer_key");
    neConfigDict -> setObject(CCString::create("gZxwyNOvjFYpxwwlnuizHRRtBRZ2lV1j"), "consumer_secret");
    neConfigDict -> setObject(CCString::create("http://www.shareSDK.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatType163Weibo, neConfigDict);
    
    //豆瓣
    CCDictionary *dbConfigDict = CCDictionary::create();
    dbConfigDict -> setObject(CCString::create("02e2cbe5ca06de5908a863b15e149b0b"), "api_key");
    dbConfigDict -> setObject(CCString::create("9f1e7b4f71304f2f"), "secret");
    dbConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeDouBan, dbConfigDict);
    
    //印象笔记
    CCDictionary *enConfigDict = CCDictionary::create();
    enConfigDict -> setObject(CCString::create("sharesdk-7807"), "consumer_key");
    enConfigDict -> setObject(CCString::create("d05bf86993836004"), "consumer_secret");
    enConfigDict -> setObject(CCString::create("0"), "host_type");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeEvernote, enConfigDict);
    */
    //LinkedIn
    CCDictionary *liConfigDict = CCDictionary::create();
    liConfigDict -> setObject(CCString::create("ejo5ibkye3vo"), "api_key");
    liConfigDict -> setObject(CCString::create("cC7B2jpxITqPLZ5M"), "secret_key");
    liConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "redirect_url");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeLinkedIn, liConfigDict);
    
    //Pinterest
    CCDictionary *piConfigDict = CCDictionary::create();
    piConfigDict -> setObject(CCString::create("1432928"), "client_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypePinterest, piConfigDict);
    
	/*
    //Pocket
    CCDictionary *poConfigDict = CCDictionary::create();
    poConfigDict -> setObject(CCString::create("11496-de7c8c5eb25b2c9fcdc2b627"), "consumer_key");
    poConfigDict -> setObject(CCString::create("pocketapp1234"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypePocket, poConfigDict);
    
    //Instapaper
    CCDictionary *ipConfigDict = CCDictionary::create();
    ipConfigDict -> setObject(CCString::create("4rDJORmcOcSAZL1YpqGHRI605xUvrLbOhkJ07yO0wWrYrc61FA"), "consumer_key");
    ipConfigDict -> setObject(CCString::create("GNr1GespOQbrm8nvd7rlUsyRQsIo3boIbMguAl9gfpdL0aKZWe"), "consumer_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeInstapaper, ipConfigDict);
    
    //有道云笔记
    CCDictionary *ydConfigDict = CCDictionary::create();
    ydConfigDict -> setObject(CCString::create("dcde25dca105bcc36884ed4534dab940"), "consumer_key");
    ydConfigDict -> setObject(CCString::create("d98217b4020e7f1874263795f44838fe"), "consumer_secret");
    ydConfigDict -> setObject(CCString::create("http://www.sharesdk.cn/"), "oauth_callback");
    ydConfigDict -> setObject(CCString::create("1"), "host_type");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeYouDaoNote, ydConfigDict);
    
    //搜狐随身看
    CCDictionary *shkConfigDict = CCDictionary::create();
    shkConfigDict -> setObject(CCString::create("e16680a815134504b746c86e08a19db0"), "app_key");
    shkConfigDict -> setObject(CCString::create("b8eec53707c3976efc91614dd16ef81c"), "app_secret");
    shkConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSohuKan, shkConfigDict);
    
    //Flickr
    CCDictionary *flickrConfigDict = CCDictionary::create();
    flickrConfigDict -> setObject(CCString::create("33d833ee6b6fca49943363282dd313dd"), "api_key");
    flickrConfigDict -> setObject(CCString::create("3a2c5b42a8fbb8bb"), "api_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeFlickr, flickrConfigDict);
    
    //Tumblr
    CCDictionary *tumblrConfigDict = CCDictionary::create();
    tumblrConfigDict -> setObject(CCString::create("2QUXqO9fcgGdtGG1FcvML6ZunIQzAEL8xY6hIaxdJnDti2DYwM"), "consumer_key");
    tumblrConfigDict -> setObject(CCString::create("3Rt0sPFj7u2g39mEVB3IBpOzKnM3JnTtxX2bao2JKk4VV1gtNo"), "consumer_secret");
    tumblrConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "callback_url");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTumblr, tumblrConfigDict);
    
    //Dropbox
    CCDictionary *dropboxConfigDict = CCDictionary::create();
    dropboxConfigDict -> setObject(CCString::create("7janx53ilz11gbs"), "app_key");
    dropboxConfigDict -> setObject(CCString::create("c1hpx5fz6tzkm32"), "app_secret");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeDropbox, dropboxConfigDict);
    
    //Instagram
    CCDictionary *instagramConfigDict = CCDictionary::create();
    instagramConfigDict -> setObject(CCString::create("ff68e3216b4f4f989121aa1c2962d058"), "client_id");
    instagramConfigDict -> setObject(CCString::create("1b2e82f110264869b3505c3fe34e31a1"), "client_secret");
    instagramConfigDict -> setObject(CCString::create("http://sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeInstagram, instagramConfigDict);
    
    //VK
    CCDictionary *vkConfigDict = CCDictionary::create();
    vkConfigDict -> setObject(CCString::create("3921561"), "application_id");
    vkConfigDict -> setObject(CCString::create("6Qf883ukLDyz4OBepYF1"), "secret_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeVKontakte, vkConfigDict);
	*/
}

bool AppDelegate::PlayEffect(const char* pAudiofile){

	if (getBGMstatus() == BGMusic_flag::ON){
		auto audio = SimpleAudioEngine::getInstance();
		if(audio == nullptr) {return false;}
		audio->playEffect(pAudiofile, false, 1.0f, 1.0f, 1.0f);
	}
	return true;
}
bool AppDelegate::StartBGMusic(const char* pAudiofile){
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("res/Media/loop_94.wav", true);  
	m_iBGMFlag = BGMusic_flag::ON;

	UserDefault *save = UserDefault::getInstance();
	if (save == nullptr) {return false;}
	save->setIntegerForKey("BGM_FLAG", BGMusic_flag::ON);
	save->setStringForKey("BGM_FILE", pAudiofile);

	//for testing only
	//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pAudiofile, true);



	return true;
}

bool AppDelegate::StopBGMusic(){
	m_iBGMFlag = BGMusic_flag::OFF;	
	UserDefault *save = UserDefault::getInstance();
	if (save == nullptr) {return false;}
	save->setIntegerForKey("BGM_FLAG", BGMusic_flag::OFF);
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();  

	return true;
}

bool AppDelegate::getBGMstatus(){
	UserDefault *save = UserDefault::getInstance();
	if (save == nullptr) {return false;}
	m_iBGMFlag = save->getIntegerForKey("BGM_FLAG", BGMusic_flag::ON);
	return m_iBGMFlag;
}

string AppDelegate::base64_xxtea_encode(string strSource){
	const char *text = strSource.c_str();
	char *key = "2012@tweebaa!2015*&%^";
	size_t len;
	// const void *text1 = static_cast<unsigned char*>(text);

	unsigned char *encrypt_data = (unsigned char *)xxtea_encrypt(text, strlen(text), (unsigned char*)key, &len);
	char * base64_data = base64_encode(encrypt_data, len);
	/*
	char *decrypt_data = (char*)xxtea_decrypt(encrypt_data, len, key, &len);
	if (strncmp(text, decrypt_data, len) == 0) {
		printf("success!\n");
	}
	else {
		printf("fail!\n");
	}*/
	free(encrypt_data);
	return string(base64_data);
	//free(decrypt_data);

}
string AppDelegate::base64_xxtea_decode(string strSource){

	if (strSource.length() < 6){
		return strSource;

	}
	const char *text = strSource.c_str();
	char *key = "2012@tweebaa!2015*&%^";
	size_t len;
	// const void *text1 = static_cast<unsigned char*>(text);

	//unsigned char *encrypt_data = (unsigned char *)xxtea_encrypt(text, strlen(text), (unsigned char*)key, &len);
	unsigned char * encrypt_data = base64_decode(text, &len);

	char *decrypt_data = (char*)xxtea_decrypt(encrypt_data, len, key, &len);
	string sRet(decrypt_data);
	free(encrypt_data);
	free(decrypt_data);
	return sRet;
}

std::string AppDelegate::random_string(size_t length){
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;

}
std::string AppDelegate::encode_data_for_http(std::string strSource){

	std::string str10 = random_string(10);
	str10 = str10 + strSource;
	str10 = base64_xxtea_encode(str10);

	//str10 = string_to_hex(ss);
	return str10;

}

std::string AppDelegate::string_to_hex(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}


NetworkType AppDelegate::GetNetWorkStatus(){
	return m_networktype;
	//return NetworkType::NetworkTypeNone;
}

int AppDelegate::getCurrentGameLevel(){
	//get current level
	//UserDefault *save = UserDefault::getInstance();
	//m_iCurrentGameLevel = save->getIntegerForKey(PlayerPassLevelCountKey[m_iCurrentGameID], 0) + 1;
	std::string slvl = DataAccess::getParamValue(PlayerPassLevelCountKey[m_iCurrentGameID],"0");
	m_iCurrentGameLevel = atoi(slvl.c_str()) + 1;

	if (m_iCurrentGameID == 1){
		if (m_iCurrentGameLevel > g_maxLevel[m_iCurrentGameID]) { 		//For testing purpose, set 3 levels only
			m_iCurrentGameLevel = 1;
		}
	}else{
		std::vector<_ProductInfo>* pList = nullptr;
		pList = DataAccess::getProductListbyGame(m_iCurrentGameID);
		if (m_iCurrentGameLevel > pList->size()) { 		//For testing purpose, set 3 levels only
			m_iCurrentGameLevel = 1;
		}
	}

	//if (m_iCurrentGameLevel > g_maxLevel[m_iCurrentGameID]) { 		//For testing purpose, set 3 levels only
	//	m_iCurrentGameLevel = 1;
	//}
	return m_iCurrentGameLevel;
}
int AppDelegate::getTotalPassedLevelInfo(){
	//get current level
	//UserDefault *save = UserDefault::getInstance();
	//m_iTotalPassedGameLevel = save->getIntegerForKey(TotalPassLevelCountKey[m_iCurrentGameID], 0);

	std::string slvl = DataAccess::getParamValue(TotalPassLevelCountKey[m_iCurrentGameID],"0");
	m_iTotalPassedGameLevel = atoi(slvl.c_str());

	//if (m_iCurrentGameLevel > g_maxLevel[m_iCurrentGameID]) { 		//For testing purpose, set 3 levels only
	//	m_iCurrentGameLevel = 1;
	//}
	return m_iTotalPassedGameLevel;
}
bool AppDelegate::setToolsNumberInfo(int nToolNum){
	//get current level
	char sTotal[10];
	sprintf(sTotal, "%d", nToolNum);
	string strTool = sTotal;
	strTool = this->base64_xxtea_encode(strTool);
	DataAccess::setParamValue("Total_Tool_Num", strTool);
	return true;
}
int AppDelegate::getToolsNumberInfo(){
	std::string strTool = DataAccess::getParamValue("Total_Tool_Num","\0");
	if (strTool.length() > 0)
	{
		m_iToolNum = atoi(this->base64_xxtea_decode(strTool).c_str());
	}
	else if (m_UserID>0)
	{
		m_iToolNum = 0;
	}
	else
	{
		m_iToolNum = 10;
	}
	return m_iToolNum;
}

string AppDelegate::GetUserGuid(){
	std::string s ="\0";
	if (m_UserID > 0)
	{
		_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
		s = userInfo.strUserGuid;
	}
	return s;
}

PromoteProduct AppDelegate::getPromoteProduct(){
	if (m_PromoteProduct.m_strProductGUID.size() == 0){
		m_PromoteProduct.m_strProductGUID = "1700012210";
		m_PromoteProduct.m_strProductName = "TweeBot";
		m_PromoteProduct.m_strProductURL = "https://www.tweebaa.com/Product/saleBuy.aspx?id=670bdf26-a935-4643-ac0e-ba24c2249107";
		m_PromoteProduct.m_strImage = "res/spot_it/LevelInfo/SPOT_level1_Tweebot_imageA.jpg";	
	}
	
	return m_PromoteProduct;
}

bool AppDelegate::setPromoteProduct(std::string strSKU){

	bool bRet = false;
	do{
		//std::string strFileName = PATH_PROMOTION_PRODUCT;

		//m_PromoteProduct.m_iIsLoad = 1;
		m_PromoteProduct.m_strProductGUID = "1700012210";
		m_PromoteProduct.m_strProductName = "TweeBot";
		m_PromoteProduct.m_strProductURL = "https://www.tweebaa.com/Product/saleBuy.aspx?id=670bdf26-a935-4643-ac0e-ba24c2249107";
		m_PromoteProduct.m_strImage = "res/spot_it/LevelInfo/SPOT_level1_Tweebot_imageA.jpg";				

		//m_PromoteProduct.m_strProductGUID = "";
		//m_PromoteProduct.m_strProductName = "";
		//m_PromoteProduct.m_strProductURL = "";
		//m_PromoteProduct.m_strImage = "";

		_ProductInfo PDInfo;
		if(strSKU.size() > 0){
			PDInfo = DataAccess::getProductInfobySKU(strSKU);
			if(PDInfo.ID.size() > 0){
				m_PromoteProduct.m_strProductName = PDInfo.NAME;
				m_PromoteProduct.m_strProductURL = PDInfo.URL;
				m_PromoteProduct.m_strImage = PDInfo.IMAGE;
			}
		}else{
			std::vector<_ProductInfo>* pPDInfolist = nullptr;
			pPDInfolist = DataAccess::getProductList();
			if( m_PromoteProduct.m_nCurrid > pPDInfolist->size() ){ 
					m_PromoteProduct.m_nCurrid = 1;
			}

			char sID[10];
			sprintf(sID, "%d", m_PromoteProduct.m_nCurrid);
			string strID = sID;
			PDInfo = DataAccess::getProductInfobyID(strID);
			if(PDInfo.ID.size() > 0){
				m_PromoteProduct.m_strProductGUID = PDInfo.ID;
				m_PromoteProduct.m_strProductName = PDInfo.NAME;
				m_PromoteProduct.m_strProductURL = PDInfo.URL;
				m_PromoteProduct.m_strImage = PDInfo.IMAGE;
				m_PromoteProduct.m_nCurrid++;
			}
		}

		//if (FileUtils::getInstance()->isFileExist(strFileName)){
		//	ValueMap node = FileUtils::getInstance()->getValueMapFromFile(strFileName);
		//	ValueMap pdList = node["PromoteProductList"].asValueMap();
		//	if (pdList.size() > 0) {
		//		if(strSKU.size() > 0){
		//			m_PromoteProduct.m_strProductGUID = strSKU;
		//			auto it = pdList.find(m_PromoteProduct.m_strProductGUID);
		//			if (it != pdList.end()){
		//				ValueMap levelinfo = it->second.asValueMap();
		//				//m_PromoteProduct.m_strProductGUID = levelinfo["ID"].asString();
		//				m_PromoteProduct.m_strProductName = levelinfo["NAME"].asString();
		//				m_PromoteProduct.m_strProductURL = levelinfo["URL"].asString();
		//				m_PromoteProduct.m_strImage = levelinfo["IMAGE"].asString();
		//			}
		//		}else{
		//			//m_PromoteProduct.m_nCurrid++;
		//			if( m_PromoteProduct.m_nCurrid == pdList.size() ){ 
		//					m_PromoteProduct.m_nCurrid = 0;
		//			}
		//			//char p[20];
		//			//sprintf(p, "%d", m_PromoteProduct.m_nCurrid);
		//			//std::string strID = p;
		//			//auto it = pdList.find(strID);
		//			//if (it != pdList.end()){
		//			//	ValueMap levelinfo = it->second.asValueMap();
		//			//	//m_PromoteProduct.m_strProductGUID = levelinfo["ID"].asString();
		//			//	m_PromoteProduct.m_strProductName = levelinfo["NAME"].asString();
		//			//	m_PromoteProduct.m_strProductURL = levelinfo["URL"].asString();
		//			//	m_PromoteProduct.m_strImage = levelinfo["IMAGE"].asString();
		//			//}

		//			int i = 0;
		//			for (auto it = pdList.begin(); it != pdList.end(); it++)
		//			{
		//				if (i == m_PromoteProduct.m_nCurrid){
		//					ValueMap levelinfo = it->second.asValueMap();

		//					m_PromoteProduct.m_strProductGUID = it->first.c_str();
		//					m_PromoteProduct.m_strProductName = levelinfo["NAME"].asString();
		//					m_PromoteProduct.m_strProductURL = levelinfo["URL"].asString();
		//					m_PromoteProduct.m_strImage = levelinfo["IMAGE"].asString();
		//					m_PromoteProduct.m_nCurrid++;
		//					break;
		//				}
		//				i++;
		//			}
		//		}
 	//		}
		//}
		//else{
		//	m_PromoteProduct.m_strProductGUID = "1700012210";
		//	m_PromoteProduct.m_strProductName = "TweeBot";
		//	m_PromoteProduct.m_strProductURL = "https://www.tweebaa.com/Product/saleBuy.aspx?id=670bdf26-a935-4643-ac0e-ba24c2249107";
		//	m_PromoteProduct.m_strImage = "res/spot_it/LevelInfo/SPOT_level1_Tweebot_imageA.jpg";	
		//}
		bRet = true;
	}while(0);
	return bRet;
}
void AppDelegate::setReward(int iReward,int iStar)
{
	m_Reward = iReward;
	m_Star = iStar;
}
int AppDelegate::getReward()
{
	return m_Reward;
}
int AppDelegate::getStar()
{
	return m_Star;
}


void AppDelegate::SyncPoint()
{
}

void AppDelegate::onSyncCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		log("onSyncCompleted no response.");
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
		log("onSyncCompleted response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	if (statusCode != 200){
		log("onSyncCompleted response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}


	// dump data
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
	if (iScore>0)
	{
		SaveUserScoreDiff(iScore);
	}

}

void AppDelegate::ResetDailyScore(){

	std::string sCurrent;
	std::string sLasttime;

	//UserDefault *load = UserDefault::getInstance();
	//sLasttime = load->getStringForKey("LastRefreshDate","\0");
	sLasttime = DataAccess::getParamValue("LastRefreshDate","***************");

	char *pszCurrTime = (char*)malloc(sizeof(char)* 20);
	memset(pszCurrTime, 0, sizeof(char)* 20);
	time_t now;
	time(&now);
	strftime(pszCurrTime, 20, "%Y/%m/%d %H:%M:%S", localtime(&now));
	sCurrent = pszCurrTime;

	if (strcmp(sLasttime.substr(0, 10).c_str(), sCurrent.substr(0, 10).c_str()) != 0) //if it's different day
	{
		//load->setStringForKey("LastRefreshDate", sCurrent);
		DataAccess::setParamValue("LastRefreshDate", sCurrent);

		char sTotal[10];
		sprintf(sTotal, "%d", 0);
		string sScore = this->base64_xxtea_encode(sTotal);

		for(int i = 1; i < 7; i++){
			//load->setStringForKey(DailyScoreCountKey[i], sScore);
			DataAccess::setParamValue(DailyScoreCountKey[i], sScore);
		}

		//load->setStringForKey("DailyScore_Publish", sScore);
		//load->setStringForKey("DailyScore_Share", sScore);
		DataAccess::setParamValue("DailyScore_Publish", sScore);
		DataAccess::setParamValue("DailyScore_Publish", sScore);

	}
	char sTool[10];
	sprintf(sTool, "%d",  10);
	string strTool = sTool;
	strTool = this->base64_xxtea_encode(strTool);
	DataAccess::setParamValue("Total_Tool_Num", strTool);
	//nNew = nCurrent + nScore;
	//char sTotal[10];
	//sprintf(sTotal, "%d", nNew);
	//string sTotalScore = this->base64_xxtea_encode(sTotal);
	//DataAccess::setParamValue(DailyScoreCountKey[m_iCurrentGameID], sTotalScore);
	
	return;

}

void AppDelegate::updateGameWinScore(int nSec){
	int nScore = 0;
	int	nNew = 0;
	int nCurrent = 0;
	int nStar = 0;

	//UserDefault *save = UserDefault::getInstance();
	//get today's total points
	//string sScore = save->getStringForKey(DailyScoreCountKey[m_iCurrentGameID], "0"); 
	string sScore =DataAccess::getParamValue(DailyScoreCountKey[m_iCurrentGameID], "\0");
	if (sScore.length() > 0)
	{
		nCurrent = std::atoi(this->base64_xxtea_decode(sScore).c_str());
	}
	else nCurrent = 0;
	/*
	if (sScore.length() > 6){
		string sDecode = this->base64_xxtea_decode(sScore);
		nCurrent = std::atol(sDecode.c_str());
	}
	else{
		nCurrent = std::atol(sScore.c_str());
	}*/

	switch (m_iCurrentGameID)
	{
	case 1://spot
		if (nSec > 60){
			nScore = WIN_POINT_3;
			nStar = 3;
		}
		else if (nSec > 30 && nSec <= 60){
			nScore = WIN_POINT_2;
			nStar = 2;
		}
		else{
			nScore = WIN_POINT_1;
			nStar = 1;
		}
		break;
	case 2://zoom
		if (nSec > 60){
			nScore = WIN_POINT_3;
			nStar = 3;
		}
		else if (nSec > 30 && nSec <= 60){
			nScore = WIN_POINT_2;
			nStar = 2;
		}
		else{
			nScore = WIN_POINT_1;
			nStar = 1;
		}
		break;
	case 3://jigsaw
		if (nSec > 120){
			nScore = WIN_POINT_9;
			nStar = 3;
		}
		else if (nSec > 60 && nSec <= 120){
			nScore = WIN_POINT_7;
			nStar = 2;
		}
		else{
			nScore = WIN_POINT_5;
			nStar = 1;
		}
		break;
	case 4://flip
		if (nSec > 15){
			nScore = WIN_POINT_3;
			nStar = 3;
		}
		else if (nSec > 5 && nSec <= 15){
			nScore = WIN_POINT_2;
			nStar = 2;
		}
		else{
			nScore = WIN_POINT_1;
			nStar = 1;
		}
		break;
	case 5://match
		if (nSec > 60){
			nScore = WIN_POINT_3;
			nStar = 3;
		}
		else if (nSec > 30 && nSec <= 60){
			nScore = WIN_POINT_2;
			nStar = 2;
		}
		else{
			nScore = WIN_POINT_1;
			nStar = 1;
		}
		break;
	case 6://link
		if (nSec > 60){
			nScore = WIN_POINT_3;
			nStar = 3;
		}
		else if (nSec > 30 && nSec <= 60){
			nScore = WIN_POINT_2;
			nStar = 2;
		}
		else{
			nScore = WIN_POINT_1;
			nStar = 1;
		}
		break;
	}

	if (nCurrent + nScore >= DAILY_SCORE_LIMIT)
	{
		nScore = DAILY_SCORE_LIMIT - nCurrent;
	}

	nNew = nCurrent + nScore;
	char sTotal[10];
	sprintf(sTotal, "%d", nNew);
	string sTotalScore = this->base64_xxtea_encode(sTotal);
	//save->setStringForKey(DailyScoreCountKey[m_iCurrentGameID], sTotalScore);
	DataAccess::setParamValue(DailyScoreCountKey[m_iCurrentGameID], sTotalScore);
	UpdateUserScore(1,nScore);
	setReward(nScore,nStar);

	return;
}
void  AppDelegate::updateCollageScore(int nMode)
{
	int nScore=0;
	long nCurrent=0;
	long nNew = 0;
	string sPublish = DataAccess::getParamValue("DailyScore_Publish", "\0");
	string sShare = DataAccess::getParamValue("DailyScore_Share", "\0");
	if (nMode == UPDATE_SCORE_PUBLISH)
	{
		if (sPublish.length() > 0)
		{ 
			nCurrent = std::atol(this->base64_xxtea_encode(sPublish).c_str()); 
		}
		else
		{
			nCurrent = 0;
		}
		nScore = 10;
		if (nCurrent + nScore >= PUBLISH_SCORE_LIMIT)
		{
			nScore = PUBLISH_SCORE_LIMIT - nCurrent;
		}
	}
	else if (nMode == UPDATE_SCORE_PUBLISH)
	{
		if (sShare.length() > 0)
		{
			nCurrent = std::atol(this->base64_xxtea_encode(sShare).c_str()); 
		}
		else
		{
			nCurrent = 0;
		}
		nScore = 1;
		if (nCurrent + nScore >= DAILY_SCORE_LIMIT)
		{
			nScore = DAILY_SCORE_LIMIT - nCurrent;
		}
	}
	else return;
	nNew = nCurrent + nScore;
	char sTotal[10];
	sprintf(sTotal, "%d", nNew);
	string sTotalScore = this->base64_xxtea_encode(sTotal);
	if (nMode == UPDATE_SCORE_PUBLISH)
	{
		DataAccess::setParamValue("DailyScore_Publish", sTotalScore);
	}
	else
	{
		//UserDefault::getInstance()->setStringForKey("DailyScore_Share", sTotalScore)
		DataAccess::setParamValue("DailyScore_Share", sTotalScore);
	}
	UpdateUserScore(1, nScore);
}
bool AppDelegate::initGameDefault(){
	bool bRet = false;
	do{
		UserDefault *load = UserDefault::getInstance();
		int nSetFlag = load->getIntegerForKey("IsUserDefaultInited", 0);
		
		if(nSetFlag == 0){ //first-time installing
			iCurrentStep=0;
			iGameMode=0;
			m_iCurrentGameID = 1;
			m_iCurrentGameLevel = 1;
			m_iTotalPassedGameLevel = 0;
			m_Reward = 0;
			m_Refresh = 0;
			m_iShowRegister = 0;
			
			NetworkType m_networktype = NetworkType::NetworkTypeNone;

			//UserDefault *load = UserDefault::getInstance();
			m_iBGMFlag = BGMusic_flag::ON;

			load->setIntegerForKey("BGM_FLAG", m_iBGMFlag);

			//init user total score
			//string sScore;
			//string sDecode;
			//string sEncode;
			//sScore = load->getStringForKey("UserTotalScore", "\0");
			//if (sScore.length() > 0)
			//{
			//	sDecode = this->base64_xxtea_decode(sScore);
			//	long lScore = atol(sDecode.c_str());
			//	if (lScore >= 0 && lScore < 1000000)
			//	{
			//		m_iUserTotalScore = lScore;
			//		sEncode = this->base64_xxtea_encode(sDecode);
			//		load->setStringForKey("UserLastUpdateScore", sEncode);
			//	}
			//	else
			//	{
			//		sEncode = this->base64_xxtea_encode("0");
			//		load->setStringForKey("UserTotalScore", sEncode);
			//		load->setStringForKey("UserLastUpdateScore", sEncode);
			//		m_iUserTotalScore = 0;
			//	}
			//}
			//else
			//{
			//	sEncode = this->base64_xxtea_encode("0");
			//	load->setStringForKey("UserTotalScore", sEncode);
			//	load->setStringForKey("UserLastUpdateScore", sEncode);
			//	m_iUserTotalScore = 0;
			//}

			//init Tool #
			//string sTool;
			//sTool = load->getStringForKey("Total_Tool_Num", "10");
			//m_iToolNum = atoi(sTool.c_str());
			//load->setStringForKey("Total_Tool_Num", sTool); //App side tool is 10
			//sTool = "\0";
			//sTool = load->getStringForKey("UserLastUpdateTools", "0");
			//load->setStringForKey("UserLastUpdateTools", sTool); //server side tools number should be 0
		
			//for(int i = 1; i < 7;i++){
			//	load->setIntegerForKey(TotalPassLevelCountKey[i], 0);
			//}

			//for(int i = 1; i < 7;i++){
			//	load->setIntegerForKey(PlayerPassLevelCountKey[i], 0);
			//}

			//init all game pass level
			//AppDelegate::initGameLevelPassedInfo();	


			//init promotion product
			m_PromoteProduct.m_iIsLoad = 1; //0: load from Server 1:read from local storage
			m_PromoteProduct.m_nCurrid = 1;
			m_PromoteProduct.m_strProductGUID = "";
			m_PromoteProduct.m_strProductName = "";
			m_PromoteProduct.m_strProductURL = "";
			m_PromoteProduct.m_strImage = "";
			load->setIntegerForKey("IsUserDefaultInited", 1);
			load->flush();
		}else if (nSetFlag == 2){ //Reset all values
			iCurrentStep=0;
			iGameMode=0;
			m_iCurrentGameID = 1;
			m_iCurrentGameLevel = 1;
			m_iTotalPassedGameLevel = 0;
			m_Reward = 0;
			m_Refresh = 0;
			m_iShowRegister = 0;

			//UserDefault *load = UserDefault::getInstance();
			m_iBGMFlag = BGMusic_flag::ON;
			load->setIntegerForKey("BGM_FLAG", m_iBGMFlag);

			//init user total score
			//string sScore;
			//string sDecode;
			//string sEncode;
			//sScore = load->getStringForKey("UserTotalScore", "\0");
			//sEncode = this->base64_xxtea_encode("0");
			//load->setStringForKey("UserTotalScore", sEncode);
			//load->setStringForKey("UserLastUpdateScore", sEncode);
			//m_iUserTotalScore = 0;

			////reset tools to 0
			//m_iToolNum = 0;
			//load->setStringForKey("Total_Tool_Num", "0");
			//load->setStringForKey("UserLastUpdateTools", "0");;
	
			//Reset user to empty
			//load->setStringForKey("UserGuid", "\0");
			//load->setIntegerForKey("remember_me", 0);
			//load->setStringForKey("tweebaa_username", "\0");
			//load->setStringForKey("tweebaa_Password", "\0");
			//load->setStringForKey("SyncFlag","0");
			//load->setStringForKey("LocalFlow", "\0");

			//for(int i = 1; i < 7;i++){
			//	load->setIntegerForKey(TotalPassLevelCountKey[i], 0);
			//}

			//for(int i = 1; i < 7;i++){
			//	load->setIntegerForKey(PlayerPassLevelCountKey[i], 0);
			//}

			for(int i = 1; i < 7;i++){
				load->setStringForKey(TutorialIsFirstTime[i],"0");				
			}
			

			//Rest all game pass level
			//load->setStringForKey("LevelInfoFlag", "0");
			//AppDelegate::initGameLevelPassedInfo();	

			//init promotion product
			m_PromoteProduct.m_iIsLoad = 1; //0: load from Server 1:read from local storage
			m_PromoteProduct.m_nCurrid = 1;
			m_PromoteProduct.m_strProductGUID = "";
			m_PromoteProduct.m_strProductName = "";
			m_PromoteProduct.m_strProductURL = "";
			m_PromoteProduct.m_strImage = "";		
			load->setIntegerForKey("IsUserDefaultInited", 1);
			load->flush();
		}
		//reset daily score limit, 20 points per day
		ResetDailyScore();
		bRet = true;
	}while(0);

	return bRet;

}