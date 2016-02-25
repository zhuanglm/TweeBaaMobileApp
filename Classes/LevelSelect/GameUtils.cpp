//
//  GameUtils.cpp
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#include "GameUtils.h"
#include "../PriceMatch/PriceMatchScene.h"

#include "../find_diff/FindDiffScene.h"
#include "../zoom/ZoomGameScene.h"
#include "../memory_match/GameScene.h"
#include "../jigsaw/jigSawScene.h"
#include "../MultiPriceMatch/MultiPriceMatchScene.h"

//#include "global_const.h"


cocos2d::Vec2 GameUtils::pointOffsetCenter(float x, float y){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return Vec2(visibleSize.width / 2 + x, visibleSize.height / 2 + y);
}

float GameUtils::getXScaleRate(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return visibleSize.width / 960;
}

float GameUtils::getSpriteXScaleRate(Sprite* sprite){
    float spriteWidth = sprite->getContentSize().width;
    float scaleRate = VISIBLE_WIDTH / spriteWidth;
    if(scaleRate <= 1){
        return 1;
    }else{
        return scaleRate;
    }
}

float GameUtils::getSpriteYScaleRate(Sprite* sprite){
    float spriteHeight = sprite->getContentSize().height;
    float scaleRate = VISIBLE_HEIGHT / spriteHeight;
    if(scaleRate <= 1){
        return 1;
    }else{
        return scaleRate;
    }
}

float GameUtils::getPainSpriteXScaleRate(Sprite* sprite){
    float spriteWidth = sprite->getContentSize().width;
    float scaleRate = VISIBLE_WIDTH / spriteWidth;
    return scaleRate;
}

float GameUtils::getVisibleWidth(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return visibleSize.width;
}

float GameUtils::getVisibleHeight(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return visibleSize.height;
}





Animation* GameUtils::getAnimationByKeyAndFrameName(const std::string& aniKey, const std::string& frameName,
                                                    const int& frameCount, float delay){
    auto animCache = AnimationCache::getInstance();
    auto resultAnimation = animCache->getAnimation(aniKey);
    if( resultAnimation == nullptr){
        auto frameCache = SpriteFrameCache::getInstance();
        Vector<SpriteFrame*> animFrames(frameCount);
        char str[50] = {0};
        for(int i = 0; i < frameCount; i++){
            sprintf(str, frameName.c_str(),i);
            auto frame = frameCache->getSpriteFrameByName(str);
            CCASSERT(frame != nullptr, "frame is null!");
            animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, delay);
        
        // Add an animation to the Cache
        AnimationCache::getInstance()->addAnimation(animation, aniKey);
        animFrames.clear();
        resultAnimation = animCache->getAnimation(aniKey);
        //log("no cache");
    }
    return resultAnimation;
}


Animation* GameUtils::getAnimationByKeyAndFileName(const std::string& aniKey, const std::string& fileName,
                                                   
                                                   
                                                   const int& frameCount, float delay){
    auto animCache = AnimationCache::getInstance();
    auto resultAnimation = animCache->getAnimation(aniKey);
    if( resultAnimation == nullptr){
        auto cache = Director::getInstance()->getTextureCache();
        Vector<SpriteFrame*> animFrames(frameCount);
        char str[50] = {0};
        for(int i = 0; i < frameCount; i++){
            sprintf(str, fileName.c_str(),i);
            auto texture = cache->addImage(str);
            CCASSERT(texture != nullptr, "texture is null!");
            auto size = texture->getContentSize();
            auto rect = Rect(0, 0, size.width, size.height);
            auto frame = SpriteFrame::createWithTexture(texture, rect);
            animFrames.pushBack(frame);
        }
        auto animation = Animation::createWithSpriteFrames(animFrames, delay);
        
        // Add an animation to the Cache
        AnimationCache::getInstance()->addAnimation(animation, aniKey);
        animFrames.clear();
        resultAnimation = animCache->getAnimation(aniKey);
        //log("no cache");
    }
    return resultAnimation;
}

void GameUtils::startGameByLevel(int level){
	/*
    char logBuffer[50] = {0};
    sprintf(logBuffer, "start level:%d",level);
    log(logBuffer);
	*/

	switch(m_iCurrentGameID){
	case 1:{
			auto scene = FindDiffScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		   break;

	case 2:{
			auto scene = ZoomGameScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		  break;
	case 3:{
			

			auto scene = jigSawScene::create();
			
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		   break;
	case 4:{
			auto scene = GameScene::create();
			
			
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		   break;

	case 5:{
			auto scene = PriceMatchScene::create();
			
			
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		   break;
        case 6:
        {
            auto scene = MultiPriceMatchScene::create();
            TransitionScene *transition = TransitionFade::create(1, scene);
            Director::getInstance()->replaceScene(transition);
        }
	}

}
