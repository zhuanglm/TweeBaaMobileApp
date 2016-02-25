#include "RegisterTabScene.h"
//#include "memory_match/memory_match_welcome.h"
#include "ChooseLayer.h"
#include "global_const.h"
#include "Login.h"
#include "RegisterView.h"


#define TOOLBARTAG 10086

USING_NS_CC;



Scene* RegisterTab::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RegisterTab::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool RegisterTab::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
   
    layers=NULL;
	tabBarItems=NULL;
	tabBarBg=NULL;

	//Login* layer1 = Login::create();
    LayerColor *layer1 = LayerColor::create(Color4B(180, 180, 0, 180));
    
	layer1->ignoreAnchorPointForPosition(true);
    //layer1->init();

	LayerColor *layer2 = LayerColor::create(Color4B(0, 180, 180,  180));
	//layer2->init();
    
    //LayerColor *layer3 = LayerColor::create(Color4B(0, 180, 180,  180));
    
    
    //LayerColor *layer4 = LayerColor::create(Color4B(255, 255, 0, 180));
	layers=LayerMultiplex::create(layer1,layer2,NULL);
	
   /* layers->addLayer(layer1);
    layers->addLayer(layer2);
	layers->addLayer(layer3);
	layers->addLayer(layer4);*/
	//layers->inse
    this -> addChild(layers);
    
    
    tabBarBg = Sprite::create("tarbarBg.png");
	tabBarBg -> setPosition(Director::getInstance() ->  getVisibleSize().width * 0.5, visibleSize.height - 80);
    tabBarBg -> setAnchorPoint(Point(0.5, 0));
    
    this -> addChild(tabBarBg, 1000);
    auto toolbar1 = MenuItemSprite::create(Sprite::create("toolbarLogin.png"), Sprite::create("toolbarLogin_p.png"), CC_CALLBACK_1(RegisterTab::tapBarMenuPressed, this));
    toolbar1 -> setTag(TOOLBARTAG + 0);
    
    auto toolbar2 = MenuItemSprite::create(Sprite::create("toolbarReg.png"), Sprite::create("toolbarReg_p.png"), CC_CALLBACK_1(RegisterTab::tapBarMenuPressed, this));
    toolbar2 -> setTag(TOOLBARTAG + 1);
     /*
    auto toolbar3 = MenuItemSprite::create(Sprite::create("toolbar3.png"), Sprite::create("toolbar3_p.png"), CC_CALLBACK_1(RegisterTab::tapBarMenuPressed, this));
    toolbar3 -> setTag(TOOLBARTAG + 2);
   
    auto toolbar4 = MenuItemSprite::create(Sprite::create("toolbar4.png"), Sprite::create("toolbar4_p.png"), CC_CALLBACK_1(RegisterTab::tapBarMenuPressed, this));
    toolbar4 -> setTag(TOOLBARTAG + 3);
    */
    tabBarItems = Menu::create(toolbar1, toolbar2,  NULL);
	
	tabBarItems->alignItemsHorizontally();
    tabBarItems -> alignItemsHorizontallyWithPadding(30);

    tabBarItems -> setPositionY(tabBarBg -> getContentSize().height * 0.5f);
    tabBarItems->setPositionX(Director::getInstance() ->  getVisibleSize().width * 0.335);
    tabBarBg -> addChild(tabBarItems);
    return true;
}


void RegisterTab::onEnter()
{
    Layer::onEnter();
    layers -> switchTo(0);
    static_cast<MenuItemSprite *>(tabBarItems -> getChildByTag(TOOLBARTAG + 0)) -> selected();
}

void RegisterTab::tapBarMenuPressed(Ref *pSender)
{
    auto toolbar = (MenuItemSprite *)pSender;
    toolbar -> selected();
    layers -> switchTo(toolbar -> getTag() - TOOLBARTAG);
    
    for (int i = 0; i < tabBarItems -> getChildren().size(); i++) {
        auto tmpbar = dynamic_cast<MenuItemSprite *>(tabBarItems -> getChildren().at(i));
        if (tmpbar != NULL && tmpbar -> getTag() != toolbar -> getTag()) {
            tmpbar -> unselected();
        }
    }
}