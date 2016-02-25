#include "CCAlert.h"


CCAlert::CCAlert(void):m_delegate(NULL)
{
}


CCAlert::~CCAlert(void)
{
}

CCAlert* CCAlert::create(std::string sMessage,int alertTag, CCAlertDelegate* del, AlertType type){
    CCAlert* alert = new CCAlert;
    if (alert&& alert->init(sMessage,alertTag, del, type))
    {
        alert->autorelease();
    }else{
        CC_SAFE_DELETE(alert);
        alert = NULL;
    }

    return alert;
}

bool CCAlert::init(std::string sMessage,int alertTag, CCAlertDelegate* del, AlertType type){
    CCLayer::init();
    m_delegate = del;
    m_alertType = type;


    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	
	Sprite* background = Sprite::create("AlertBg.png");
		
	background->setScale(winSize.width / background->getContentSize().width);
	background->setAnchorPoint(Vec2(0.0f,1.0f)) ;
	background->setPosition(Vec2(0, winSize.height));
	addChild(background,5);

	float generalscalefactor  = winSize.height / 500 ;

	CCLabelTTF*	statusLabel =CCLabelTTF::create(sMessage, "fonts/Marker Felt.ttf",20,Size(300, 300));
	statusLabel->setScale(1.3f*generalscalefactor);
	statusLabel->setAnchorPoint(Vec2(0,1));
	
	//statusLabel->setContentSize(Size(200, 75));
	statusLabel->setPosition( ccp( winSize.width/2-200, winSize.height /2));
	addChild(statusLabel,6);
	/*
    CCLayerColor* colorLayer = CCLayerColor::create(ccc4(100, 100, 100, 100));
    colorLayer->setContentSize(winSize);
    colorLayer->setAnchorPoint(Vec2(0,0));
    addChild(colorLayer);
	*/
    //onOk item
    CCMenuItemFont* okItem = CCMenuItemFont::create("Yes", this, menu_selector(CCAlert::alertFunc));
    okItem->setTag(Item_OK_tag);
    //onCancel item
    CCMenuItemFont* cancelItem = CCMenuItemFont::create("No", this, menu_selector(CCAlert::alertFunc));
    cancelItem->setTag(Item_Cancel_tag);
    CCMenu* menu = CCMenu::create();
    switch (m_alertType)
    {
    case AlertType_OkAndCancel:
        {
            menu->addChild(okItem);
            menu->addChild(cancelItem);
            menu->alignItemsHorizontallyWithPadding(winSize.width*0.2);
        }break;
    case AlertType_OkOnly:
        {
            menu->addChild(okItem);
        }break;
    default:break;
    }
    
    menu->setPosition(Vec2(winSize.width/2, winSize.height*0.3));
    addChild(menu,6);
    return true;
}

void CCAlert::alertFunc(CCObject* pSender){
    int tag = ((CCMenuItem*)pSender)->getTag();
    switch (tag)
    {
    case Item_OK_tag:
        {
            m_delegate->onOk(this);
        }break;
    case Item_Cancel_tag:
        {
            m_delegate->onCancel(this);
        }break;
    default:break;
    }

    this->removeFromParentAndCleanup(true);
}

void CCAlert::onEnter(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
    CCLayer::onEnter();
}

void CCAlert::onExit(){
  //  CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
/*
bool CCAlert::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void CCAlert::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}
*/