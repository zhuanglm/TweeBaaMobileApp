#include "EventBoard.h"
#include "../HelloWorldScene.h"
#include "../Collage/CollageScene.h"
#include "../utilits/NetworkUtil.h"
#include "AppDelegate.h"
#include "global_const.h"
#include "chooselayer.h"
#include "cocostudio/CocoStudio.h"//for animation


using namespace cocostudio;
EventBoard::EventBoard(){};

EventBoard::~EventBoard(){};

bool EventBoard::init(){

	bool bRet = false;
	do{
		//_ttl_time = 0;
		CC_BREAK_IF(!Scene::init());
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(EVENTBOARD_UI_CSB));

		_UINode = CSLoader::createNode(EVENTBOARD_UI_CSB);
		this->addChild(_UINode);
		auto btnClose= dynamic_cast<Button*>(_UINode->getChildByName(EVENTBOARD_BUTTON_CLOSE));
		CC_BREAK_IF(btnClose == nullptr);
		btnClose->addTouchEventListener(CC_CALLBACK_2(EventBoard::btnQuitCallBack, this));

		iPage = 0;

		auto *pageView = dynamic_cast<PageView*>(_UINode->getChildByName(EVENTBOARD_PAGEVIEW));
		CC_BREAK_IF(pageView == nullptr);
		pageView->addEventListenerPageView(this, pagevieweventselector(EventBoard::pageViewEvent));

		auto *panell = dynamic_cast<Layout*>(pageView->getChildByName(EVENTBOARD_PANEL1));
		auto *panel2 = dynamic_cast<Layout*>(pageView->getChildByName(EVENTBOARD_PANEL2));
		auto *panel3 = dynamic_cast<Layout*>(pageView->getChildByName(EVENTBOARD_PANEL3));
		CC_BREAK_IF(panell == nullptr);
		CC_BREAK_IF(panel2 == nullptr);
		CC_BREAK_IF(panel3 == nullptr);

		auto btnBtn1 = dynamic_cast<Button*>(panell->getChildByName(EVENTBOARD_BTN_TWEEBOT));
		CC_BREAK_IF(btnBtn1 == nullptr);
		btnBtn1->setTag(1001);
		btnBtn1->addTouchEventListener(CC_CALLBACK_2(EventBoard::touchButton, this));


		auto btnBtn2 = dynamic_cast<Button*>(panel2->getChildByName(EVENTBOARD_BTN_SLOT));
		CC_BREAK_IF(btnBtn2 == nullptr);
		btnBtn2->setTag(1002);
		btnBtn2->addTouchEventListener(CC_CALLBACK_2(EventBoard::touchButton, this));

		auto btnBtn3 = dynamic_cast<Button*>(panel3->getChildByName(EVENTBOARD_BTN_10000));
		CC_BREAK_IF(btnBtn3 == nullptr);
		btnBtn3->setTag(1003);
		btnBtn3->addTouchEventListener(CC_CALLBACK_2(EventBoard::touchButton, this));

		Sprite* imgBackground = Sprite::create(EVENTBOARD_BACKGROUND);
		imgBackground->setAnchorPoint(Vec2(0, 0));
		imgBackground->setPosition(Vec2(0, 0));
		this->addChild(imgBackground, -1, 222);

		ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/finger_02/finger_02.ExportJson");
		Armature *armature = Armature::create("finger_02");
		armature->setAnchorPoint(Vec2(0.5, 0.5));
		armature->setPosition(Vec2(800, 150));
		armature->setTag(888);
		armature->setRotation(330);
		armature->setVisible(false);
		_UINode->addChild(armature);


	bRet = true;

	}while(0);
	return bRet;
}

void EventBoard::btnQuitCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		this->cleanup();
		Director::getInstance()->popScene();
	}
	return;
}
void EventBoard::pageViewEvent(Ref *pSender, PageViewEventType type)
{
	auto btnBtn1 = dynamic_cast<Button*>(_UINode->getChildByName(EVENTBOARD_BUTTON1));
	auto btnBtn2 = dynamic_cast<Button*>(_UINode->getChildByName(EVENTBOARD_BUTTON2));
	auto btnBtn3 = dynamic_cast<Button*>(_UINode->getChildByName(EVENTBOARD_BUTTON3));
	switch (type) 
	{
		case cocos2d::ui::PAGEVIEW_EVENT_TURNING:
		{	
			PageView* pageView = dynamic_cast<PageView*> (pSender);
			int iPage=pageView->getCurPageIndex();
			if (iPage >= MAXPAGE) break;
			if (iPage == 0)
			{
				btnBtn1->loadTextures(EVENTBOARD_BTN_ON, EVENTBOARD_BTN_ON, EVENTBOARD_BTN_ON);
				btnBtn2->loadTextures(EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF);
				btnBtn3->loadTextures(EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF);
				int iNews1 = atoi(DataAccess::getParamValue("NewsTweebotRead", "0").c_str());
				if (iNews1 == 0)
				{
					playAni();
				}
				else disableAni();
			}
			else if (iPage==1)
			{
				btnBtn1->loadTextures(EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF);
				btnBtn2->loadTextures(EVENTBOARD_BTN_ON, EVENTBOARD_BTN_ON, EVENTBOARD_BTN_ON);
				btnBtn3->loadTextures(EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF);
				int iNews2 = atoi(DataAccess::getParamValue("NewsSlotRead", "0").c_str());
				if (iNews2 == 0)
				{
					playAni();
				}
				else disableAni();
			}
			else if (iPage == 2)
			{
				btnBtn1->loadTextures(EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF);
				btnBtn2->loadTextures(EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF, EVENTBOARD_BTN_OFF);
				btnBtn3->loadTextures(EVENTBOARD_BTN_ON, EVENTBOARD_BTN_ON, EVENTBOARD_BTN_ON);
				int iNews3 = atoi(DataAccess::getParamValue("News10000Read", "0").c_str());
				if (iNews3 == 0)
				{
					playAni();
				}
				else disableAni();
			}
		}
		break;
	default:
		break;
	}
}
void EventBoard::playAni()
{
	Armature *armature = (Armature*)_UINode->getChildByTag(888);
	armature->setVisible(true);
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
}
void EventBoard::disableAni()
{
	Armature *armature = (Armature*)_UINode->getChildByTag(888);
	armature->setVisible(false);
}
void EventBoard::touchButton(Ref* pSender, ui::Widget::TouchEventType eEventType) 
{
	auto button = dynamic_cast<::ui::Button*>(pSender);
	int tag = button->getTag();
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
				
		if (tag == 1001)
		{
			Application::getInstance()->openURL("https://www.tweebaa.com/");
			DataAccess::setParamValue("NewsTweebotRead", "1");
		}
		if (tag == 1002)
		{
			Application::getInstance()->openURL("https://www.tweebaa.com/Games/SlotMachine/index.aspx/");
			DataAccess::setParamValue("NewsSlotRead", "1");
		}
		if (tag == 1003)
		{
			Application::getInstance()->openURL("http://tweebaa.com/Events/tweebot/");
			DataAccess::setParamValue("News10000Read", "1");
		}
		int iNews1 = atoi(DataAccess::getParamValue("NewsTweebotRead","0").c_str());
		int iNews2 = atoi(DataAccess::getParamValue("NewsSlotRead", "0").c_str());
		int iNews3 = atoi(DataAccess::getParamValue("News10000Read", "0").c_str());

		if (iNews1 + iNews2 + iNews3 == MAXPAGE) DataAccess::setParamValue("NewsRead", "1");

		

	}
	return;
}

