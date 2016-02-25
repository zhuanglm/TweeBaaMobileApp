#include "SpinLuckyLayer.h"
#include "WelcomeScene.h"

//#include "MemberCenter/RegisterTabScene.h"
#include "global_const.h"
//#include "theme/ThemeLayer.h"
#include "ui/CocosGUI.h"
#include "../Collage/CollageScene.h"
#include "../ZhuanPanSystem/SpinLuckyItemAdapter.h"
#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../utilits/pugixml.hpp"
#include "../utilits/NetworkUtil.h"
#include "../Msgbox/Dialogbox.h"
#include "ChooseLayer.h"
#include "../ZhuanPanSystem/TurntableSystem.h"

using namespace ui; 


Scene* SpinLuckyLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto pLayer = new SpinLuckyLayer();

	if (scene && pLayer && pLayer->init())
	{
		pLayer->autorelease();
		scene->addChild(pLayer);
		return scene;
	}
	else
	{
		delete pLayer;
		pLayer = NULL;
		return NULL;
	}
}

SpinLuckyLayer::SpinLuckyLayer(){


}
SpinLuckyLayer::~SpinLuckyLayer(){
	leader.clear();
}

ssize_t SpinLuckyLayer::numberOfCellsInTableView(TableView *table)
{
	return leader.size()+1;
}

void SpinLuckyLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
	AppDelegate *app = (AppDelegate*)Application::getInstance();
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		if(tag==2003)
		{
			//auto scene = TurntableSystem::create();
			//TransitionScene *transition = TransitionFade::create(1, scene);
			//Director::getInstance()->replaceScene(transition);
			this->cleanup();
			Director::getInstance()->popScene();

		}
		break;
	}
}
bool SpinLuckyLayer::init()
{
	//////////////////////////////  
	// 1. super init first  
	//if (!Layer::init())
	//{
	//	return false;
	//}

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sCSD = app->GetLayoutString();
	_rootNode = CSLoader::createNode("res/daily_spin/SpinLuckyLayer" + sCSD + ".csb");

	//loginLayer=Layer::create();
	addChild(_rootNode);

	//initialize();

	auto btnHome = dynamic_cast<Button*>(_rootNode->getChildByName("btnHome"));
	if (btnHome){

		btnHome->setTag(2003);
		btnHome->addTouchEventListener(CC_CALLBACK_2(SpinLuckyLayer::touchButton, this));
	}
	
	HttpRequest* request = new HttpRequest();
	string sUrl = GameAPIURL;
	sUrl = sUrl + "?action=Load_Spin_Lucky";
	char url[100];
	strcpy(url, sUrl.c_str());
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(SpinLuckyLayer::onHttpRequestCompleted, this));
	request->setTag("Load Top Rank");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	

	return true;

}

void SpinLuckyLayer::initialize() {
	auto bgSize = Director::getInstance()->getWinSize();
	Size m_viewSize = Size(bgSize.width, bgSize.height * 0.95);
	//添加内容
	/*
	auto pContainer = Layer::create();
	pContainer->setContentSize(Size(m_viewSize.width * 3, m_viewSize.height));
	pContainer->setAnchorPoint(Point::ZERO);
	pContainer->setPosition(Vec2::ZERO);
	this->addChild(pContainer);
	*/
	//m_scrollView->setContainer(pContainer);
	//添加tabelview
	auto containerSize = bgSize;// pContainer->getContentSize();
	m_leftTable = new TableView();
	//m_leftTable->setColor(ccc3(255, 0, 0));
	m_leftTable->initWithViewSize(m_viewSize, NULL);
	m_leftTable->autorelease();
	m_leftTable->setDataSource(this);
	m_leftTable->setTag(1001);
	m_leftTable->ignoreAnchorPointForPosition(false);
	m_leftTable->setAnchorPoint(Vec2(0, 1));
	m_leftTable->setPosition(Vec2(160, 485));
	m_leftTable->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	//m_leftTable->setDelegate(this);
	m_leftTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	m_leftTable->reloadData();
	m_leftTable->setBounceable(false);

	//auto lblInfo2 = dynamic_cast<Label*>(this->getChildByName("lblInfo2"));
	//lblInfo2->setTextColor(ccc3(73, 34, 0));

	this->addChild(m_leftTable);
}



