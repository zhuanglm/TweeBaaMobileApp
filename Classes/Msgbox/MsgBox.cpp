#include "MsgBox.h"
#include "cocos2d.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

int MsgBox::msgtype = 1;
std::string *MsgBox::m_Msgbody = nullptr;
std::string *MsgBox::m_Title = nullptr;
std::string *MsgBox::m_Info = nullptr;
std::string sBlank = "";

bool MsgBox::onInitDialog()
{
    bool bRet = false;
	do{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	_MSG_UINode = nullptr;
	_MSG_UINode = CSLoader::createNode(MSGBOX_UI_CSB);
	CC_BREAK_IF(_MSG_UINode == nullptr);
	_MSG_UINode->setTag(MSGBOX_UINODE);
	this->addChild(_MSG_UINode);

	auto title = dynamic_cast<Text*>(_MSG_UINode->getChildByName("txtTitle"));
	auto msg = dynamic_cast<Text*>(_MSG_UINode->getChildByName("txtMsg"));
	auto info = dynamic_cast<Text*>(_MSG_UINode->getChildByName("txtInfo"));
	CC_BREAK_IF(title == nullptr);
	CC_BREAK_IF(msg == nullptr);
	CC_BREAK_IF(info == nullptr);
	//title->setFontName("res/FONT/HUM521B.TTF");
	//title->setColor(ccc3(75, 25, 21));
	title->setFontSize(30);
	title->setString(*MsgBox::m_Title);

	//msg->setFontName("res/FONT/HUM521B.TTF");
	//msg->setColor(ccc3(75, 25, 21));
	//msg->setFontSize(28);
	msg->setString(*MsgBox::m_Msgbody);

	//info->setFontName("res/FONT/Humanst521 BT.TTF");
	//info->setColor(ccc3(75, 25, 21));
	info->setFontSize(26);
	info->setString(*MsgBox::m_Info);

	auto btnYES = dynamic_cast<Button*>(_MSG_UINode->getChildByName("btn_Yes"));
	auto btnNO = dynamic_cast<Button*>(_MSG_UINode->getChildByName("btn_No"));
	auto btnOK = dynamic_cast<Button*>(_MSG_UINode->getChildByName("btn_OK"));
	auto btnCANCEL = dynamic_cast<Button*>(_MSG_UINode->getChildByName("btn_Cancel"));
	CC_BREAK_IF(btnYES == nullptr);
	CC_BREAK_IF(btnNO == nullptr);
	CC_BREAK_IF(btnOK == nullptr);
	CC_BREAK_IF(btnCANCEL == nullptr);

	btnYES->setTitleFontName("res/FONT/HUM521B.TTF");
	btnNO->setTitleFontName("res/FONT/HUM521B.TTF");
	btnOK->setTitleFontName("res/FONT/HUM521B.TTF");
	btnCANCEL->setTitleFontName("res/FONT/HUM521B.TTF");

	//_MSG_UINode->getChildByName("btn_OK")->setVisible(false);
	//_MSG_UINode->getChildByName("btn_Cancel")->setVisible(false);

	//auto btnYes = dynamic_cast<Button*>(_MSG_UINode->getChildByName("btn_Yes"));// Magnifier Button
	//auto btnNo = dynamic_cast<Button*>(_MSG_UINode->getChildByName("btn_No"));// Magnifier Button
	//btnYes->addTouchEventListener(CC_CALLBACK_2(MsgBox::btnClick, this));
	//btnNo->addTouchEventListener(CC_CALLBACK_2(MsgBox::btnClick, this));

	//auto chatTextSize = CCSizeMake(300,200);
	//CCLabelTTF *label = CCLabelTTF::create("", "Message", 20, chatTextSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	////auto *label = Label::create("", "", 24);
	////label->setString(msg);
	//label->setString(*MsgBox::m_Msgbody);
	//label->setColor(ccc3(0,0,0));
 //   label->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 20));
 //   this->addChild(label,99);

	//if (msgtype == 1) {
	//	cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("/public/yes.png","/public/yes.png",this,menu_selector(MsgBox::yesMenuItemCallback));
	//	okMenuItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 90));
	//	pushMenu(okMenuItem);
	//	//CCMenuItemFont *okMenuItem = CCMenuItemFont::create("OK", this, menu_selector(MsgBox::okMenuItemCallback));
	//	//okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 50));

	//	//CCMenuItemFont *cancelMenuItem = CCMenuItemFont::create("Cancel", this, menu_selector(MsgBox::cancelMenuItemCallback));
	//	//cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 50));

	//} else if (msgtype == 2){
	//	cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("/public/yes.png","/public/yes.png",this,menu_selector(MsgBox::yesMenuItemCallback));
	//	okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 90));

	//	cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("/public/no.png","/public/no.png",this,menu_selector(MsgBox::noMenuItemCallback));
	//	cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 90));
	//	pushMenu(okMenuItem);
	//	pushMenu(cancelMenuItem);

	//}else {
	//	cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("/public/no.png","/public/no.png",this,menu_selector(MsgBox::noMenuItemCallback));
	//	cancelMenuItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 90));
	//	pushMenu(cancelMenuItem);
	//}

	if (msgtype == TP_Q_YESNO) {
		_MSG_UINode->getChildByName("btn_OK")->setVisible(false);
		_MSG_UINode->getChildByName("btn_Cancel")->setVisible(false);
		//cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("res/MsgBox/message_btn.png","res/MsgBox/message_btn.png",this,menu_selector(MsgBox::yesMenuItemCallback));
		//okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 90));

		//cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("res/MsgBox/message_btn.png","res/MsgBox/message_btn.png",this,menu_selector(MsgBox::noMenuItemCallback));
		//cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 90));

		//pushMenu(okMenuItem);
		//pushMenu(cancelMenuItem);

	} else if (msgtype == TP_Q_OKCANCEL){
		_MSG_UINode->getChildByName("btn_Yes")->setVisible(false);
		_MSG_UINode->getChildByName("btn_No")->setVisible(false);
		_MSG_UINode->getChildByName("btn_OK")->setPosition(_MSG_UINode->getChildByName("btn_Yes")->getPosition());
		_MSG_UINode->getChildByName("btn_Cancel")->setPosition(_MSG_UINode->getChildByName("btn_No")->getPosition());

		//cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("res/public/yes.png","res/public/yes.png",this,menu_selector(MsgBox::okMenuItemCallback));
		//okMenuItem->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2 - 90));

		//cocos2d::MenuItem* cancelMenuItem = cocos2d::MenuItemImage::create("res/public/no.png","res/public/no.png",this,menu_selector(MsgBox::cancelMenuItemCallback));
		//cancelMenuItem->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2 - 90));
		//pushMenu(okMenuItem);
		//pushMenu(cancelMenuItem);

	}else if (msgtype = TP_INFO){
		_MSG_UINode->getChildByName("btn_Yes")->setVisible(false);
		_MSG_UINode->getChildByName("btn_No")->setVisible(false);
		_MSG_UINode->getChildByName("btn_Cancel")->setVisible(false);

		//cocos2d::MenuItem* okMenuItem = cocos2d::MenuItemImage::create("res/public/yes.png","res/public/yes.png",this,menu_selector(MsgBox::okMenuItemCallback));
		//okMenuItem->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 90));
		//pushMenu(okMenuItem);
	}


    //setColor(ccc3(255,178,102));
    setOpacity(128);
	bRet = true;

	}while(0);

    return bRet;
}
void MsgBox::btnClick(Ref* pSender, ui::Widget::TouchEventType eEventType){

	//if (eEventType != ui::Widget::TouchEventType::BEGAN)
	Sprite* sp = (Sprite*) pSender;
	int tag = sp->getTag();
	setSwallowsTouches(true);

	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");

			removeFromParentAndCleanup(true);
			break;
		case ui::Widget::TouchEventType::MOVED:
			cocos2d::log("touch Cancel");
			break;
		case ui::Widget::TouchEventType::CANCELED:
			cocos2d::log("touch Cancel");
			break;
		default:
			cocos2d::log("touch Default");
			break;
	}
	return;

}

