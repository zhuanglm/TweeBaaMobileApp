#include "DailyCheckInLayer.h"
#include "WelcomeScene.h"
//#include "Login.h"
//#include "CollageGUI/CollageMainTabScene.h"
#include "../MemberCenter/RegisterTabScene.h"
#include "global_const.h"
#include "../theme/ThemeLayer.h"
#include "ui/CocosGUI.h"
#include "../Collage/CollageScene.h"

#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

#include "../utilits/NetworkUtil.h"
#include "../Msgbox/MsgBox.h"
#include "../Msgbox/Dialogbox.h"
#include "CCAlert.h"
#include "../utilits/DailyCheckIn.h";
#include "../utilits/pugixml.hpp"


using namespace ui; 

extern Vector<DailyCheckIn *> vecUserCheckInDays;

const char sDay[7][4] = { "sun", "mon", "tue", "wed", "thu", "fri", "sat" };
/*
Scene* DailyCheckInLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = DailyCheckInLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}*/


DailyCheckInLayer::~DailyCheckInLayer(){

}
/*
bool DailyCheckInLayer::init()
{
	//////////////////////////////  
	// 1. super init first  
	//if (!Layer::init())
	//{
	//	return false;
	//}

	initialize();
	return true;

}
*/
int DailyCheckInLayer::dayofweek(int d, int m, int y){
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}
void DailyCheckInLayer::menuCallbackYes(Ref *sender) {

	//post http post
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	HttpRequest* request = new HttpRequest();
	string sUrl = GameAPIURL;
	sUrl = sUrl + "?action=daily_check_in&UserID=" + app->GetUserGuid();
	char url[300];
	strcpy(url, sUrl.c_str());
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(DailyCheckInLayer::onCheckinCompleted, this));
	request->setTag("Load Top Rank");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	
	_rootNode->removeFromParentAndCleanup(true);

}
void DailyCheckInLayer:: menuCallbackNo(Ref *sender){

	_rootNode->removeFromParentAndCleanup(true);

}


//void DailyCheckInLayer::initialize() {

