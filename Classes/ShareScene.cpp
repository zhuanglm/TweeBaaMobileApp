#include "ShareScene.h"
//#include "memory_match/memory_match_welcome.h"
#include "ChooseLayer.h"
#include "C2DXShareSDK/C2DXShareSDK.h"
#include "WelcomeScene.h"

USING_NS_CC;
using namespace cn::sharesdk;

void authResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
           log("授权成功");
            break;
        case C2DXResponseStateFail:
          log("授权失败");
            break;
        default:
            break;
    }
}

void getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *userInfo, CCDictionary *error)
{
    if (state == C2DXResponseStateSuccess)
    {
        //输出用户信息  
        try
        {
            CCArray *allKeys = userInfo -> allKeys();
			allKeys->retain();
            for (int i = 0; i < allKeys -> count(); i++)
            {
                CCString *key = (CCString *)allKeys -> objectAtIndex(i);
                CCObject *obj = userInfo -> objectForKey(key -> getCString());
                
               log("key = %s", key -> getCString());
                if (dynamic_cast<CCString *>(obj))
                {
                   log("value = %s", dynamic_cast<CCString *>(obj) -> getCString());
                }
                else if (dynamic_cast<CCInteger *>(obj))
                {
                   log("value = %d", dynamic_cast<CCInteger *>(obj) -> getValue());
                }
                else if (dynamic_cast<CCDouble *>(obj))
                {
                   log("value = %f", dynamic_cast<CCDouble *>(obj) -> getValue());
                }
            }
			allKeys->release();
        }
        catch(...)
        {
            log("==============error");
        }
        
    }
}

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
			cocos2d::MessageBox("Shared Success","Share");
            log("分享成功");
            break;
        case C2DXResponseStateFail:
			cocos2d::MessageBox("Shared Failed","Share");
           log("分享失败");
            break;
        default:
            break;
    }
}

