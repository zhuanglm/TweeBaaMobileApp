#include "MyAccountLayer.h"
#include "../memory_match/GameScene.h"
#include "../jigsaw/jigSawScene.h"
#include "WelcomeScene.h"
#include "../MemberCenter/RegisterTabScene.h"
#include "global_const.h"
#include "ChooseLayer.h"

MyAccountLayer::MyAccountLayer(){};

MyAccountLayer::~MyAccountLayer(){};


Scene* MyAccountLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MyAccountLayer::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void MyAccountLayer::menuCallback(Ref *sender) {

		//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());
		auto scene = RegisterTab::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
}

bool MyAccountLayer::init(){
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
		auto menuItem_7  = MenuItemSprite::create(game7,game7_a,NULL,CC_CALLBACK_1(MyAccountLayer::menuCallback, this));
		menuItem_7->setPosition(Vec2(40 , visiableSize.height -40));

		auto menu7 = Menu::create(menuItem_7,NULL);
		menu7->setPosition(Point(origin.x ,origin.y));
		this->addChild(menu7,1);

		
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

void MyAccountLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  
		auto scene = ChooseLayer::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
		break;
	}

}

void MyAccountLayer::scrollLand(float dt){
	/*
	this->land1->setPositionX(this->land1->getPositionX() - 2.0f);
	this->land2->setPositionX(this->land1->getPositionX() + this->land1->getContentSize().width - 2.0f);

	if(this->land2->getPositionX() == 0) {
		this->land1->setPositionX(0);
	}*/
}