DailyCheckInLayer::DailyCheckInLayer(Layer* layer){
	_rootNode = layer;
	Size visiableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/*
	::ui::Layout* layout = ::ui::Layout::create();
	layout->setSize(Size(visiableSize.width /2 , visiableSize.height/2));
	layout->setBackGroundColorType(::ui::LAYOUT_COLOR_SOLID);
	layout->setBackGroundColor(ccc3(4, 123, 182));
	layout->setPosition(Vec2(0, 0));
	addChild(layout, -5);
	*/

	Sprite* background = Sprite::create("dailycheckin/spin_bg.png");

	//float scaleRate = visiableSize.width / background->getContentSize().width;
	//background->setScale(scaleRate);
	//background->setAnchorPoint(Vec2(0.0f, 1.0f));
	//float height = visiableSize.height - (visiableSize.height - background->getContentSize().height * scaleRate) / 2;
	background->setPosition(Vec2(visiableSize.width / 2, visiableSize.height / 2));


	//loop it
	//now
	/*
	time_t rawtime;
	tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	int wday = timeinfo->tm_wday;
	*/
	//create 7 sprite

	/*
	Label* lblYes = Label::create();
	lblYes->setString("Yes");
	lblYes->setPosition(Vec2(30, 40));
	lblYes->setSystemFontSize(30);
	Label* lblNo = Label::create();
	lblNo->setSystemFontSize(40);
	lblNo->setPosition(Vec2(40, 30));
	lblNo->setString("No");
	*/
	Sprite* Info1 = Sprite::create("dailycheckin/Info_1.png");
	Info1->setPosition(Vec2(visiableSize.width / 2 - 170, 280));
	background->addChild(Info1);

	Sprite* Info2 = Sprite::create("dailycheckin/Info_2.png");
	Info2->setPosition(Vec2(visiableSize.width / 2 - 170, 20));
	background->addChild(Info2);

	Sprite* btnYes = Sprite::create("dailycheckin/btnYes.png");
	//btnYes->addChild(lblYes,1,1001);
	//btnYes->setPosition(Vec2(visiableSize.width / 2 - 50, visiableSize.height / 2 - 80));
	//addChild(btnYes, 2);

	Sprite *btnYes_a = Sprite::create("dailycheckin/btnYes.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	btnYes_a->setPositionY(5);
	auto menuItem_main_menu_yes = MenuItemSprite::create(btnYes, btnYes_a, NULL, CC_CALLBACK_1(DailyCheckInLayer::menuCallbackYes, this));
	menuItem_main_menu_yes->setPosition(Vec2(visiableSize.width / 2 - 80, visiableSize.height / 2 - 90));

	auto menu_main_menu_yes = Menu::create(menuItem_main_menu_yes, NULL);
	menu_main_menu_yes->setPosition(Point(origin.x, origin.y));


	layer->addChild(background);
	layer->addChild(menu_main_menu_yes);


	Sprite* btnNo = Sprite::create("dailycheckin/btnNo.png");
	//btnNo->addChild(lblNo,1,1002);
	//btnNo->setPosition(Vec2(visiableSize.width / 2 + 50, visiableSize.height / 2 - 80));
	//addChild(btnNo, 2);

	Sprite *btnNo_a = Sprite::create("dailycheckin/btnNo.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	btnNo_a->setPositionY(5);
	auto menuItem_main_menu = MenuItemSprite::create(btnNo, btnNo_a, NULL, CC_CALLBACK_1(DailyCheckInLayer::menuCallbackNo, this));
	menuItem_main_menu->setPosition(Vec2(visiableSize.width / 2 + 70, visiableSize.height / 2 - 90));
	auto menu_main_menu_no = Menu::create(menuItem_main_menu, NULL);
	menu_main_menu_no->setPosition(Point(origin.x, origin.y));

	layer->addChild(menu_main_menu_no);
	/*
	auto touchListener1 = EventListenerTouchOneByOne::create();
	touchListener1->onTouchBegan = [](Touch* touch, Event* event) -> bool
	{
	auto bounds = event->getCurrentTarget()->getBoundingBox();

	if (bounds.containsPoint(touch->getLocation()))
	{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	//this->change_pic(1);
	int i = target->getTag() - 1000;

	if (i == 1){
	log("dddddddddddd");
	}
	}
	return true;
	};
	*/
	int iSpace = 80;
	int iFirst = 230;
	int iFlag = 0;

	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < vecUserCheckInDays.size(); i++)
		{
			DailyCheckIn* d = vecUserCheckInDays.at(i);
			if (d->sWeekDay.compare(sDay[j]) == 0 && d->isSameWeek == 1)
			{
				std::string strDay = "dailycheckin/" + string(sDay[j]) + ".png";
				Sprite* day = Sprite::create(strDay);
				if (day)
				{
					day->setPosition(Vec2(iFirst + iSpace * j, visiableSize.height / 2));
					layer->addChild(day, 1);
					iFlag = 1;
				}
			}
		}
		if (!iFlag)
		{
			std::string strDay = "dailycheckin/" + string(sDay[j]) + "_dis.png";
			Sprite* day = Sprite::create(strDay);
			if (day)
			{
				day->setPosition(Vec2(iFirst + iSpace * j, visiableSize.height / 2));
				layer->addChild(day, 1);
			}
		}
		iFlag = 0;
	}
}

void DailyCheckInLayer::onCheckinCompleted(HttpClient *sender, HttpResponse *response)
{
	std::string title = "\0";
	std::string msg = "\0";
    if (!response)
	{
		title = "Network Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsgBox(title, msg);
        return;
    }   

    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        log("%s completed", response->getHttpRequest()->getTag());
    }   
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
   // _labelStatusCode->setString(statusString);
    log("response code: %d", statusCode);   
    if (!response->isSucceed())
	{
		title = "Response Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsgBox(title, msg);
        return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();
	std::string sXml(buffer->begin(), buffer->end());
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml, sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppCheckIn");

	std::string strError = nodes.child_value("result");

	char p[100];
	strcpy(p, strError.c_str());
	int iResult = atoi(p);
	if (iResult == 2){
		//get 1 points
		title = "Congratulations\0";
		msg = "You have signed in successfully, and get 1 bonus point.\0";
	}
	else if (iResult == 1){
		//10 points
		title = "Congratulations\0";
		msg = "You have continuous attendance for a week, get 10 bonus points.\0";
	}
	else if (iResult==0)
	{
		//already sign in
		title = "Sorry\0";
		msg = "You already checked-in today. Please come back tomorrow!\0";
	}
	else
	{
		title = "Sorry\0";
		msg = "Sign in Error!\0";
	}

	ShowMsgBox(title, msg);

}


void DailyCheckInLayer::ShowMsgBox(std::string title, std::string msg)
{
	MsgBoxType m_type = TP_INFO;
	MsgBox::setMsgBox(m_type, &title, &msg);
	auto msgbox = MsgBox::create();
	msgbox->setTag(MSGBOX_TAG);
	this->addChild(msgbox, 999);

	//Add Event to process
	auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
	btnOk->addTouchEventListener(CC_CALLBACK_1(DailyCheckInLayer::menuCallbackNo, this));

}

