#include "GameRoundWinScene.h"
//#include "memory_match/memory_match_welcome.h"
#include "ChooseLayer.h"
//#include "ShareScene.h"
#include "CheckoutScene.h"
#include "WelcomeScene.h"

#include "global_const.h"
#include "utilits/pugixml.hpp"
#include "AppDelegate.h"
#include "theme/ThemeLayer.h"
#include "collage/HttpConnection.h"


#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui; 

int iRwd;
int iLcl[6] = { 0, 0, 0, 0, 0, 0 };//last is for theme

Scene* GameRoundWinScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameRoundWinScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameRoundWinScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	  std::string sCSD=app->GetLayoutString();
	 _rootNode = CSLoader::createNode("res/Christmas/ThemePass"+sCSD+".csb");

	 //loginLayer=Layer::create();
	 addChild(_rootNode,1);

	 auto btnContine = dynamic_cast<Button*>(_rootNode->getChildByName("btnContinue")); 
	btnContine->setTag(1001);
	btnContine->addTouchEventListener(CC_CALLBACK_2(GameRoundWinScene::touchButton, this));	


	//ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivPromotionImg"));
	string strPublish = HttpConnection::v_design_like_info.begin()->cache_filename;
	Sprite* sp = Sprite::create(strPublish);
	sp->setAnchorPoint(Vec2(0.5, 0.5));
	sp->setPosition(Vec2(690, 362));
	sp->setScale(0.5);
	_rootNode->addChild(sp, 12);
	//imgPromotion->addChild(sp, 2, 1001);

	string strName = HttpConnection::v_design_like_info.begin()->_objecttitle;
	Text* title = dynamic_cast<Text*>(_rootNode->getChildByName("lblProductName"));
	title->setText(strName);
	//---------------------------------------------------------------------------------------------
	char sKey[100];
	sprintf(sKey, "PassTheme%d_%d", app->iThemeID, app->iRound);
	string strFinishKey = sKey;
	string strPassKey = DataAccess::getParamValue(strFinishKey, "\0");
	if (strPassKey.length() == 0)//never pass
	{
		iRwd = 20;
	}
	else
	{
		iRwd = 5;
	}

	app->UpdateUserScore(1, iRwd);
	long l_coins = app->GetUserTotalScore();
	char p[20];
	sprintf(p, "%ld", l_coins);
	string sTotal = p;

	char p1[20];
	sprintf(p1, "%ld", iRwd);
	string sReward = p1;

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

	syncFlow();//sync with server

	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_WIN);
	}
    return true;
}

void GameRoundWinScene::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
	switch (eventType)
	{
	case ::ui::Widget::TouchEventType::ENDED:

		if (tag == 1001){ //Continue
			AppDelegate *app = (AppDelegate*)Application::getInstance();
			if (app->iGameMode == MobileGameMode::Theme_Mode)
			{
				char sKey[100];
				sprintf(sKey, "PassTheme%d_%d", app->iThemeID, app->iRound);
				string strFinishKey = sKey;
				string strPassKey=DataAccess::getParamValue(strFinishKey,"\0");
				if (strPassKey.length() == 0)
				{//never passed before

					//need +20 points
					DataAccess::setParamValue(strFinishKey, "1");
				}
			}

			auto scene = ThemeLayer::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		break;
	}
}


void GameRoundWinScene::syncFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string  strFlow = updateFlow();//read local and add current

	NetworkType Networ = app->GetNetWorkStatus();
	if (Networ != NetworkTypeNone)
	{
		if (m_UserID > 0 && strFlow.length()>0)
		{
			_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
			HttpRequest* request = new HttpRequest();
			request->setUrl(UCAPIWebPostURL);
			request->setRequestType(HttpRequest::Type::POST);
			request->setResponseCallback(CC_CALLBACK_2(GameRoundWinScene::onSyncCompleted, this));

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

			string s3 = "score = " + strFlow + "&local_point = " + s1 + "&LV = " + s2 + "&UserID = " + userInfo.strUserGuid + "&VirtualPops=" + s4;
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



std::string GameRoundWinScene::updateFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string strLocal = DataAccess::getParamValue("LocalFlow");
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
				iLcl[i] = atoi(sSub.c_str());
			}
			else return "\0";//wrong 
		}
		iLcl[0] += iRwd;
	}
	else  iLcl[0] = iRwd;
	char cFlow[20] = "\0";
	std::string strFlow = "";
	for (int i = 0; i < 6; i++)
	{
		if (iLcl[i]>0)
		{
			sprintf(cFlow, "%d:%d,\0", i + 1, iLcl[i]);
			strFlow += cFlow;
		}
	}
	return strFlow;
}



void GameRoundWinScene::updateLocalFlow()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	char cFlow[20] = "\0";
	std::string strLocal = "";
	for (int i = 0; i < 6; i++)
	{
		sprintf(cFlow, "%d,\0", iLcl[i]);
		strLocal += cFlow;
	}
	strLocal = app->base64_xxtea_encode(strLocal);
	DataAccess::setParamValue("LocalFlow", strLocal);
	DataAccess::setParamValue("SyncFlag", "1");
}

void GameRoundWinScene::clearLocalFlow()
{
	DataAccess::setParamValue("LocalFlow", "\0");
}


void GameRoundWinScene::onSyncCompleted(HttpClient *sender, HttpResponse *response){
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
	if (iScore == 0)
	{
		//get TotalPoints
		strResult = nodes.child_value("TotalPoints");
		app->SaveUserScoreDiff(atoi(strResult.c_str()));
		//get TotalTools
		strResult = nodes.child_value("TotalTools");
		app->SaveUserToolsDiff(atoi(strResult.c_str()));
	}

}
