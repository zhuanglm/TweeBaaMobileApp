#include "RegisterView.h"
#include "ChooseLayer.h"
#include "global_const.h"
#include "../utilits/pugixml.hpp"
#include "Login.h"
#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
//#include "../Msgbox/Dialogbox.h"
#include "../Msgbox/MsgBox.h"

USING_NS_CC;
using namespace ui; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
using namespace cn::sharesdk;
#endif

std::string sThirdPartyUsername2;
std::string sThirdPartyEMail2;
int iLoginType2;
extern Login* dlgLogin;

//------------Check validity of email address----------------
#include <regex>
bool RegisterView::CheckValid(std::string sEMail)
{
	regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
	if (regex_match(sEMail, pattern)) return true;
	return false;
}
//-----------------------------------------------------------

RegisterView::RegisterView(){

}
RegisterView::~RegisterView(){

}

Scene* RegisterView::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();    
    // 'layer' is an autorelease object
    auto layer = RegisterView::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
void reg_getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *userInfo, CCDictionary *error)
{
    if (state == C2DXResponseStateSuccess)
    {
		//cocos2d::MessageBox("getUserResultHandler","start");
        //Output user information
        CCArray *allKeys = userInfo -> allKeys();
        //string strEMail,strID;
        for (int i = 0; i < allKeys -> count(); i++)
        {
            CCString *key = (CCString *)allKeys -> objectAtIndex(i);
            CCObject *obj = userInfo -> objectForKey(key -> getCString());
            
            string str_key=key -> getCString();
            log("key = %s", str_key.c_str());
             
            if (dynamic_cast<CCString *>(obj))
            {
                string str_Value=dynamic_cast<CCString *>(obj) -> getCString();
                log("value = %s",str_Value.c_str()) ;
                if(iLoginType2==1){ //facebook
                    if(str_key=="email") sThirdPartyEMail2=str_Value;
					if(str_key=="id") sThirdPartyUsername2="fb_"+str_Value;
					//cocos2d::MessageBox("getUserResultHandler","fb");
                }
                if(iLoginType2==2){ //Twitter
                    if(str_key=="screen_name") sThirdPartyEMail2=str_Value;
					if(str_key=="id_str") sThirdPartyUsername2="twitter_"+str_Value;
					//cocos2d::MessageBox("getUserResultHandler","tt");
                }
            }
            else if (dynamic_cast<CCInteger *>(obj))
            {
				log("value = %d", dynamic_cast<CCInteger *>(obj) -> getValue());
				//cocos2d::MessageBox("getUserResultHandler","d");
            }
            else if (dynamic_cast<CCDouble *>(obj))
            {
				log("value = %f", dynamic_cast<CCDouble *>(obj) -> getValue());
				//cocos2d::MessageBox("getUserResultHandler","f");
            }
        }
        //Do Login
		//cocos2d::MessageBox("getUserResultHandler","end");
        dlgLogin->DoThirdPartyLogin();   
    }
}
#endif

