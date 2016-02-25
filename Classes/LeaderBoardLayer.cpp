#include "LeaderBoardLayer.h"
#include "WelcomeScene.h"

#include "MemberCenter/RegisterTabScene.h"
#include "global_const.h"
#include "theme/ThemeLayer.h"
#include "ui/CocosGUI.h"
#include "Collage/CollageScene.h"
#include "LeaderBoard/LeaderBoardItemAdapter.h"
#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "utilits/pugixml.hpp"
#include "utilits/NetworkUtil.h"
#include "Msgbox/Dialogbox.h"
#include "ChooseLayer.h"

using namespace ui; 


Scene* LeaderBoardLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto pLayer = new LeaderBoardLayer();

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

LeaderBoardLayer::LeaderBoardLayer(){


}
LeaderBoardLayer::~LeaderBoardLayer(){
	leader.clear();
}

ssize_t LeaderBoardLayer::numberOfCellsInTableView(TableView *table)
{
	return 11;
}

void LeaderBoardLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
	AppDelegate *app = (AppDelegate*)Application::getInstance();
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:
		if(tag==2003){
			//app->iGameMode=MobileGameMode::Theme_Mode;
			/*auto scene = ChooseLayer::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);*/
			this->cleanup();
			Director::getInstance()->popScene();

		}
		break;
	}
}
bool LeaderBoardLayer::init()
{
	//////////////////////////////  
	// 1. super init first  
	if (!Layer::init())
	{
		return false;
	}

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	std::string sCSD = app->GetLayoutString();
	_rootNode = CSLoader::createNode("res/leaderboard/LeaderBoard" + sCSD + ".csb");

	//loginLayer=Layer::create();
	addChild(_rootNode);

	//initialize();

	auto btnHome = dynamic_cast<Button*>(_rootNode->getChildByName("btnHome"));
	if (btnHome){

		btnHome->setTag(2003);
		btnHome->addTouchEventListener(CC_CALLBACK_2(LeaderBoardLayer::touchButton, this));
	}
	
	HttpRequest* request = new HttpRequest();
	string sUrl = GameAPIURL;
	sUrl = sUrl + "?action=load_top_ranking&top=10";
	char url[100];
	strcpy(url, sUrl.c_str());
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(LeaderBoardLayer::onHttpRequestCompleted, this));
	request->setTag("Load Top Rank");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	

	return true;

}

void LeaderBoardLayer::initialize() {
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
	m_leftTable->setPosition(Vec2(160, 472));
	//m_leftTable->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	//m_leftTable->setDelegate(this);
	m_leftTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	m_leftTable->reloadData();
	m_leftTable->setBounceable(false);
	this->addChild(m_leftTable);
}



void LeaderBoardLayer::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
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
	fileName = filePath + "leader_board.xml";
	*/
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
	{

		//pBuffer[size - 1] = '\0';

		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
		XMLError errorId = pDoc->Parse((const char*)sXml);
		if (errorId != 0)	//xml格式错误
		{
			log("Leader board load error");
			return;
		}

		XMLElement *rootEle = pDoc->RootElement();
		//XMLElement *errorCode = rootEle->FirstChildElement();
		XMLElement *ranking = rootEle->FirstChildElement();
		string st;
		while (ranking) {
			// <game_type_id>3</game_type_id>
			//<username>dragon2934< / username>
			//	<game_score>457< / game_score>

			LeaderBoard* leader1 = new LeaderBoard();
			XMLElement *e_game_type1 = ranking->FirstChildElement();
			if (e_game_type1 && e_game_type1->GetText())
				leader1->game_type_id = atoi(e_game_type1->GetText());
			XMLElement *e_username1 = e_game_type1->NextSiblingElement();
			if (e_username1 && e_username1->GetText())
				leader1->Username = e_username1->GetText();
			string::size_type pos;

			pos = leader1->Username.find("$");
			if (pos != string::npos)//found
			{
				leader1->Username = leader1->Username.substr(0, pos);
			}
			pos = leader1->Username.find("@");
			if (pos != string::npos)//found
			{
				leader1->Username = leader1->Username.substr(0, pos);
			}
			
			XMLElement *e_userscore1 = e_username1->NextSiblingElement();
			if (e_userscore1 && e_userscore1->GetText())
				leader1->UserScore = e_userscore1->GetText();
			leader.pushBack(leader1);

			ranking = ranking->NextSiblingElement();
			leader1->release();

		}
	}

	initialize();

}

Size LeaderBoardLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(650, 32);
}

TableViewCell* LeaderBoardLayer::tableCellAtIndex(TableView *table, ssize_t idx)
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

		if (idx == 0){
			//"RANK “                      “ PLAYER”                              “POINTS”
			auto menu = LeaderBoardItemAdapter::create();//适配类
			//menu->itemIcon->loadTexture("match_game_img_1.jpg");//添加Icon
			menu->itemNum->setString("RANK");//添加Text
			menu->itemUserName->setString("PLAYER");
			menu->itemScore->setString("POINTS");

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

			LeaderBoard* leader_list = (LeaderBoard*)leader.at(idx - 1);
			if (leader_list){
				//Size winSize = Director::getInstance()->getWinSize();
				auto menu = LeaderBoardItemAdapter::create();//适配类
				//menu->itemIcon->loadTexture("match_game_img_1.jpg");//添加Icon
				menu->itemNum->setString(p);//添加Text
				menu->itemUserName->setString(leader_list->Username);
				menu->itemScore->setString(leader_list->UserScore);

				//menu->setContentSize(Size(winSize.width, winSize.height / 8));//设置视图大小
				log("menu.width = %lf,menu.height = %lf", menu->getContentSize().width, menu->getContentSize().height);

				menu->setSize(Size(650, 1));//设置大小
				menu->setLayoutType(cocos2d::ui::Layout::Type::RELATIVE);//设置布局类型
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