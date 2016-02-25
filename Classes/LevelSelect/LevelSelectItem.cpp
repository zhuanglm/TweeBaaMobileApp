//
//  LevelSelectItem.cpp
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#include "LevelSelectItem.h"
#include "HelloWorldScene.h"
#include "GameUtils.h"
#include "global_const.h"
//#include "GameSound.h"

LevelSelectItem::LevelSelectItem(int level):_level(level),_type(kLockLevel){
    if(level > (g_passLevelCount + 1)){
        this->setEnabled(false);
        _type = kLockLevel;
    }else if(level == (g_passLevelCount + 1)){
        this->setEnabled(true);
        _type = kNotPassYet;
    }else if(level < (g_passLevelCount + 1)){
        this->setEnabled(true);
        _type = kAlreadyPass;
    }
}


LevelSelectItem* LevelSelectItem::create(int level){
    LevelSelectItem *ret = new LevelSelectItem(level);
    auto callback = CC_CALLBACK_1(LevelSelectItem::selectCallBack, ret);
    
    
    auto normalSprite = Sprite::createWithSpriteFrameName(ret->getFrameNameByType(ret->_type));
    auto selectedSprite = Sprite::createWithSpriteFrameName(ret->getFrameNameByType(ret->_type));
    auto disabledSprite = Sprite::createWithSpriteFrameName(ret->getFrameNameByType(ret->_type));
    ret->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
    ret->extraInit();
    ret->autorelease();
    return ret;
}

void LevelSelectItem::selectCallBack(Ref* sender){
    if(_type != kLockLevel){
        log("level item");
        m_iCurrentGameLevel = _level;
        
        GameUtils::startGameByLevel(_level);
        //auto scene = HelloWorld::createScene();
        //Director::getInstance()->replaceScene( TransitionFade::create(0.5, scene));
    }
}

void LevelSelectItem::extraInit(){
    
    
    if(_type != kLockLevel){
        char levelBuffer[20] = {0};
        sprintf(levelBuffer, "%d", _level);
        Label* valueLabel = nullptr;
        if(_type == kAlreadyPass){
            valueLabel = Label::createWithBMFont("fonts/blueLevel.fnt", levelBuffer);
            
        }else if(_type == kNotPassYet){
            valueLabel = Label::createWithBMFont("fonts/whiteLevel.fnt", levelBuffer);
        }
        this->addChild(valueLabel);
        auto size = this->getContentSize();
        valueLabel->setPosition(size.width * 0.5, size.height * 0.5);
        
    }
    
    
}

std::string LevelSelectItem::getFrameNameByType(const LevelItemType& type){
    switch (type) {
        case kLockLevel:
        {
            return "locked.png";
        }
            break;
        case kNotPassYet:
        {
            return "notPassYet.png";
        }
            break;
        case kAlreadyPass:
        {
            return "pass.png";
        }
            break;
        default:
            break;
    }
}