Scene* ShareSceneLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ShareSceneLayer::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ShareSceneLayer::init()
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
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	/*
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(ShareSceneLayer::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	*/
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    /*
    auto label = Label::createWithTTF("At Tweebaa,Everyone Earns", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "ShareSceneLayer" splash screen"
    auto sprite = Sprite::create("mascot.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    */

	/*
	   CCMenuItemLabel *authMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("Author", "Arial", 40),
                                                            this,
                                                            menu_selector(ShareSceneLayer::authMenuItemClick));
    CCMenuItemLabel *cancelAuthMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("Cancel Author", "Arial", 40),
                                                                  this,
                                                                  menu_selector(ShareSceneLayer::cancelAuthMenuItemClick));
    CCMenuItemLabel *hasAuthMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("Check Author", "Arial", 40),
                                                               this,
                                                               menu_selector(ShareSceneLayer::hasAuthMenuItemClick));
    CCMenuItemLabel *getUserMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("User Info", "Arial", 40),
                                                               this,
                                                               menu_selector(ShareSceneLayer::getUserInfoMenuItemClick));
    CCMenuItemLabel *shareMenuItem = CCMenuItemLabel::create(CCLabelTTF::create("Share", "Arial", 40),
                                                             this,
                                                             menu_selector(ShareSceneLayer::shareMenuItemClick));

	  CCMenuItemLabel *checkClientItem = CCMenuItemLabel::create(CCLabelTTF::create("Check Client","Arial", 40), this, menu_selector(ShareSceneLayer::checkClientItemClick));
  

    CCMenu *itemsMenu = CCMenu::create(authMenuItem, cancelAuthMenuItem, hasAuthMenuItem, getUserMenuItem, shareMenuItem, checkClientItem,NULL);
    itemsMenu -> alignItemsHorizontallyWithPadding(20);
    itemsMenu -> setPosition(Vec2(CCDirector::sharedDirector() -> getWinSize().width / 2, 100));
    this -> addChild(itemsMenu);
	*/
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCDictionary *content = CCDictionary::create();
		content -> setObject(CCString::create("这是一条测试内容"), "content");
		content -> setObject(CCString::create("http://img0.bdstatic.com/img/image/shouye/systsy-11927417755.jpg"), "image");
		content -> setObject(CCString::create("测试标题"), "title");
		content -> setObject(CCString::create("测试描述"), "description");
		content -> setObject(CCString::create("http://sharesdk.cn"), "url");
		content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
		content -> setObject(CCString::create("http://sharesdk.cn"), "siteUrl");
		content -> setObject(CCString::create("ShareSDK"), "site");
		content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
		content -> setObject(CCString::create("extInfo"), "extInfo");
    
		C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
	#else#
		auto uButtonA = ::ui::Button::create();
		uButtonA->setTouchEnabled(true);
		uButtonA->setTitleText("Sorry ,Embed web browser not Support, Click here to Return");
		uButtonA->setTitleFontSize(30.0f);
		//uButton->loadTextures("cocosgui/animationbuttonnormal.png", "cocosgui/animationbuttonpressed.png", "");
		uButtonA->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2) + Point(-200, -180));
		uButtonA->addTouchEventListener(this, ::ui::SEL_TouchEvent(&ShareSceneLayer::touchButton));

		addChild(uButtonA,-1,1001);
	#endif
		//add the start-menu to the current scene
	/*
	Sprite *startButton = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	Sprite *activeStartButton = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("button_play"));
	activeStartButton->setPositionY(5);
	auto menuItem  = MenuItemSprite::create(startButton,activeStartButton,NULL,CC_CALLBACK_1(WelcomeLayer::menuStartCallback, this));
	menuItem->setPosition(Point(origin.x + visibleSize.width/2 ,origin.y + visibleSize.height*2/5));

    auto menu = Menu::create(menuItem,NULL);
	menu->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu,1);
	*/

		// start ansyc method load the atlas.png
	//Director::getInstance()->getTextureCache()->addImageAsync("MatchBg.png", CC_CALLBACK_1(ShareSceneLayer::loadingCallBack, this));

    return true;
}
void ShareSceneLayer::touchButton(Ref* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
    switch(eventType)  
    {  
	case ::ui::Widget::TouchEventType::ENDED:  
		auto scene = WelcomeScene::create();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
		break;
	}

}
void ShareSceneLayer::checkClientItemClick(Ref* pSender)
{
	/*
    if (C2DXShareSDK::isClientInstalled(C2DXPlatTypeQQ))
    {
       log("客户端已安装");
    }else
    {
       log("客户端未安装");
    }*/
}
void ShareSceneLayer::authMenuItemClick(Ref* pSender)
{
//    C2DXShareSDK::authorize(C2DXPlatTypeFacebook, authResultHandler);
}

void ShareSceneLayer::cancelAuthMenuItemClick(Ref* pSender)
{
//    C2DXShareSDK::cancelAuthorize(C2DXPlatTypeSinaWeibo);
}
void ShareSceneLayer::hasAuthMenuItemClick(Ref* pSender)
{
    if (C2DXShareSDK::hasAutorized(C2DXPlatTypeSinaWeibo))
    {
       log("用户已授权");
    }
    else
    {
       log("用户尚未授权");
    }
}

void ShareSceneLayer::getUserInfoMenuItemClick(Ref* pSender)
{
    C2DXShareSDK::getUserInfo(C2DXPlatTypeSinaWeibo, getUserResultHandler);
}
void ShareSceneLayer::shareMenuItemClick(Ref* pSender)
{
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create("这是一条测试内容"), "content");
    content -> setObject(CCString::create("http://img0.bdstatic.com/img/image/shouye/systsy-11927417755.jpg"), "image");
    content -> setObject(CCString::create("测试标题"), "title");
    content -> setObject(CCString::create("测试描述"), "description");
    content -> setObject(CCString::create("http://sharesdk.cn"), "url");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
    content -> setObject(CCString::create("http://sharesdk.cn"), "siteUrl");
    content -> setObject(CCString::create("ShareSDK"), "site");
    content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
    content -> setObject(CCString::create("extInfo"), "extInfo");
    
    C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
//    C2DXShareSDK::showShareView(C2DXPlatTypeSinaWeibo, content, shareResultHandler);
}

//Loading Call back
void ShareSceneLayer::loadingCallBack(Texture2D *texture){
	auto scene = ChooseLayer::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}
/*
void ShareSceneLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
*/