#ifndef __GameFailed_SCENE_H__
#define __GameFailed_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "global_const.h"

using namespace cocos2d;
//using namespace CocosDenshion;
using namespace cocos2d::network;
class GameFailed : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
	void menuCallback(Ref *sender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuCallbackBuy(Ref *sender);
	void menuCallbackShare(Ref *sender);
	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    // implement the "static create()" method manually
    CREATE_FUNC(GameFailed);
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */

	cocos2d::Node* _rootNode;
	cocos2d::Node* _quitNode;
	PromoteProduct m_PrmtPrdct;

	

	void loadingCallBack(Texture2D *texture);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	void onHttpLoadImageCompleted(HttpClient *sender, HttpResponse *response);
	void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onHttpRequestShareCompleted(HttpClient *sender, HttpResponse *response);

	void menuCallbackPopupNo(Ref *sender);
	void menuCallbackPopupYes(Ref *sender);

	void showRegister();
	void CallbackRegisterYes(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void CallbackRegisterNo(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);


};

#endif // __GameFailed_SCENE_H__