// on "init" you need to initialize your instance
bool RegisterView::init() 
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sCSD=app->GetLayoutString();

    _rootNode = CSLoader::createNode("res/register/RegisterLayer"+sCSD+".csb");

	addChild(_rootNode);

	auto btnFacebook = dynamic_cast<Button*>(_rootNode->getChildByName("btnFacebook"));
	btnFacebook->setTag(1001);
	btnFacebook->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	btnFacebook->setEnabled(false);

	auto btnTwitter = dynamic_cast<Button*>(_rootNode->getChildByName("btnTwitter"));
	btnTwitter->setTag(1002);
	btnTwitter->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	btnTwitter->setEnabled(false);

	auto btnBack2Login = dynamic_cast<Button*>(_rootNode->getChildByName("btnBack2Login"));
	if (btnBack2Login){
		btnBack2Login->setTag(2003);
		btnBack2Login->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	}
	auto btnCreate = dynamic_cast<Button*>(_rootNode->getChildByName("btnCreate"));
	if (btnCreate){
		btnCreate->setTag(2002);
		btnCreate->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	}
	auto btnCancel = dynamic_cast<Button*>(_rootNode->getChildByName("btnCancel"));
	if (btnCancel){
		btnCancel->setTag(2001);
		btnCancel->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	}
	auto textEMail = dynamic_cast<TextField*>(_rootNode->getChildByName("txtEMail"));
	auto textUserName = dynamic_cast<TextField*>(_rootNode->getChildByName("txtUserName"));
	auto textPassword=dynamic_cast<TextField*>(_rootNode->getChildByName("txtPassword"));
	auto textPasswordConfirm=dynamic_cast<TextField*>(_rootNode->getChildByName("txtConfPassword"));

	if (textEMail)
	{
		textEMail->setText("");
		textEMail->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this, 1));
	}

	if (textUserName)
	{
		textUserName->setText("");
		textUserName->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this, 2));
	}
	if (textPassword)
	{
		textPassword->setText("");
		textPassword->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this, 3));
	}
	if (textPasswordConfirm)
	{
		textPasswordConfirm->setText("");
		textPasswordConfirm->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this, 3));
	}
	auto btnTerm = dynamic_cast<Button*>(_rootNode->getChildByName("btnTerm"));
	auto btnPolicy = dynamic_cast<Button*>(_rootNode->getChildByName("btnPolicy"));

	if (btnTerm){
		btnTerm->setTag(2101);
		btnTerm->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	}
	if (btnPolicy){
		btnPolicy->setTag(2102);
		btnPolicy->addTouchEventListener(CC_CALLBACK_2(RegisterView::touchButton, this));
	}

	Sprite* imgBackground = Sprite::create(REGISTER_BACKGROUND);
	imgBackground->setAnchorPoint(Vec2(0, 0));
	imgBackground->setPosition(Vec2(0, 0));
	this->addChild(imgBackground, -1, 222);

	return true;
	
}
void RegisterView::textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type,int iRow)
{
    if (type == cocos2d::ui::TextField::EventType::ATTACH_WITH_IME)
    {
		_rootNode->setAnchorPoint(Vec2(0.5,0.5));
		_rootNode->setPosition(Vec2(_rootNode->getContentSize().width/2,_rootNode->getContentSize().height/2 +100*iRow));
    }
	else if (type == cocos2d::ui::TextField::EventType::DETACH_WITH_IME)
    {
		_rootNode->setAnchorPoint(Vec2(0.5,0.5));
		_rootNode->setPosition(Vec2(_rootNode->getContentSize().width/2,_rootNode->getContentSize().height/2 ));
    }
}

void RegisterView::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj); 
    int tag = button->getTag();  
	if (eventType == ::ui::Widget::TouchEventType::ENDED)
	{
		if(tag==1001)//facebook
		{
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

				iLoginType2=1;
				C2DXPlatType platType=C2DXPlatType::C2DXPlatTypeFacebook;
				C2DXShareSDK::getUserInfo(platType, reg_getUserResultHandler);

			#endif
		}

		if(tag==1002)//twitter
		{ 
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

				iLoginType2=2;
				C2DXPlatType platType=C2DXPlatType::C2DXPlatTypeTwitter;
				C2DXShareSDK::getUserInfo(platType, reg_getUserResultHandler);
        
			#endif
		}

		if (tag == 2001)//Cancel,go back to chooselayer
		{
			/*
			auto scene = ChooseLayer::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);*/
			this->cleanup();
			m_Refresh = 1;
			Director::getInstance()->popScene();

		}
		if(tag==2002)//Do Register
		{
			auto textEMail = dynamic_cast<TextField*>(_rootNode->getChildByName("txtEMail"));
			auto textUserName = dynamic_cast<TextField*>(_rootNode->getChildByName("txtUserName"));
			auto textPassword=dynamic_cast<TextField*>(_rootNode->getChildByName("txtPassword"));
			auto textPasswordConfirm=dynamic_cast<TextField*>(_rootNode->getChildByName("txtConfPassword"));
			sPassword = textPassword->getString();
			std::string sPasswordConf = textPasswordConfirm->getString();
			if(sPassword==sPasswordConf)
			{

				std::string sEMail = textEMail->getString();
				sUserName = textUserName->getString();
				if (!CheckValid(sEMail))
				{
					std::string title = "Input Error\0";
					std::string msg = "Please input a valid E-Mail.\0";
					ShowMsg(title, msg);
				}
				else
				{

					// Setting HTTP Request Header
					HttpRequest* request = new HttpRequest();
					request->setUrl(UCAPIWebPostURL);
					request->setRequestType(HttpRequest::Type::POST);
					request->setResponseCallback(CC_CALLBACK_2(RegisterView::onHttpRequestCompleted, this));

					// write the post data
					//std::string sPost = "action=register&username=" + sUserName + "&e_mail=" + sEMail + "&Password=" + sPassword;
					AppDelegate *app = (AppDelegate*)Application::getInstance();
					std::string sPost = "action=register&EncryptData=";
					string s = "username = " + sUserName + "&e_mail = " + sEMail + "&Password = " + sPassword;;
					s = app->encode_data_for_http(s);
					sPost = sPost + s;

					const char* postData = sPost.c_str();
					request->setRequestData(postData, strlen(postData));
					request->setTag("Register POST");
					cocos2d::network::HttpClient::getInstance()->send(request);
					request->release();
				}
			}
			else
			{
				std::string title = "Input Error\0";
				std::string msg = "Password and Confirm Password Not Match.\0";
				ShowMsg(title, msg);
			}

		}
		if(tag==2003)//go to login
		{
			auto scene = Login::createScene();
			//TransitionScene *transition = TransitionFade::create(1, scene);
			//Director::getInstance()->replaceScene(transition);
			Director::getInstance()->pushScene(scene);
			
		}
		if (tag == 2101)//Terms
		{
			Application::getInstance()->openURL("https://tweebaa.com/College/Genernal_Terms_ofUse.aspx");
		}
		if (tag == 2102)//Policy
		{
			Application::getInstance()->openURL("https://tweebaa.com/College/Privacy_Policy.aspx");
		}
	}
}

