#ifndef __RegisterTab_SCENE_H__
#define __RegisterTab_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
using namespace cocos2d;
//using namespace CocosDenshion;
using namespace cocos2d::network;


class RegisterTab : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
	LayerMultiplex *layers;
    Menu *tabBarItems;
    Sprite *tabBarBg;
    
    void tapBarMenuPressed(Ref *pSender);
    
    virtual void onEnter();
    // implement the "static create()" method manually
    CREATE_FUNC(RegisterTab);
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */


};

#endif // __RegisterTab_SCENE_H__
