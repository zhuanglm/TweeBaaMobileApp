#include "Login.h"
#include "global_const.h"
#include "utilits/pugixml.hpp"
#include "ChooseLayer.h"
#include "MemberCenter/RegisterView.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "AppDelegate.h"
#include "VisibleRect.h"
#include "utilits/DailyCheckIn.h"
#include "utilits/NetworkUtil.h"
#include "Msgbox/Dialogbox.h"
#include "Msgbox/MsgBox.h"

//#include "facebook\FacebookUserScene.h"

USING_NS_CC;
using namespace ui; 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
using namespace cn::sharesdk;
#endif


// on "init" you need to initialize your instance
Login* dlgLogin;
std::string sThirdPartyUsername;
std::string sThirdPartyEMail;
int iLoginType;

Login::Login(){}
Login::~Login(){}
//------------Check validity of email address----------------
#include <regex>
bool Login::CheckValid(std::string sEMail)
{
	regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
	if (regex_match(sEMail, pattern)) return true;
	return false;
}
//-----------------------------------------------------------

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void login_getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *userInfo, CCDictionary *error)
{
	if (state == C2DXResponseStateSuccess)
	{

		//Output user information
		CCArray *allKeys = userInfo->allKeys();
		//string strEMail,strID;
		for (int i = 0; i < allKeys->count(); i++)
		{
			CCString *key = (CCString *)allKeys->objectAtIndex(i);
			CCObject *obj = userInfo->objectForKey(key->getCString());

			string str_key = key->getCString();
			log("key = %s", str_key.c_str());

			if (dynamic_cast<CCString *>(obj))
			{
				string str_Value = dynamic_cast<CCString *>(obj)->getCString();
				log("value = %s", str_Value.c_str());
				if (iLoginType == 1){ //facebook
					if (str_key == "email") sThirdPartyEMail = str_Value;
					if (str_key == "id") sThirdPartyUsername = "fb_" + str_Value;
				}
				if (iLoginType == 2){ //Twitter
					if (str_key == "screen_name") sThirdPartyEMail = str_Value;
					if (str_key == "id_str") sThirdPartyUsername = "twitter_" + str_Value;
				}
			}
			else if (dynamic_cast<CCInteger *>(obj))
			{
				log("value = %d", dynamic_cast<CCInteger *>(obj)->getValue());
			}
			else if (dynamic_cast<CCDouble *>(obj))
			{
				log("value = %f", dynamic_cast<CCDouble *>(obj)->getValue());
			}
		}

		//Do Login

		dlgLogin->DoThirdPartyLogin();

	}
}
#endif

Scene* Login::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = Login::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}
bool Login::init()
{
	//////////////////////////////  
	// 1. super init first  
	if (!Layer::init())
	{
		return false;
	}
	iLoginType = 0;
	dlgLogin = this;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sCSD = app->GetLayoutString();
	_rootNode = CSLoader::createNode("res/Login/LoginLayer" + sCSD + ".csb");

	//loginLayer=Layer::create();
	addChild(_rootNode);

	auto btnFacebook = dynamic_cast<Button*>(_rootNode->getChildByName("btnFacebook"));
	btnFacebook->setTag(1001);
	btnFacebook->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));
	btnFacebook->setEnabled(false);

	auto btnTwitter = dynamic_cast<Button*>(_rootNode->getChildByName("btnTwitter"));
	btnTwitter->setTag(1002);
	btnTwitter->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));
	btnTwitter->setEnabled(false);

	auto btnCancel = dynamic_cast<Button*>(_rootNode->getChildByName("btnCancel"));
	btnCancel->setTag(2001);
	btnCancel->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));

	auto btnCreate = dynamic_cast<Button*>(_rootNode->getChildByName("btnCreate"));
	btnCreate->setTag(2002);
	btnCreate->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));

	auto btnPlay = dynamic_cast<Button*>(_rootNode->getChildByName("btnPlay"));
	btnPlay->setTag(2003);
	btnPlay->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));

	auto txtLogIn = dynamic_cast<Text*>(_rootNode->getChildByName("collage"));
	txtLogIn->setText("LOG IN");

	auto btnFgtPswd = dynamic_cast<Button*>(_rootNode->getChildByName("btnFgtPswd"));
	btnFgtPswd->setTag(2004);
	btnFgtPswd->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));

	auto textEMail = dynamic_cast<TextField*>(_rootNode->getChildByName("txtEMail"));
	auto textPassword = dynamic_cast<TextField*>(_rootNode->getChildByName("txtPassword"));

	if (textEMail)
	{
		textEMail->setText("");
		textEMail->addEventListener(CC_CALLBACK_2(Login::textFieldEvent, this, 1));
	}
	if (textPassword)
	{
		textPassword->setText("");
		textPassword->addEventListener(CC_CALLBACK_2(Login::textFieldEvent, this, 2));
	}
	if (m_UserID > 0)
	{
		_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);

		string  sUsername = app->base64_xxtea_decode(userInfo.strUserName);
		string  sPass = app->base64_xxtea_decode(userInfo.strPassWd);

		textEMail->setString(sUsername);
		textPassword->setString(sPass);
	}

	Sprite* imgBackground = Sprite::create(LOGIN_BACKGROUND);
	imgBackground->setAnchorPoint(Vec2(0, 0));
	imgBackground->setPosition(Vec2(0, 0));
	this->addChild(imgBackground, -1, 222);

	return true;
}


