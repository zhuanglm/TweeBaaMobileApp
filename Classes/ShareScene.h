#ifndef __ShareSceneLayer_SCENE_H__
#define __ShareSceneLayer_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
//using namespace CocosDenshion;

class ShareSceneLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
 
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);
	void authMenuItemClick(Ref* pSender);
    
    void cancelAuthMenuItemClick(Ref* pSender);
    
    void hasAuthMenuItemClick(Ref* pSender);
    
    void getUserInfoMenuItemClick(Ref* pSender);
    
    void shareMenuItemClick(Ref* pSender);
    
    void checkClientItemClick(Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(ShareSceneLayer);
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */
	void loadingCallBack(Texture2D *texture);

};

#endif // __ShareSceneLayer_SCENE_H__
