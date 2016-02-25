#include "Dialogbox.h"
#include "cocos2d.h"

int DlgBox::msgtype = 1;
std::string *DlgBox::m_Msgbody = nullptr;
std::string *DlgBox::m_Title = nullptr;


bool DlgBox::onInitDialog()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	//std::string msg = *m_Msg;
	//if (msgtype == 1) {
	//	msg = MSG_NETWORK_NONETWORK;
	//}else if (msgtype == 2){
	//	msg = MSG_NETWORK_3G;
	//}else if (msgtype == 3){
	//	msg = MSG_NETWORK_WIFI;	
	//}

	auto chatTextSize = CCSizeMake(300,200);
	CCLabelTTF *label = CCLabelTTF::create("", "Message", 20, chatTextSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	//auto *label = Label::create("", "", 24);
	//label->setString(msg);
	label->setString(*DlgBox::m_Msgbody);
	label->setColor(ccc3(0,0,0));
    label->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 20));
    this->addChild(label,99);

	//if (msgtype == 1) {
	//	cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("/public/yes.png","/public/yes.png",this,menu_selector(DlgBox::yesMenuItemCallback));
	//	okMenuItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 90));
	//	pushMenu(okMenuItem);
	//	//CCMenuItemFont *okMenuItem = CCMenuItemFont::create("OK", this, menu_selector(DlgBox::okMenuItemCallback));
	//	//okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 50));

	//	//CCMenuItemFont *cancelMenuItem = CCMenuItemFont::create("Cancel", this, menu_selector(DlgBox::cancelMenuItemCallback));
	//	//cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 50));

	//} else if (msgtype == 2){
	//	cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("/public/yes.png","/public/yes.png",this,menu_selector(DlgBox::yesMenuItemCallback));
	//	okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 90));

	//	cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("/public/no.png","/public/no.png",this,menu_selector(DlgBox::noMenuItemCallback));
	//	cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 90));
	//	pushMenu(okMenuItem);
	//	pushMenu(cancelMenuItem);

	//}else {
	//	cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("/public/no.png","/public/no.png",this,menu_selector(DlgBox::noMenuItemCallback));
	//	cancelMenuItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 90));
	//	pushMenu(cancelMenuItem);
	//}

	if (msgtype == TP_Q_YESNO) {
		cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("res/MsgBox/message_btn.png","res/MsgBox/message_btn.png",this,menu_selector(DlgBox::yesMenuItemCallback));
		okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 90));

		cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("res/MsgBox/message_btn.png","res/MsgBox/message_btn.png",this,menu_selector(DlgBox::noMenuItemCallback));
		cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 90));

		pushMenu(okMenuItem);
		pushMenu(cancelMenuItem);

	} else if (msgtype == TP_Q_OKCANCEL){
		cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("res/public/yes.png","res/public/yes.png",this,menu_selector(DlgBox::okMenuItemCallback));
		okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 90));

		cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("res/public/no.png","res/public/no.png",this,menu_selector(DlgBox::cancelMenuItemCallback));
		cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 90));
		pushMenu(okMenuItem);
		pushMenu(cancelMenuItem);

	}else if (msgtype = TP_INFO){
		cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("res/public/yes.png","res/public/yes.png",this,menu_selector(DlgBox::okMenuItemCallback));
		okMenuItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 90));
		pushMenu(okMenuItem);
	}


    //setColor(ccc3(255,178,102));
    setOpacity(128);

    return true;
}

void DlgBox::okMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_OK);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
	
}

void DlgBox::cancelMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_CANCEL);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
}

void DlgBox::yesMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_YES);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
}

void DlgBox::noMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_NO);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
}

void DlgBox::update(float delta) {
	auto position = this->getChildByTag(199)->getPosition();
	auto sp = (Sprite*) this->getChildByTag(199);
	//position.x -= 250 * delta;
	//if (position.x  < 0 - (this->getChildByTag(199)->getBoundingBox().size.width / 2))
	//	position.x = this->getBoundingBox().getMaxX() + sp->getBoundingBox().size.width/2;
	//sp->setPosition(position);

}

bool DlgBox::test(){

 //if (!LayerColor::initWithColor(cocos2d::Color4B::BLACK))
 //   {
 //       return false;
 //   }
    
   //label = cocos2d::Label::createWithSystemFont("Press space to pause all, 1 to pause left", "Arial", 30);
   //label->setPosition(cocos2d::Vec2(this->getBoundingBox().getMidX(), this->getBoundingBox().getMaxY() - 20));

   //sprite = cocos2d::Sprite::create("/games_lance/no.png");
   //sprite2 = cocos2d::Sprite::create("/games_lance/no.png");
   //sprite->setPosition(250, this->getBoundingBox().getMidY());
   //sprite2->setPosition(700, this->getBoundingBox().getMidY());

   //auto rotate = cocos2d::RotateBy::create(1, 45);
   //auto rotate2 = cocos2d::RotateBy::create(1, -45);

   //auto repeat1 = cocos2d::RepeatForever::create(rotate);
   //auto repeat2 = cocos2d::RepeatForever::create(rotate2);

   //this->addChild(label,0);
   //this->addChild(sprite, 0);
   //this->addChild(sprite2, 0);
   // 
   //sprite->runAction(repeat1);
   //sprite2->runAction(repeat2);
   //auto listener = cocos2d::EventListenerKeyboard::create();
   //listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)->void{
   //   // On Spacebar, Pause/Unpause all actions and updates
   //   if (code == cocos2d::EventKeyboard::KeyCode::KEY_SPACE){
   //      if (pausedNodes.size()){
   //         cocos2d::Director::getInstance()->getActionManager()->resumeTargets(pausedNodes);
   //         pausedNodes.clear();
   //         spritePaused = false; // In case user currently has 1 pressed too
   //      }
   //      else
   //         pausedNodes = cocos2d::Director::getInstance()->getActionManager()->pauseAllRunningActions();
   //      label->setString("Spacebar pressed");
   //   }
   //   // Pause/UnPause just sprite 1
   //   if (code == cocos2d::EventKeyboard::KeyCode::KEY_1){
   //      if (spritePaused)
   //         sprite->resumeSchedulerAndActions();
   //      else
   //         sprite->pauseSchedulerAndActions();
   //      spritePaused = !spritePaused;
   //      label->setString("1 pressed");
   //   }
   //   
   //};

   //this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
   return true;
}

void DlgBox::setMsgBox(MsgBoxType type,std::string* pstr){
	DlgBox::msgtype = type;
	DlgBox::m_Msgbody = pstr;
}

