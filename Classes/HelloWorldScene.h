#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
using namespace cocos2d;
//using namespace CocosDenshion;
using namespace cocos2d::network;
#include "CCAlert.h"

class HelloWorld : public cocos2d::Layer, public CCAlertDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
	void onOk(CCAlert* pSender);
	void onCancel(CCAlert* pSender);
	void StartGame();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void AutoCheckVersion();
	void ShowSplash(float dt);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	void checknetworkdelay(float dt);
	void checknetwork();
	void loadingCallBack(Texture2D *texture);
	void onHttpRequestLoadRoundCompleted(HttpClient *sender, HttpResponse *response);
	
	void menuCallbackPopupNo();
	void menuCallbackPopupYes();
	void menuCallbackPopupOk();
};

#endif // __HELLOWORLD_SCENE_H__
