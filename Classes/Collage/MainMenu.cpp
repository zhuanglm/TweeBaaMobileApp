//===========Tweebaa==============
//MainMenu.cpp	--Jly 20 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ChooseLayer.h"
#include "CheckoutScene.h"
#include "AppDelegate.h"

menuItem MainMenu::currentMenuItem=menuItem::Home;

USING_NS_CC;
using namespace ui;

//FeedLayer * MainMenu::fLayer;


Scene* MainMenu::createScene()
{
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	// 'scene' is an autorelease object
	auto scene = Scene::create();

    // 'layer' is an autorelease object

	auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool MainMenu::init(){
	if(!Layer::init()){
		return false;
	}
	
	auto rootNode = CSLoader::createNode("Collage/CollageScene.csb");
	addChild(rootNode);

	initialize(rootNode);
	return true;
}

MainMenu::MainMenu(void)
{
	cLayer = nullptr;
	fLayer = nullptr;
	bLayer = nullptr;
	//initialize(rootNode);
}

MainMenu::~MainMenu(void)
{
	if(fLayer)
	{
		delete fLayer;
		fLayer=nullptr;
	}
	if(cLayer)
	{
		delete cLayer;
		cLayer=nullptr;
	}
	if(bLayer)
	{
		delete bLayer;
		bLayer=nullptr;
	}
}



void MainMenu::ResetLMButton(Node* object)
{
	
	if(currentMenuItem==menuItem::Trends && HttpConnection::v_design_time_info.size()>0)
		SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Trends"),true);
	else if(currentMenuItem==menuItem::Create)SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Create"),true);
	else if(currentMenuItem==menuItem::Feed && HttpConnection::v_design_like_info.size()>0)
		SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Feed"),true);
	else if(currentMenuItem==menuItem::Shop)SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Shop"),true);
}

void MainMenu::touchCreate(Node* object,TouchEventType type)
{
	if(DesignLayer::bLongPressed) return;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(currentMenuItem!=menuItem::Home)
			{
				switch(currentMenuItem)
				{
					case menuItem::Create:
						cLayer->release();
						cLayer=nullptr;
						break;
						
					case menuItem::Feed:
						fLayer->release();
						fLayer=nullptr;
						break;
					case menuItem::Trends:
						bLayer->Layer::release();
						bLayer=nullptr;
						break;
					default:
						break;
				}
				
				object->getParent()->removeChildByTag(currentMenuItem,true);
				ResetLMButton(object);
			}

			SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Create"),false);
			/*_createLayer = CSLoader::createNode("Collage/RightMenu.csb");
			object->getParent()->addChild(_createLayer);
			_createLayer->setTag(MenuItem_Create);*/
			currentMenuItem=menuItem::Create;
			//cLayer = new CreateLayer(_createLayer);
			if(cLayer)					
				cLayer->release();
			cLayer = new CreateLayer(object);

			//cLayer->autorelease();
			break;  
		  
		default:  
			break;  
    }
	
}

void MainMenu::touchTrends(Node* object,TouchEventType type)
{

	if(DesignLayer::bLongPressed) return;
	Node* _browseLayer;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(currentMenuItem!=menuItem::Home)
			{
				switch(currentMenuItem)
				{
					case menuItem::Create:
						cLayer->release();
						cLayer=nullptr;
						break;
					case menuItem::Feed:
						fLayer->release();
						fLayer=nullptr;
						break;
					case menuItem::Trends:
						bLayer->Layer::release();
						bLayer=nullptr;
						break;
					default:
						break;
				}

				object->getParent()->removeChildByTag(currentMenuItem,true);
				if(currentMenuItem==menuItem::Create) object->getParent()->removeChildByTag(Design_Tag,true);
				ResetLMButton(object);
			}
			SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Trends"),false);

			_browseLayer = CSLoader::createNode("Collage/BrowseLayer.csb");
			object->getParent()->addChild(_browseLayer,-10);
			_browseLayer->setTag(MenuItem_Trends);
			currentMenuItem=menuItem::Trends;

			if(bLayer)					
				bLayer->Layer::release();
			bLayer = new BrowseLayer(_browseLayer);
			//bLayer->autorelease();
			break;  
		  
		default:  
			break;  
    }
	
}