void SpinLuckyLayer::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
		//StartGame();
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
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
		//StartGame();
        return;
	}
	if (statusCode != 200){
		log("response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		//StartGame();
		return;
	}
    // dump data
    std::vector<char> *buffer = response->getResponseData();

	char sXml[10000];

	//printf("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		sXml[i] = (*buffer)[i];
	}
	sXml[buffer->size()] = '\0';
	/*
	string filePath, fileName;
	filePath = FileUtils::getInstance()->getWritablePath();
	fileName = filePath + "leader_board.xml";*/
	/*
	ifstream ifc(fileName);
	if (!ifc){


	}
	*/
	/*

	std::string filePath1 = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
	ssize_t size;
	auto pBuffer = CCFileUtils::sharedFileUtils()->getFileData(filePath1.c_str(), "rb", &size);
	if (pBuffer) */

		//pBuffer[size - 1] = '\0';
	try
	{
		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
		XMLError errorId = pDoc->Parse((const char*)sXml);
		if (errorId != 0)	//xml格式错误
		{
			log("Leader board load error");
			return;
		}

		XMLElement *rootEle = pDoc->RootElement();
		XMLElement *errorCode = rootEle->FirstChildElement();
		if (errorCode != nullptr)
		{
			XMLElement *ranking = errorCode->NextSiblingElement();
			string st;
			while (ranking) 
			{
				/*
				<?xml version="1.0" encoding="UTF-8"?>
				<mobileAppSpinLucky>
				<result>1</result>
				<SpinLucky>
				<username>dragon2934</username>
				<Prize>1 Powerup</Prize>
				<SpinDate>9/24/2015 12:00:00 AM</SpinDate>
				</SpinLucky>
				</mobileAppSpinLucky>
				*/

				SpinLucky* leader1 = new SpinLucky();
				XMLElement *e_username1 = ranking->FirstChildElement();
				XMLElement *e_RewardItem = e_username1->NextSiblingElement();
				XMLElement *e_DateTime = e_RewardItem->NextSiblingElement();

				if (e_username1 && e_username1->GetText())
					leader1->Username = e_username1->GetText();

				string::size_type pos;

				pos = leader1->Username.find("$");
				if (pos != string::npos)//found
				{
					leader1->Username = leader1->Username.substr(0, pos);
				}

				if (e_RewardItem && e_RewardItem->GetText())
					leader1->RewardItemName = e_RewardItem->GetText();


				if (e_DateTime && e_DateTime->GetText()){
					leader1->sLuckyDate = e_DateTime->GetText();
					leader1->sLuckyDate=leader1->sLuckyDate.substr(0, 10);
				}
				leader.pushBack(leader1);

				ranking = ranking->NextSiblingElement();
				leader1->release();
				/*
				SpinLucky* leader2 = new SpinLucky();
				XMLElement *e_game_type2 = ranking->FirstChildElement();
				if (e_game_type2 && e_game_type2->GetText())
					leader2->game_type_id = atoi(e_game_type2->GetText());
				XMLElement *e_username2 = e_game_type2->NextSiblingElement();
				if (e_username2 && e_username2->GetText())
					leader2->Username = e_username2->GetText();
				XMLElement *e_userscore2 = e_username2->NextSiblingElement();
				if (e_userscore2 && e_userscore2->GetText())
					leader2->UserScore = e_userscore2->GetText();
				leader.pushBack(leader2);
				*/
			}

		}
	}
	catch (...)
	{
	}

	initialize();
	/*
	string sXml(buffer->begin(), buffer->end());
	pugi::xml_document doc;
	pugi::char_t cXml[1024];
	strcpy(cXml, sXml.c_str());
	pugi::xml_parse_result result = doc.load_string(cXml);

	pugi::xml_node nodes = doc.child("mobileAppTopRanking");

	std::string strError = nodes.child_value("error");
	char p[100];
	strcpy(p, strError.c_str());
	if (atoi(p) == 0){
		//get next 

	}
	else{


	}
	*/

}

Size SpinLuckyLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(650, 32);
}