void Login::textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type, int iRow)
{
	if (type == cocos2d::ui::TextField::EventType::ATTACH_WITH_IME)
	{
		_rootNode->setAnchorPoint(Vec2(0.5, 0.5));
		_rootNode->setPosition(Vec2(_rootNode->getContentSize().width / 2, _rootNode->getContentSize().height / 2 + 100 * iRow));
	}
	else if (type == cocos2d::ui::TextField::EventType::DETACH_WITH_IME)
	{
		_rootNode->setAnchorPoint(Vec2(0.5, 0.5));
		_rootNode->setPosition(Vec2(_rootNode->getContentSize().width / 2, _rootNode->getContentSize().height / 2));
	}
}


void Login::touchButton(Ref* obj, ::ui::Widget::TouchEventType eventType)
{
	auto button = dynamic_cast<::ui::Button*>(obj);
	int tag = button->getTag();
	if (eventType == ::ui::Widget::TouchEventType::ENDED)
	{
		if (tag == 1001)
		{
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				iLoginType = 1;
				C2DXPlatType platType = C2DXPlatType::C2DXPlatTypeFacebook;
				C2DXShareSDK::getUserInfo(platType, login_getUserResultHandler);
			#else
				iLoginType = 1;
				winTestLogin();
			#endif
		}
		if (tag == 1002)
		{
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				iLoginType = 2;
				C2DXPlatType platType = C2DXPlatType::C2DXPlatTypeTwitter;
				C2DXShareSDK::getUserInfo(platType, login_getUserResultHandler);
			#else
				iLoginType = 2;
				winTestLogin();
			#endif
		}
		if (tag == 2001)//Cancel
		{
			//auto scene = ChooseLayer::createScene();
			//TransitionScene *transition = TransitionFade::create(1, scene);
			//Director::getInstance()->replaceScene(transition);
			this->cleanup();
			m_Refresh = 1;
			Director::getInstance()->popScene();
		}
		if (tag == 2002)//register
		{
			//auto scene = RegisterView::createScene();
			//TransitionScene *transition = TransitionFade::create(1, scene);
			//Director::getInstance()->replaceScene(transition);
			this->cleanup();
			m_Refresh = 1;
			Director::getInstance()->popScene();
		}
		if (tag == 2003)//Do Login
		{
			auto textUsername = dynamic_cast<TextField*>(_rootNode->getChildByName("txtEMail"));
			auto textPassword = dynamic_cast<TextField*>(_rootNode->getChildByName("txtPassword"));

			sUsername = textUsername->getString();
			//sUsername = sUsername.replace(sUsername.find("@"), 1, "_");
			sPassword = textPassword->getString();

			//ui::CheckBox* checkBox =(ui::CheckBox*)getChildByTag(2012);
			// Setting HTTP Request Header
			HttpRequest* request = new HttpRequest();
			request->setUrl(UCAPIWebPostURL);
			request->setRequestType(HttpRequest::Type::POST);
			request->setResponseCallback(CC_CALLBACK_2(Login::onHttpRequestCompleted, this));

			// write the post data
			AppDelegate *app = (AppDelegate*)Application::getInstance();
			std::string sPost = "action=login&EncryptData=";
			string s = "username = " + sUsername + "&Password = " + sPassword;
			s = app->encode_data_for_http(s);
			sPost = sPost + s;
			const char* postData = sPost.c_str();
			request->setRequestData(postData, strlen(postData));
			request->setTag("Login POST");
			cocos2d::network::HttpClient::getInstance()->send(request);
			request->release();

			//DoThirdPartyLogin(2,"dragon2934"); //For debug
		}
		if (tag == 2004)//Forgot password
		{
			Application::getInstance()->openURL("https://www.tweebaa.com/User/resetpwd.aspx");
		}
		if (tag == 3001)//bind E-mail
		{
			auto textEMail = dynamic_cast<TextField*>(_popNode->getChildByName("txtEMail"));


			string strEmail = textEMail->getString();
			if (CheckValid(strEmail))
			{
				// write the post data
				//std::string sPost = "action=register&username=" + sThirdPartyUsername + "&e_mail=" + textEMail->getString() + "&Password=twitter";

				HttpRequest* request = new HttpRequest();
				request->setUrl(UCAPIWebPostURL);
				request->setRequestType(HttpRequest::Type::POST);
				request->setResponseCallback(CC_CALLBACK_2(Login::onHttpRegisterRequestCompleted, this));

				AppDelegate *app = (AppDelegate*)Application::getInstance();
				std::string sPost = "action=register&EncryptData=";
				string s = "username = " + sThirdPartyUsername + "&e_mail = " + textEMail->getString() + "&Password =twitter";;
				s = app->encode_data_for_http(s);
				sPost = sPost + s;

				const char* postData = sPost.c_str();
				request->setRequestData(postData, strlen(postData));
				request->setTag("Register POST");
				cocos2d::network::HttpClient::getInstance()->send(request);
				request->release();
			}
			else
			{
				std::string title = "Invalid Email\0";
				std::string msg = "Please check the spelling of your Email address.\0";
				ShowMsg(title, msg);
			}
		}
		if (tag == 3002)//exit
		{
			Layout* layout =dynamic_cast<Layout*>( _popNode->getParent());
			layout->removeFromParentAndCleanup(true);

			
		}
	}
}

