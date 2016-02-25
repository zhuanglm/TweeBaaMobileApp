#ifndef __GameRoundWin_SCENE_H__
#define __GameRoundWin_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
//using namespace CocosDenshion;
using namespace cocos2d::network;

class GameRoundWinScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    // implement the "static create()" method manually
    CREATE_FUNC(GameRoundWinScene);
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */

	cocos2d::Node* _rootNode;
	void touchButton(Object* obj, ::ui::Widget::TouchEventType eventType);

	void syncFlow();
	std::string updateFlow();
	void updateLocalFlow();
	void clearLocalFlow();
	void onSyncCompleted(HttpClient *sender, HttpResponse *response);

};

#endif // __GameWin_SCENE_H__