TableViewCell* SpinLuckyLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	auto cellSize = this->tableCellSizeForIndex(table, idx);
	auto tag = table->getTag();

	if (!cell)
	{
		cell = new TableViewCell();
		cell->autorelease();


		Sprite * pCellBg = NULL;
		Label * pTitle = NULL;
		Label * pDesigner = NULL;
		Sprite * pIcon = NULL;
		switch (tag)
		{
		case 0:
		{
			/*
			pCellBg = Sprite::create("match_game_img_1.jpg");
			pNum = Label::createWithTTF("Designer:Snow","fonts/Marker Felt.ttf",20);
			pIcon = Sprite::create("ic_new_btn_like.png");
			*/
		}
		break;
		/*
		case Table_Center:
		{
		pCellBg = Sprite::create("CollageShareView/cell2.png");
		pNum = Label::createWithTTF("2","fonts/Marker Felt.ttf",20);
		pIcon = Sprite::create("CollageShareView/plane.png");
		}
		break;
		case Table_Right:
		{
		pCellBg = Sprite::create("CollageShareView/cell3.png");
		pNum = Label::createWithTTF("3","fonts/Marker Felt.ttf",20);
		pIcon = Sprite::create("CollageShareView/setting.png");
		}
		*/
		default:
			break;
		}
		/*
		pCellBg->setPosition(Vec2(cellSize.width / 2,cellSize.height / 2));
		cell->addChild(pCellBg);
		pNum->setColor(Color3B(255,0,0));
		pNum->setPosition(Vec2(cellSize.width * 0.1,cellSize.height / 2));
		cell->addChild(pNum);
		pIcon->setPosition(Vec2(cellSize.width * 0.85,cellSize.height / 2));
		//pIcon->setScale(0.2);
		cell->addChild(pIcon);
		*/

		if (idx == 0){
			//"RANK “                      “ PLAYER”                              “POINTS”
			auto menu = SpinLuckyItemAdapter::create();//适配类
			//menu->itemIcon->loadTexture("match_game_img_1.jpg");//添加Icon
			menu->itemUserName->setString("PLAYER");//添加Text
			menu->itemPrize->setString("REWARD");
			menu->itemDateTime->setString("DATE");
			menu->itemUserName->setColor(ccc3(72, 37, 0));
			menu->itemPrize->setColor(ccc3(72, 37, 0));
			menu->itemDateTime->setColor(ccc3(72, 37, 0));

			//menu->setContentSize(Size(winSize.width, winSize.height / 8));//设置视图大小
			log("menu.width = %lf,menu.height = %lf", menu->getContentSize().width, menu->getContentSize().height);

			menu->setSize(Size(650, 0));//设置大小
			menu->setLayoutType(cocos2d::ui::Layout::Type::RELATIVE);//设置布局类型
			menu->ignoreAnchorPointForPosition(false);
			menu->setAnchorPoint(Vec2::ZERO);//设置锚点
			//menu->touchBtnLike->addTouchEventListener(CC_CALLBACK_2(CollageShareView::touchEvent, this));//设置Item触摸事件
			//;menu->touchBtnShare->addTouchEventListener(CC_CALLBACK_2(CollageShareView::touchEvent, this));//设置Item触摸事件
			cell->addChild(menu);
		}
		else{
			int iIndex = idx ;
			char p[10];
			sprintf(p, "%d", iIndex);

			SpinLucky* leader_list = (SpinLucky*)leader.at(idx - 1);
			if (leader_list){
				//Size winSize = Director::getInstance()->getWinSize();
				auto menu = SpinLuckyItemAdapter::create();//适配类
				//menu->itemIcon->loadTexture("match_game_img_1.jpg");//添加Icon
				menu->itemUserName->setString(leader_list->Username);//添加Text
				menu->itemPrize->setString(leader_list->RewardItemName);
				menu->itemDateTime->setString(leader_list->sLuckyDate);
				menu->itemUserName->setColor(ccc3(72, 37, 0));
				menu->itemPrize->setColor(ccc3(72, 37, 0));
				menu->itemDateTime->setColor(ccc3(72, 37, 0));
				//menu->setContentSize(Size(winSize.width, winSize.height / 8));//设置视图大小
				log("menu.width = %lf,menu.height = %lf", menu->getContentSize().width, menu->getContentSize().height);

				menu->setSize(Size(650, 1));//设置大小
				menu->setLayoutType(cocos2d::ui::Layout::Type::HORIZONTAL);//设置布局类型
				menu->ignoreAnchorPointForPosition(false);
				menu->setAnchorPoint(Vec2::ZERO);//设置锚点

				//menu->touchBtnLike->addTouchEventListener(CC_CALLBACK_2(CollageShareView::touchEvent, this));//设置Item触摸事件
				//;menu->touchBtnShare->addTouchEventListener(CC_CALLBACK_2(CollageShareView::touchEvent, this));//设置Item触摸事件
				cell->addChild(menu);
			}
		}
	}
	return cell;
}