void Login::winTestLogin()
{
	if (iLoginType == 1)
	{
		sThirdPartyEMail = "test113@bbb.com";
		sThirdPartyUsername = "fb_11111110";
	}
	else
	{
		sThirdPartyEMail = "11111110";
		sThirdPartyUsername = "twitter_11111110";
	}
	DoThirdPartyLogin();
}

void Login::DoThirdPartyLogin()
{
	//	std::string sThirdPartyUsername;
	std::string sPassword;	
	if(iLoginType==1){ //facebook
		//sThirdPartyUsername=m_strUserInfo;
		sPassword="facebook";
	}
	if(iLoginType==2){ //Twitter
		//sThirdPartyUsername=m_strUserInfo;
		sPassword="twitter";
	}
	
	// Setting HTTP Request Header
	HttpRequest* request = new HttpRequest();
	request->setUrl(UCAPIWebPostURL);
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(Login::onHttpThirdPartyRequestCompleted,this));

	// write the post data
    // std::string sThirdPartyUsername;
    //std::string sThirdPartyEMail;
	//std::string sPost="action=login&username="+sThirdPartyUsername+"&Password="+sPassword;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sPost = "action=login&EncryptData=";
	string s = "username = " + sThirdPartyUsername + "&Password = " + sPassword;
	s = app->encode_data_for_http(s);
	sPost = sPost + s;

	const char* postData = sPost.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Login POST");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}