void RegisterView::CallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		this->removeChildByTag(MSGBOX_TAG);
		//msg->removeFromParentAndCleanup(true);
		//auto scene = RegisterView::createScene();
		//TransitionScene *transition = TransitionFade::create(1, scene);
		//Director::getInstance()->pushScene(transition);
		//Director::getInstance()->replaceScene(scene);
	}
}
void RegisterView::ShowMsg(std::string title, std::string msg)
{
	MsgBoxType m_type = TP_INFO;
	MsgBox::setMsgBox(m_type, &title, &msg);
	auto msgbox = MsgBox::create();
	msgbox->setTag(MSGBOX_TAG);
	this->addChild(msgbox, 999);

	//Add Event to process
	auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
	btnOk->addTouchEventListener(CC_CALLBACK_2(RegisterView::CallBack, this));

}

void RegisterView::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	std::string title = "\0";
	std::string msg = "\0";
    if (!response)
	{
		title = "Register Error\0";
		msg = "Register Error.\0";
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

    log("response code: %d", statusCode);   
    if (!response->isSucceed())
    {
        log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		title = "Register Error\0";
		msg = "Register Error.\0";
		ShowMsg(title, msg);
        return;
    }
	if (statusCode != 200){
		log("response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		title = "Register Error\0";
		msg = "Register Error.\0";
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
		/*UserDefault::getInstance()->setStringForKey("UserGuid", strUserGuid);
		UserDefault::getInstance()->setIntegerForKey("remember_me", 1);
		UserDefault::getInstance()->setStringForKey("tweebaa_username", sUserName);
		UserDefault::getInstance()->setStringForKey("tweebaa_Password", sPassword);
		UserDefault::getInstance()->flush();*/


		AppDelegate *app = (AppDelegate*)Application::getInstance();
		sUserName = app->base64_xxtea_encode(sUserName);
		sPassword = app->base64_xxtea_encode(sPassword);

		string strTool = DataAccess::getParamValue("Total_Tool_Num","\0");
		int iTool=0;
		if (strTool.length() > 0)
		{
			iTool = atoi(app->base64_xxtea_decode(strTool).c_str());
		}

		DataAccess::addUser(strUserGuid, sUserName, sPassword);
		_UserInfo userInfo = DataAccess::getUserInfoByName(sUserName);
		m_UserID = userInfo.nUserID;
		UserDefault::getInstance()->setIntegerForKey("CurrentUserId", m_UserID);

		app->UpdateUserScore(1, 50);
		char sTool[10];
		sprintf(sTool, "%d", iTool + 10);
		strTool = sTool;
		strTool = app->base64_xxtea_encode(strTool);
		DataAccess::setParamValue("Total_Tool_Num", strTool);
		//get it
		//string s=CCUserDefault::sharedUserDefault()->getStringForKey("UserGuid");
		//auto scene = CollageMainTab::createScene();
		/*auto scene = ChooseLayer::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);*/

		this->cleanup();
		m_Refresh = 1;
		Director::getInstance()->popScene();
	}
	if(atoi(p)==-1){

		title = "Input Error\0";
		msg = "E-mail or username already exists, please choose another one.\0";
		ShowMsg(title, msg);
	}
	if (atoi(p) == -2){
		title = "Input Error\0";
		msg = "Register Failed, please try another e-mail or username.\0";
		ShowMsg(title, msg);
	}
}

//Loading Call back
void RegisterView::loadingCallBack()
{
	/*
	auto scene = ChooseLayer::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);*/

	this->cleanup();
	m_Refresh = 1;
	Director::getInstance()->popScene();
}

/*
void RegisterView::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
*/