void MsgBox::okMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_OK);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
	
}

void MsgBox::cancelMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_CANCEL);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
}

void MsgBox::yesMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_YES);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
}

void MsgBox::noMenuItemCallback(CCObject *obj)
{
	auto event = EventCustom(APP_CE_NO);
	auto voice = Value("Can you hear me?");
	event.setUserData(&voice);
	getEventDispatcher()->dispatchEvent(&event);

	removeFromParentAndCleanup(true);
}

void MsgBox::update(float delta) {
	auto position = this->getChildByTag(199)->getPosition();
	auto sp = (Sprite*) this->getChildByTag(199);
	//position.x -= 250 * delta;
	//if (position.x  < 0 - (this->getChildByTag(199)->getBoundingBox().size.width / 2))
	//	position.x = this->getBoundingBox().getMaxX() + sp->getBoundingBox().size.width/2;
	//sp->setPosition(position);

}

bool MsgBox::test(){

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

void MsgBox::setMsgBox(MsgBoxType type,std::string* pTitle,std::string* pMsg){
	MsgBox::msgtype = type;
	MsgBox::m_Msgbody = pMsg;
	MsgBox::m_Title = pTitle;
	MsgBox::m_Info = &sBlank;
}


void MsgBox::setMsgBox(MsgBoxType type, std::string* pTitle, std::string* pMsg, std::string* pInfo)
{
	MsgBox::msgtype = type;
	MsgBox::m_Msgbody = pMsg;
	MsgBox::m_Title = pTitle;
	MsgBox::m_Info = pInfo;
}