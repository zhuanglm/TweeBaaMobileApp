#ifndef __Login_SCENE_H__
#define __Login_SCENE_H__

//#include "cocos2d.h"
//#include "cocos-ext.h"
#include "network/HttpClient.h"
//#include "CursorTextField.h"
#include "ui/CocosGUI.h"

#include "TwitterWrapper.h"
#define LOGIN_BACKGROUND "res/public/bg_2.png"

//#include "Stage.h"

using namespace cocos2d;
 //using namespace cocos2d::extension;
using namespace cocos2d::network;



class Login : public cocos2d::Layer
{
public:
	//Login() : Stage() {};
	Login();
    ~Login();
	static cocos2d::Scene* createScene(); 
	
    virtual bool init();  
    // a selector callback
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);
   // void menuCloseCallback(cocos2d::Ref* pSender);
	void selectedEvent(Ref* pSender,::ui::CheckBox::EventType type);
    //void menuStartCallback_Game1(Ref *sender);

	//void callBackRegister(Ref * sender);
	void DoThirdPartyLogin();
    // implement the "static create()" method manually
    //CREATE_SCENE(Login);
	CREATE_FUNC(Login); 
private:
	//Layer* loginLayer;
	cocos2d::Node* _rootNode;
	cocos2d::Node* _popNode;
	bool m_bRememberMe;
	void ShowMsg(std::string title, std::string msg);
	void StartGame();
	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type,int iRow);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	void onHttpRegisterRequestCompleted(HttpClient *sender, HttpResponse *response);
	void onHttpThirdPartyRequestCompleted(HttpClient *sender, HttpResponse *response);
	void SyncData();
	void onSyncComplete(HttpClient *sender, HttpResponse *response);

	//void onHttpRequestGetDailyCheckInCompleted(HttpClient *sender, HttpResponse *response);

	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */
	void loadingCallBack(Texture2D *texture);
	void CallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	
	void winTestLogin();
	bool CheckValid(std::string sEMail);


protected:
   // virtual void initialize();
	std::string sUsername;
	std::string sPassword;

	
};

#endif // __Login_SCENE_H__