void MainMenu::touchFeed(Node* object,TouchEventType type)
{

	if(DesignLayer::bLongPressed) return;
	Node* _feedLayer;
	
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(currentMenuItem!=menuItem::Home)
			{
				switch(currentMenuItem)
				{
					case menuItem::Create:
						if(cLayer)
							cLayer->release();
						cLayer=nullptr;
						break;
					case menuItem::Feed:
						fLayer->release();
						fLayer=nullptr;
						break;
					case menuItem::Trends:
						bLayer->Layer::release();
						bLayer=nullptr;
						break;
					default:
						break;
				}
				
				object->getParent()->removeChildByTag(currentMenuItem,true);
				if(currentMenuItem==menuItem::Create) object->getParent()->removeChildByTag(Design_Tag,true);
				ResetLMButton(object);
			}
			SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Feed"),false);

			_feedLayer = CSLoader::createNode("Collage/BrowseLayer.csb");
			object->getParent()->addChild(_feedLayer,-10);
			_feedLayer->setTag(MenuItem_Feed);
			currentMenuItem=menuItem::Feed;

			if(fLayer)					
				fLayer->release();
			fLayer = new FavoritLayer(_feedLayer);
			
			//fLayer->autorelease();
			break;  
		  
		default:  
			break;  
    }

}

void MainMenu::touchHome(Node* object,TouchEventType type)
{
	//LabelTTF* label;
	char *title="QUIT COLLAGE";
	std::string msg = MSG_WARN_QUIT;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			//MsgBox_Quit(Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg);
			DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","NOT NOW"
					,NULL,callfuncND_selector(MainMenu::MsgBox_Quit_Callback));
			break;  
		
		case TouchEventType::TOUCH_EVENT_CANCELED:  
			//label = static_cast(getChildByTag(11));  
			//label->setString("取消点击");  
			break;  
		default:  
			break;  
    }
	
}


void MainMenu::initialize(Node * rootNode) {

	//create main menu layer
	auto menuNode = CSLoader::createNode("Collage/LeftMenu.csb");
	//rootNode->addChild(menuNode,10);
	addChild(menuNode,10);
	menuNode->setTag(L_Menu_Tag);

	auto buttonProfile = dynamic_cast<Button*>(menuNode->getChildByName("Button_Member"));
	//buttonProfile->setTouchEnabled(true);
	buttonProfile->addTouchEventListener(this,toucheventselector(MainMenu::touchProfile));

	auto buttonTrends = dynamic_cast<Button*>(menuNode->getChildByName("Button_Trends"));
	buttonCreate = dynamic_cast<Button*>(menuNode->getChildByName("Button_Create"));
	auto buttonHome = dynamic_cast<Button*>(menuNode->getChildByName("Button_Home"));
	auto buttonHelp = dynamic_cast<Button*>(menuNode->getChildByName("Button_Help"));

	buttonTrends->setTag(B_TRENDS);
	
	/*auto _browseLayer = CSLoader::createNode("Collage/BrowseLayer.csb");
	menuNode->addChild(_browseLayer);
	SetButtonEnabled((Button*)menuNode->getChildByName("Button_Trends"),false);
	_browseLayer->setTag(MenuItem_Trends);
	currentMenuItem=menuItem::Trends;

	bLayer = new BrowseLayer(_browseLayer);*/
	


	//buttonTrends->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchHome,menuNode));
	buttonTrends->addTouchEventListener(this,toucheventselector(MainMenu::touchTrends));
	buttonCreate->addTouchEventListener(this,toucheventselector(MainMenu::touchCreate));
	buttonHome->addTouchEventListener(this,toucheventselector(MainMenu::touchHome));
	buttonHelp->addTouchEventListener(this,toucheventselector(MainMenu::touchHelp));
	
	//Add by Long for button shop
	auto buttonShop = dynamic_cast<Button*>(menuNode->getChildByName("Button_Shop"));
	buttonShop->addTouchEventListener(this, toucheventselector(MainMenu::touchShop));
	auto buttonFeeds = dynamic_cast<Button*>(menuNode->getChildByName("Button_Feed"));
	buttonFeeds->addTouchEventListener(this, toucheventselector(MainMenu::touchFeed));

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	long l_coins=app->GetUserTotalScore();
	char p[20];
	sprintf(p, "%ld", l_coins);
	G_Score = p;
	auto textPoint = dynamic_cast<Text*>(menuNode->getChildByName("Text_Point"));
	textPoint->setText(G_Score);

	auto nLevel =app->GetUserLevel();
	sprintf(p, "%d", nLevel);
	G_Level = p;
	auto textLevel = dynamic_cast<Text*>(menuNode->getChildByName("Text_Level"));
	textLevel->setText(G_Level);

	G_fP = app->GetPercentToNextLevel();
	auto LevelBar = dynamic_cast<ImageView*>(menuNode->getChildByName("Image_Level_Up"));
	LevelBar->setScaleX(G_fP);
	sprintf(p, "%d%%", (int)(G_fP*100));
	G_Percentage = p;

	// set button status according to download list
	if(!HttpConnection::v_design_like_info.size())	
	{
		SetButtonEnabled(buttonFeeds,false);
		if(!HttpConnection::v_design_time_info.size())
		{
			SetButtonEnabled(buttonTrends,false);
			//touchCreate(buttonCreate,TouchEventType::TOUCH_EVENT_ENDED);
		}
		else
		{
			//touchTrends(buttonTrends,TouchEventType::TOUCH_EVENT_ENDED);
		}
	}
	else
	{
		//touchFeed(buttonFeeds,TouchEventType::TOUCH_EVENT_ENDED);
	}
	touchCreate(buttonCreate,TouchEventType::TOUCH_EVENT_ENDED);

	//对手机返回键的监听 
	auto listener = EventListenerKeyboard::create(); 
	listener->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased,this); 
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this); 
}


