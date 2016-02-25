//
//  GameUtils.h
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#ifndef __PageTest__GameUtils__
#define __PageTest__GameUtils__

#include "cocos2d.h"

USING_NS_CC;
 
class GameUtils{
public:
    static cocos2d::Vec2 pointOffsetCenter(float x, float y);
    static float getXScaleRate();
    static float getSpriteXScaleRate(Sprite* sprite);
    static float getSpriteYScaleRate(Sprite* sprite);
    static float getPainSpriteXScaleRate(Sprite* sprite);
    static float getVisibleWidth();
    static float getVisibleHeight();
    
    static Animation* getAnimationByKeyAndFrameName(const std::string& aniKey, const std::string& frameName, const int& frameCount, float delay);
    static Animation* getAnimationByKeyAndFileName(const std::string& aniKey, const std::string& fileName, const int& frameCount, float delay);
    static void startGameByLevel(int level);
};


#define GameCCPointC(x,y) GameUtils::pointOffsetCenter(x,y)
#define GetXScaleRate GameUtils::getXScaleRate()
#define GetSpriteXScaleRate(sprite) GameUtils::getSpriteXScaleRate(sprite)
#define GetSpriteYScaleRate(sprite) GameUtils::getSpriteYScaleRate(sprite)
#define GetPainSpriteXScaleRate(sprite) GameUtils::getPainSpriteXScaleRate(sprite)
#define VISIBLE_WIDTH GameUtils::getVisibleWidth()
#define VISIBLE_HEIGHT GameUtils::getVisibleHeight()

#endif /* defined(__PageTest__GameUtils__) */
