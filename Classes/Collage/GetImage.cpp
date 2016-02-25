//===========Tweebaa==============
//GetImage.cpp	--Dec 2 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#include "GetImage.h"
//--------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import  "ImageViewController.h"
#import  "RootViewController.h"
#endif
//--------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define JAVA_CLASS              "org/cocos2dx/cpp/GetImage"
#define JAVA_FUNC_OPEN_PHOTO    "openPhoto"
#define JAVA_FUNC_OPEN_CAMERA   "openCamera"
#endif
//--------------------------------------------------
USING_NS_CC;
//--------------------------------------------------
GetImage* GetImage::s_instance = NULL;
//--------------------------------------------------
GetImage* GetImage::getInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new GetImage();
    }
    return s_instance;
}
//--------------------------------------------------
void GetImage::destoryInstance()
{
    CC_SAFE_DELETE(s_instance);
}
//--------------------------------------------------
GetImage::GetImage()
:m_callback(nullptr)
{
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("GetImageEvent", [=](EventCustom* eve)
    {
        std::string* path = (std::string*)eve->getUserData();
        if (path && m_callback != nullptr)
        {
            m_callback(*path);
        }
    });
}
//--------------------------------------------------
void GetImage::callGetImageWithPhotoAndCamera(const std::function<void(std::string)>& callback)
{
    s_instance->init();
    setListener(callback);
}
//--------------------------------------------------
void GetImage::setListener(const std::function<void(std::string)>& callback)
{
    m_callback = callback;
}
//--------------------------------------------------
void GetImage::removeListener()
{
    m_callback = nullptr;
}
//--------------------------------------------------
void GetImage::openPhoto()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ImageViewController* imageViewController = [[ImageViewController alloc] initWithNibName:nil bundle:nil];
    
    RootViewController* _viewController = (RootViewController*)m_viewController;
    [_viewController.view addSubview:imageViewController.view];
    
    [imageViewController localPhoto];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info, JAVA_CLASS, JAVA_FUNC_OPEN_PHOTO,"()V");
    if (ret)
    {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
    }
#endif
}
//--------------------------------------------------
void GetImage::openCamera()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ImageViewController* imageViewController = [[ImageViewController alloc] initWithNibName:nil bundle:nil];
    
    RootViewController* _viewController = (RootViewController*)m_viewController;
    [_viewController.view addSubview:imageViewController.view];
    
    [imageViewController takePhoto];
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info, JAVA_CLASS, JAVA_FUNC_OPEN_CAMERA,"()V");
    if (ret)
    {
        info.env->CallStaticVoidMethod(info.classID, info.methodID);
    }