//Add by Long for button shop
void MainMenu::touchShop(Node* object, TouchEventType type)
{
	//LabelTTF* label;
	if(DesignLayer::bLongPressed) return;

	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		/*auto scene = CheckoutScene::create();

		//scene->sProductID= m_strProductGUID;
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);*/
		Application::getInstance()->openURL("https://www.tweebaa.com/Product/prdSaleAll.aspx");
	}
		break;

	case TouchEventType::TOUCH_EVENT_CANCELED:
		//label = static_cast(getChildByTag(11));  
		//label->setString("取消点击");  
		break;
	default:
		break;
	}

}

void MainMenu::touchProfile(Node* object,TouchEventType type)
{
	if(DesignLayer::bLongPressed) return;
	ProfileLayer * pf;

	char *title=MSGBOX_TITLE;
	std::string msg;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(G_UID.empty())
			{
				msg = MSG_REGISTER_MSG;
				DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","NOT NOW"
					,NULL,callfuncND_selector(MainMenu::MsgBox_Reg_Callback));

				break;
			}
			pf = ProfileLayer::create("Collage/ProfileLayer.csb");
			pf->setCallbackFunc(this, callfuncND_selector(MainMenu::Forward2Create),sSelectFile);
			object->getParent()->addChild(pf);
			break;
		default:
			break;
	}
}

void MainMenu::MsgBox_Reg_Callback(cocos2d::Node *pNode,void* data)
{
	if(pNode->getTag())
	{
		auto scene = RegisterView::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}
	
}

void MainMenu::touchHelp(Node* object,TouchEventType type)
{

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			Director::getInstance()->pushScene(TransitionFade::create(1, TutorialScene::create()));
			break;
		default:
			break;
	}
}

void MainMenu::Forward2Create(cocos2d::Node *pNode,void *data)
{
	char *title="Load Failed";
	std::string msg = MSG_ERROR_GEN;

	if(DesignLayer::LoadDraft((const char *) data))
		touchCreate(buttonCreate,TouchEventType::TOUCH_EVENT_ENDED);
	else
	{
		DesignLayer::MsgBox(Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg);
	}
}

/*void MainMenu::MsgBox_Quit(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent){

    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 150);
	
    pl->setCallbackFunc(this, callfuncN_selector(MainMenu::MsgBox_Quit_Callback));
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "OK",Color3B(255,255,255), 1);
	pl->addButton("button_normal.png", "button_dis.png", "NOT NOW",Color3B(255,255,255), 0);

    // 添加到指定层
	baseLayer->addChild(pl);
}*/

void MainMenu::MsgBox_Quit_Callback(cocos2d::Node *pNode,void *data)
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	
	//if(app->iGameMode==MobileGameMode::Theme_Mode || pNode->getTag())
	if(pNode->getTag())
	{
		currentMenuItem=menuItem::Home;
		DesignLayer::sp_info_vec.clear();
		DesignLayer::sp_vec.clear();
		DesignLayer::lb_vec.clear();
		DesignLayer::UndoClear();
		DesignLayer::RedoClear();
		HttpConnection::v_template_cache_filename.clear();
		HttpConnection::v_design_time_info.clear();
		if(!CreateLayer::m_bIsTheme)	//for theme mode published, gotta keep this vector
			HttpConnection::v_design_like_info.clear();
		HttpConnection::v_product_info.clear();
		//HttpConnection::v_decoration_info.clear();

		if(fLayer)
		{
			fLayer->release();
			fLayer=nullptr;
		}
		if(cLayer)
		{
			//delete cLayer;
			cLayer->release();
			cLayer=nullptr;
		}
		if(bLayer)
		{
			bLayer->Layer::release();
			bLayer=nullptr;
		}
			
		if(app->iGameMode==MobileGameMode::Theme_Mode)
		{
			if(!CreateLayer::m_bIsTheme)	//return to theme mode round choose layer
			{
				auto scene = ThemeLayer::createScene();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
			else  //next round
			{
				app->GameRunRoundNext();
			}
		}
		else
		{
			auto scene = ChooseLayer::createScene();
			//auto scene = CollageMainTab::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}

	}
	
}


void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) 
{
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_HOME || keyCode == EventKeyboard::KeyCode::KEY_MENU)
	{
		touchHome(this,TouchEventType::TOUCH_EVENT_ENDED);
	}
}
