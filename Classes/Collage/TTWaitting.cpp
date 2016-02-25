#include "TTWaitting.h"

TTWaitting::TTWaitting()
{
    
}

TTWaitting::~TTWaitting()
{
    
}

 bool  TTWaitting::initWithBgImage(std::string  strImageName)
{
    if (strImageName.empty() )
    {
        return false;
    }
    
    m_bgSprite =  Sprite::create(strImageName.c_str());

    m_bgSprite->setPosition(Vec2(
                                m_bgSprite->getContentSize().width * 0.5f,
                                m_bgSprite->getContentSize().height * 0.5f
                                ));
    this->addChild(m_bgSprite);
    
    //设置contentSize
    this->setContentSize(m_bgSprite->getContentSize());
    
    //将锚点设置回0.5,0.5
    this->setAnchorPoint(Vec2(0.5f,0.5f));
    
    return  true;
}


void   TTWaitting::startAnimation()
{
    if (m_bgSprite)
    {
        m_bgSprite->stopAllActions();
        
        RotateBy   *  rotateAct =  RotateBy::create(2.5f, 360);
        RepeatForever  *   rotateRep =  RepeatForever::create(rotateAct);
        m_bgSprite->runAction(rotateRep);
    }
}


void   TTWaitting::stopAnimation()
{
    if (m_bgSprite)
    {
        m_bgSprite->stopAllActions();
    }
}


TTWaitting *  TTWaitting::createWithBgImage(std::string  strImageName)
{
    if (strImageName.empty() )
    {
        return  NULL;
    }
    
    TTWaitting  *   pRet = new  TTWaitting();
    
    if (pRet  != NULL  &&  pRet->initWithBgImage(strImageName))
    {
        pRet->autorelease();
        
    }
    else
    {
        delete   pRet;
        pRet = NULL;
    }

    return  pRet;
}

   