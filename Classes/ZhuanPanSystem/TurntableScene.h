#ifndef __TurntableScene_H__
#define __TurntableScene_H__

#include "cocos2d.h"

using namespace cocos2d;


class TurntableScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    

    CREATE_FUNC(TurntableScene);
private:
	/**
     * Called when the texture (exp. atlas.png) is loading finish, will call this function
     * this function finish build all the SpritFrame by using AtlasLoader
     */
	
};

#endif // __GameWin_SCENE_H__
