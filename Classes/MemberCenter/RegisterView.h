#ifndef __RegisterView_SCENE_H__
#define __RegisterView_SCENE_H__

#include "cocos2d.h"
//#include "cocos-ext.h"
#include "network/HttpClient.h"
//#include "CursorTextField.h"
#include "ui/CocosGUI.h"

#include "TwitterWrapper.h"
#include "Stage.h"
#define REGISTER_BACKGROUND "res/public/bg_2.png"

using namespace cocos2d;
 //using namespace cocos2d::extension;
using namespace cocos2d::network;

//using namespace CocosDenshion;

class RegisterView : cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
   // static cocos2d::Scene* createScene();
	//TextFieldTTF* usernameInput;
	
	//RegisterView() : Stage() {};
	/*
	ui::TextField* textEMail; 
	ui::TextField* textUsername; 
	ui::TextField* textPassword; 
	ui::TextField* textPasswordConfirm; 
	*/
	RegisterView();
	~RegisterView();
	//TextBox* editUsername;
	static cocos2d::Scene* createScene(); 
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    // implement the "static create()" method manually
    CREATE_FUNC(RegisterView);
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */
	cocos2d::Node* _rootNode;
	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type,int iRow);
	//void textFieldEvent2(Ref *pSender, cocos2d::ui::TextField::EventType type);

	//void StartGame();
	void loadingCallBack();
	void touchButton(Object* obj, ::ui::Widget::TouchEventType eventType);
	void CallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void ShowMsg(std::string title, std::string msg);
	bool CheckValid(std::string sEMail);

protected:
	std::string sUserName;
	std::string sPassword;
   // virtual void initialize();
};

#endif // __RegisterView_SCENE_H__