void Login::onHttpRegisterRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	std::string title = "\0";
	std::string msg = "\0";
	if (!response)
	{
		title = "Network Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
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
		title = "Response Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
		return;
    }
	if (statusCode != 200)
	{
		title = "Response Code Failed\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();

	string sXml(buffer->begin(),buffer->end());
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml,sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppRegister");

	std::string strError=nodes.child_value("error");
	char p[100];
	strcpy(p,strError.c_str());
	if(atoi(p)==0)
	{
		//get user guid
		string strUserGuid=nodes.child_value("UserGuid");
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		sThirdPartyUsername = app->base64_xxtea_encode(sThirdPartyUsername);
		sPassword = app->base64_xxtea_encode(sPassword);
		DataAccess::addUser(strUserGuid, sThirdPartyUsername, sPassword);
		_UserInfo userInfo = DataAccess::getUserInfoByName(sThirdPartyUsername);
		m_UserID = userInfo.nUserID;
		UserDefault::getInstance()->setIntegerForKey("CurrentUserId", m_UserID);
		

		//cocos2d::MessageBox("bind Error","bind Error");
		StartGame();

	}
	if(atoi(p)==-1)
	{
		title = "Input Error\0";
		msg = "E-mail or password incorrect. Please try again.\0";
		ShowMsg(title, msg);
		return;
	}
	if (atoi(p) == -2){
		title = "Input Error\0";
		msg = "E-mail or password incorrect. Please try again.\0";
		ShowMsg(title, msg);
		return;
	}

}

void Login::selectedEvent(Ref* pSender,::ui::CheckBox::EventType type)
{
    switch (type)
    {
        case ::ui::CheckBox::EventType::SELECTED:
			m_bRememberMe=true;
            break;

        case ::ui::CheckBox::EventType::UNSELECTED:
			m_bRememberMe=false;
            break;

        default:
            break;
    }
}

void Login::StartGame(){
	//取用户积分
	m_Refresh = 1;
	Director::getInstance()->popToRootScene();
}

void Login::onHttpThirdPartyRequestCompleted(HttpClient *sender, HttpResponse *response){

	std::string title = "\0";
	std::string msg = "\0";
	if (!response)
	{
		title = "Network Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
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
		title = "Response Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
		return;
	}
	if (statusCode != 200)
	{
		title = "Response Code Failed\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();



	//get the xml
	std::string sXml(buffer->begin(),buffer->end());
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml,sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppLogin");

	std::string strError=nodes.child_value("error");
	char p[100];
	strcpy(p, strError.c_str());

	if(atoi(p)==0)
	{
		//get user guid 
		std::string strUserGuid=nodes.child_value("UserGuid");
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		sThirdPartyUsername = app->base64_xxtea_encode(sThirdPartyUsername);
		sPassword = app->base64_xxtea_encode(sPassword);
		DataAccess::addUser(strUserGuid, sThirdPartyUsername, sPassword);
		_UserInfo userInfo = DataAccess::getUserInfoByName(sThirdPartyUsername);
		m_UserID = userInfo.nUserID;
		UserDefault::getInstance()->setIntegerForKey("CurrentUserId", m_UserID);
		

		SyncData();
	}
	if(atoi(p)==2)
	{
		//facebook /Twitter
        if(iLoginType==1)
		{
            //facebook don't need e-mail
            HttpRequest* request = new HttpRequest();
            request->setUrl(UCAPIWebPostURL);
            request->setRequestType(HttpRequest::Type::POST);
            request->setResponseCallback(CC_CALLBACK_2(Login::onHttpRegisterRequestCompleted,this));
            
            //auto textEMail=dynamic_cast<TextField*>(_popNode->getChildByName("txtEMail"));
            // write the post data
           // std::string sPost="action=register&username="+sThirdPartyUsername+"&e_mail="+sThirdPartyEMail+"&Password=facebook";

			std::string sPost = "action=register&EncryptData=";
			string s = "username=" + sThirdPartyUsername + "&e_mail=" + sThirdPartyEMail + "&Password=facebook";
			//cocos2d::MessageBox("onHttpThirdPartyRequestCompleted", "s");
			AppDelegate *app = (AppDelegate*)Application::getInstance();
			s = app->encode_data_for_http(s);
			sPost = sPost + s;


            const char* postData = sPost.c_str();
            request->setRequestData(postData, strlen(postData));
            request->setTag("Register POST");
            cocos2d::network::HttpClient::getInstance()->send(request);
            request->release();
            
        }
        if(iLoginType==2)
		{ //Twitter need E-Mail address
			Size visiableSize = Director::getInstance()->getVisibleSize();
			//need a popup to get the e-mail address
			::ui::Layout* layout = ::ui::Layout::create();
			layout->setSize(Size(visiableSize.width, visiableSize.height));
			layout->setBackGroundColorType(::ui::LAYOUT_COLOR_SOLID);
			layout->setBackGroundColor(ccc3(150,150,255));
			layout->setPosition(Vec2(0, 0));
			addChild(layout,999);

			_popNode= CSLoader::createNode("res/Login/EMailLayer.csb");
			layout->addChild(_popNode,1);
			auto btnBind = dynamic_cast<Button*>(_popNode->getChildByName("Button_8"));
	 		btnBind->setTag(3001);
			btnBind->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));

			auto btnExit = dynamic_cast<Button*>(_popNode->getChildByName("Button_7"));
			btnExit->setTag(3002);
			btnExit->addTouchEventListener(CC_CALLBACK_2(Login::touchButton, this));

        }
	}

}