#endif
}
//--------------------------------------------------
bool GetImage::init()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //-------------------------------------
    // ����
    //-------------------------------------
    LayerColor* m_layer = LayerColor::create(Color4B(0, 0, 0, 125));
    m_layer->retain();
    //-------------------------------------
    // ��ť����
    //-------------------------------------
    Sprite* sprite = Sprite::create("Collage/bk.png");
    sprite->setAnchorPoint(Vec2(0.5, 0));
    sprite->setPosition(Vec2(visibleSize.width/2, 0));
    m_layer->addChild(sprite);
    //-------------------------------------
    // ��ť
    //-------------------------------------
    Menu* menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    m_layer->addChild(menu);
    //-------------------------------------
    MenuItemImage* btnPhoto  = MenuItemImage::create("Collage/ButtonPhoto.png",  "Collage/ButtonPhoto1.png",  [=](Ref* p)
    {
        openPhoto();
    });
    btnPhoto->setAnchorPoint(Vec2(0.5, 1));
    btnPhoto->setPosition(Vec2(visibleSize.width / 2, 280));
    menu->addChild(btnPhoto);
    //-------------------------------------
    MenuItemImage* btnCamera = MenuItemImage::create("Collage/ButtonCamera.png", "Collage/ButtonCamera1.png", [=](Ref* p)
    {
        openCamera();
    });
    btnCamera->setAnchorPoint(Vec2(0.5, 1));
    btnCamera->setPosition(btnPhoto->getPosition() + Vec2(0, -btnPhoto->getContentSize().height));
    menu->addChild(btnCamera);
    //-------------------------------------
    MenuItemImage* btnCancel = MenuItemImage::create("Collage/ButtonCancel.png", "Collage/ButtonCancel1.png", [=](Ref* p)
    {
        float height = sprite->getContentSize().height;

        MoveBy* move = MoveBy::create(0.2, Vec2(0, -height));
        sprite->runAction(move);
        menu  ->runAction(move->clone());

        Sequence* seq = Sequence::createWithTwoActions(FadeOut::create(0.2), RemoveSelf::create());
        m_layer->runAction(seq);
    });
    btnCancel->setAnchorPoint(Vec2(0.5, 1));
    btnCancel->setPosition(btnCamera->getPosition() + Vec2(0, -btnCamera->getContentSize().height - 20));
    menu->addChild(btnCancel);
    //-------------------------------------
    // ����
    //-------------------------------------
    Label* textPhoto  = Label::createWithSystemFont("Photo",  "", 24);
    textPhoto->setPosition(btnPhoto->getContentSize() / 2);
    textPhoto->setTextColor(Color4B::BLACK);
    btnPhoto->addChild(textPhoto);
    //-------------------------------------
    Label* textCamera = Label::createWithSystemFont("Camera", "", 24);
    textCamera->setPosition(btnPhoto->getContentSize() / 2);
    textCamera->setTextColor(Color4B::BLACK);
    btnCamera->addChild(textCamera);
    //-------------------------------------
    Label* textCancel = Label::createWithSystemFont("Cancel", "", 24);
    textCancel->setPosition(btnPhoto->getContentSize() / 2);
    textCancel->setTextColor(Color4B::BLACK);
    btnCancel->addChild(textCancel);
    //-------------------------------------
    // ׼����ʾ
    //-------------------------------------
    Director::getInstance()->getRunningScene()->scheduleOnce([=](float time)
    {
        Director::getInstance()->getRunningScene()->addChild(m_layer, INT_MAX);
        m_layer->release();

        float height = sprite->getContentSize().height;

        sprite->setPositionY(sprite->getPositionY() - height);
        menu  ->setPositionY(menu->getPositionY()   - height);

        MoveBy* move = MoveBy::create(0.3, Vec2(0, height));
        sprite->runAction(move);
        menu  ->runAction(move->clone());

        m_layer->setOpacity(0);
        m_layer->runAction(FadeTo::create(0.2, 125));

    }, 0.1, "GetImageScheduleOnce");
    //-------------------------------------
    // �ض��¼�
    //-------------------------------------
    EventListenerTouchOneByOne* touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->setSwallowTouches(true);
    touchEvent->onTouchBegan = [=](Touch* touch, Event* eve)
    {
        if(sprite->getBoundingBox().containsPoint(touch->getLocation()))
            return true;
        
        float height = sprite->getContentSize().height;
        
        MoveBy* move = MoveBy::create(0.2, Vec2(0, -height));
        sprite->runAction(move);
        menu  ->runAction(move->clone());
        
        Sequence* seq = Sequence::createWithTwoActions(FadeOut::create(0.2), RemoveSelf::create());
        m_layer->runAction(seq);
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, sprite);
    //-------------------------------------
    return true;
}
//--------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void  GetImage::setViewController(void* viewController)
{
    m_viewController = viewController;
}
#endif
//--------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    void Java_org_cocos2dx_cpp_GetImage_onImageSaved(JNIEnv* env, jobject thiz, jstring path)
    {
        std::string strPath = JniHelper::jstring2string(path);
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("GetImageEvent", &strPath);
    }
}
#endif