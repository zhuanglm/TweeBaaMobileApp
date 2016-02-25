#include "TweebaaMobileLayer.h"
#include "memory_match/GameScene.h"
#include "jigsaw/jigSawScene.h"
#include "WelcomeScene.h"
#include "MemberCenter/RegisterTabScene.h"
#include "Login.h"

TweebaaMobileLayer::TweebaaMobileLayer(){};

TweebaaMobileLayer::~TweebaaMobileLayer(){};


Scene* TweebaaMobileLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TweebaaMobileLayer::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void TweebaaMobileLayer::menuCallback(Ref *sender) {

		//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());
		auto scene = WelcomeScene::create();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
}

bool TweebaaMobileLayer::init(){
	if(!Layer::init()){
		return false;
	}
	//get the origin point of the X-Y axis, and the visiable size of the screen
	Size visiableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	float generalscalefactor  = visiableSize.height / 500 ;
	
		Sprite *game7 = Sprite::create("Back_60.png");
		Sprite *game7_a = Sprite::create("Back_60.png");//Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
		game7_a->setPositionY(5);
		auto menuItem_7  = MenuItemSprite::create(game7,game7_a,NULL,CC_CALLBACK_1(TweebaaMobileLayer::menuCallback, this));
		menuItem_7->setPosition(Vec2(40 , visiableSize.height -40));

		auto menu7 = Menu::create(menuItem_7,NULL);
		menu7->setPosition(Point(origin.x ,origin.y));
		this->addChild(menu7,1);

		Size webViewSize;
		webViewSize.setSize(visiableSize.width*0.96,visiableSize.height);
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//Size winSize = Director::getInstance()->getVisibleSize();

		cocos2d::experimental::ui::WebView* webView= cocos2d::experimental::ui::WebView::create();
		webView->setPosition(Vec2(visiableSize.width*0.5 -20 , visiableSize.height / 2 - menuItem_7->getContentSize().height -20));
		webView->setContentSize(webViewSize );
		webView->loadURL("https://www.tweebaa.com/");
		webView->setScalesPageToFit(true);
		this->addChild(webView);
	#else#
		auto uButtonA = ::ui::Button::create();
		uButtonA->setTouchEnabled(true);
		
		uButtonA->setTitleText("Sorry ,Embed web browser not Support, Click here to Return");
		uButtonA->setTitleFontSize(30.0f);
		//uButton->loadTextures("cocosgui/animationbuttonnormal.png", "cocosgui/animationbuttonpressed.png", "");
		uButtonA->setPosition(Point(visiableSize.width / 2, visiableSize.height / 2) );
		uButtonA->addTouchEventListener(this, ::ui::SEL_TouchEvent(&TweebaaMobileLayer::touchButton));

		addChild(uButtonA,-1,1001);
	#endif
		//delete webViewSize;
	//::ui::UIWebView* webView
	/*
	ZYWebView* webview = new ZYWebView();
	webview->init();
	webview->showWebView("https://tweebaa.com/Product/saleBuy.aspx?id=670bdf26-a935-4643-ac0e-ba24c2249107, 0, 0, visiableSize.width, visiableSize.height);
	// Then a webview window will appear in your game.

	// call these method as you wish
	webview->updateURL("http://blog.go3k.org");
	webview->removeWebView();

//delete the pointer in the end
delete webview;
	*/
	return true;
}

void TweebaaMobileLayer::touchButton(Ref* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  
		auto scene = Login::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
		break;
	}

}

void TweebaaMobileLayer::scrollLand(float dt){
	/*
	this->land1->setPositionX(this->land1->getPositionX() - 2.0f);
	this->land2->setPositionX(this->land1->getPositionX() + this->land1->getContentSize().width - 2.0f);

	if(this->land2->getPositionX() == 0) {
		this->land1->setPositionX(0);
	}*/
}


