#include "DialogLayer.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "AppDelegate.h"

//USING_NS_CC;


DialogLayer::DialogLayer():
    mMenu(NULL),
    mMenuItemArray(NULL),
    mTouchedMenu(false)
{
}

DialogLayer::~DialogLayer()
{
}



bool DialogLayer::init()
{
    bool bRet = false;
    
    do {

		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 125)));
		
		auto director = Director::getInstance();
		auto size = director->getWinSize();
		CCSprite * bg = CCSprite::create("MsgBox/message_box.png");
		bg->setPosition(size.width / 2, size.height / 2);
        this->addChild(bg);

        onInitDialog();
        initMenu();
		



		//onInitDialog();
		//initMenu();

        bRet = true;
    } while (0);
    
    return bRet;
}


void DialogLayer::pushMenu(MenuItem *pMenuItem)
{
	//if (!mMenuItemArray.) {
 //       mMenuItemArray = Array::create();
 //   }

    //mMenuItemArray->addObject(pMenuItem);
	mMenuItemArray.pushBack(pMenuItem);
}

bool DialogLayer::initMenu()
{

	//if (mMenuItemArray && mMenuItemArray.size() > 0) {
	if (mMenuItemArray.size() > 0) {
		if (!mMenu) {
            mMenu = CCMenu::createWithArray(mMenuItemArray);
			//mMenu = CCMenu::createWithArray(mMenuItemArray);
			//mMenu->setPosition(CCPointZero);
			mMenu->setPosition(Point::ZERO);
            addChild(mMenu);
        }
    }
    return true;
}

void DialogLayer::onEnter()
{

	LayerColor::onEnter();
    // 屏蔽所有priority比自己大的消息
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(DialogLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(DialogLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(DialogLayer::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

}

void DialogLayer::onExit()
{
    LayerColor::onExit();
    //_eventDispatcher->removeEventListener(listener);

}

bool DialogLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    //need to dispatch message to dialog
	if (mMenu) {
        mTouchedMenu = mMenu->onTouchBegan(pTouch, pEvent);
    }
    
    return true;
}

void DialogLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if (mTouchedMenu) {
        if (mMenu) {
            mMenu->onTouchMoved(pTouch, pEvent);
        }
    }
}

void DialogLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if (mTouchedMenu) {
        if (mMenu) {
            mMenu->onTouchEnded(pTouch, pEvent);
        }
        mTouchedMenu = false;
    }
}

void DialogLayer::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if (mTouchedMenu) {
        if (mMenu) {
            mMenu->onTouchEnded(pTouch, pEvent);
        }
        mTouchedMenu = false;
    }
}