void Login::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{

	std::string title = "\0";
	std::string msg = "\0";
	if (!response)
	{
		title = "Network Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
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
		title = "Response Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
		return;
	}
	if (statusCode != 200)
	{
		title = "Response Status Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsg(title, msg);
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
		
	//get the xml
	std::string sXml(buffer->begin(),buffer->end());
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml,sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppLogin");

	std::string strError=nodes.child_value("error");

	char p[100];
	strcpy(p,strError.c_str());
	if(atoi(p)!=0)
	{

		title = "Input Error\0";
		msg = "E-mail or password incorrect. Please try again.\0";
		ShowMsg(title, msg);
		return;
	}
	else
	{
		//get user guid
		std::string strUserGuid = nodes.child_value("UserGuid");
		std::string strUserName = nodes.child_value("UserName");
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		strUserName = app->base64_xxtea_encode(strUserName);
		sPassword = app->base64_xxtea_encode(sPassword);
		DataAccess::addUser(strUserGuid, strUserName, sPassword);
		_UserInfo userInfo = DataAccess::getUserInfoByName(strUserName);
		m_UserID = userInfo.nUserID;
		UserDefault::getInstance()->setIntegerForKey("CurrentUserId", m_UserID);
	}
	SyncData();
	//StartGame();

}

void Login::ShowMsg(std::string title, std::string msg)
{
	MsgBoxType m_type = TP_INFO;
	MsgBox::setMsgBox(m_type, &title, &msg);
	auto msgbox = MsgBox::create();
	msgbox->setTag(MSGBOX_TAG);
	this->addChild(msgbox, 999);

	//Add Event to process
	auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
	btnOk->addTouchEventListener(CC_CALLBACK_2(Login::CallBack, this));

}

void Login::CallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		this->removeChildByTag(MSGBOX_TAG);
	}
}

void Login::SyncData()
{

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	NetworkType Networ = app->GetNetWorkStatus();
	std::string  strFlow = "0:0";//read local and add current
	if (Networ != NetworkTypeNone)
	{
		if (m_UserID > 0)
		{
			_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
			HttpRequest* request = new HttpRequest();
			request->setUrl(UCAPIWebPostURL);
			request->setRequestType(HttpRequest::Type::POST);
			request->setResponseCallback(CC_CALLBACK_2(Login::onSyncComplete, this));

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


void Login::onSyncComplete(HttpClient *sender, HttpResponse *response)
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
		return;
	}

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
	StartGame();
}
