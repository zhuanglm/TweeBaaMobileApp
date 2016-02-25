
//===========Tweebaa==============
//PublishLayer.cpp	--Oct 8 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#include "PublishLayer.h"
#include "DesignLayer.h"

std::string PublishLayer::InputCont1;
std::string PublishLayer::InputCont2;

PublishLayer::PublishLayer():
m__pMenu(NULL)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m__pPopBox(NULL)
{
}
 
PublishLayer::~PublishLayer(){
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__pPopBox);
}
 
bool PublishLayer::init(){
    if(!Layer::init()){
        return false;
    }
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PublishLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setContentSize(Size::ZERO);
    
    // 初始化需要的 Menu
    Menu* menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
    setMenuButton(menu);
         
    setTouchEnabled(true);
	m_bPosFlag = false;
 
    return true;
}
 
bool PublishLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    //log("PopupLayer touch");
    return true;
}
 
PublishLayer* PublishLayer::create(const char *backgroundCSB,bool OK_Flag,const char * pic_file){
    
	PublishLayer* ml = PublishLayer::create();
    //ml->setSpriteBackGround(Sprite::create(backgroundImage));
    //ml->setSprite9BackGround(Scale9Sprite::create(backgroundImage));

	ml->setBackGround(CSLoader::createNode(backgroundCSB));
	ml->setOK_Flag(OK_Flag);
	ml->setPic_File(pic_file);

    return ml;
}
 
 
void PublishLayer::setCallbackFunc(cocos2d::Object *target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;    
}
 
void PublishLayer::onEnter(){
    Layer::onEnter();
     
    Size winSize = Director::sharedDirector()->getWinSize();
     
    Size contentSize;

    // 设定好参数，在运行时加载
	if (getContentSize().equals(Size::ZERO)) {
		getBackGround()->setPosition(Vec2::ZERO);
		auto layerColor = LayerColor::create();
		layerColor->setColor(Color3B(0, 0, 0));  
		layerColor->setOpacity(80);
        layerColor->addChild(getBackGround(), 0, 0);
		this->addChild(layerColor);
    } 
     
	auto ButtonPublish = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_OK"));
	auto ButtonCancel = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_Cancel"));
	/*auto ButtonFb = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_Facebook"));
	auto ButtonTt = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_Twitter"));
	auto ButtonPin = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_Pinterest"));
	auto ButtonGP = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_GPlus"));*/
	auto Picture = dynamic_cast<ImageView*>(m__pPopBox->getChildByName("Image_Display"));
	TextDes = dynamic_cast<TextField*>(m__pPopBox->getChildByName("TextField_Description"));
	TextTitle = dynamic_cast<TextField*>(m__pPopBox->getChildByName("TextField_Title"));

	TextTitle->setString(InputCont1);
	TextTitle->addEventListener(CC_CALLBACK_2(PublishLayer::textFieldEvent, this));
	TextDes->setString(InputCont2);
	TextDes->addEventListener(CC_CALLBACK_2(PublishLayer::textFieldEvent, this));

	ButtonPublish->setTag(MenuItem_OK);
	ButtonPublish->setName(m__pPicFile);		//transfer pic filename
	SetButtonEnabled(ButtonPublish,false);
	ButtonCancel->setTag(MenuItem_CANCEL);
	/*ButtonFb->setTag(MenuItem_Facebook);
	ButtonTt->setTag(MenuItem_Twitter);
	ButtonPin->setTag(MenuItem_Pinterest);
	ButtonGP->setTag(MenuItem_GPlus);*/
	
	ButtonPublish->addTouchEventListener(this,toucheventselector(PublishLayer::buttonCallback));
	ButtonCancel->addTouchEventListener(this,toucheventselector(PublishLayer::buttonCallback));
	/*ButtonFb->addTouchEventListener(this,toucheventselector(PublishLayer::buttonCallback));
	ButtonTt->addTouchEventListener(this,toucheventselector(PublishLayer::buttonCallback));
	ButtonPin->addTouchEventListener(this,toucheventselector(PublishLayer::buttonCallback));
	ButtonGP->addTouchEventListener(this,toucheventselector(PublishLayer::buttonCallback));*/

	if(m__pPicFile!=NULL)
	{
		TextureCache::getInstance()->reloadTexture(m__pPicFile);
		Picture->loadTexture(m__pPicFile);
	}
	
	if(m_OK_Flag)
	{
		/*SetButtonEnabled(ButtonPublish,false);
		ButtonCancel->loadTextureNormal("Collage/button_normal.png");
		ButtonCancel->loadTexturePressed("Collage/button_normal.png");
		ButtonCancel->setTitleFontName("fonts/HUM521B.TTF");
		ButtonCancel->setTitleFontSize(18);
		ButtonCancel->setTitleText("RETURN");
		string tt = "Press the buttons listed bellow";
		tt += TextTitle->getString();
		TextTitle->setString(tt);
		TextTitle->setColor(Color3B::BLUE);
		TextTitle->setEnabled(false);
		TextDes->setEnabled(false);
		SetButtonEnabled(ButtonFb,true);
		SetButtonEnabled(ButtonTt,true);*/
	}
	else
	{
		//SetButtonEnabled(ButtonPublish,true);
		/*SetButtonEnabled(ButtonFb,false);
		SetButtonEnabled(ButtonTt,false);*/
	}
     
      
 
    // 弹出效果
    Action* popupLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
                                              ScaleTo::create(0.18, 1.05),
                                              ScaleTo::create(0.24, 0.95),
                                              ScaleTo::create(0.24, 1.0), NULL);
    this->runAction(popupLayer);
											
 
}

void PublishLayer::buttonCallback(cocos2d::Object *pSender,TouchEventType type)
{
    if(type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		InputCont1 = TextTitle->getString();
		InputCont2 = TextDes->getString();
		Node* node = dynamic_cast<Node*>(pSender);
		/*auto tt = InputCont1;
		tt+=" is publishing...";
		TextTitle->setString(tt);*/
log("touch tag: %d", node->getTag());
		if (m_callback && m_callbackListener){
			(m_callbackListener->*m_callback)(node);
		}
		this->removeFromParent();
	}
}

void PublishLayer::PublishCallback(cocos2d::Object *pSender,TouchEventType type)
{
}
 
void PublishLayer::onExit(){
     
//log("popup on exit.");
    Layer::onExit();
}

void PublishLayer::textFieldEvent(Ref * pSender, TextField::EventType type)
{
#if (CC_TARGET_PLATFORM == CC_IOS) 
	auto textfield = dynamic_cast<TextField*>(pSender);
	auto s = textfield->getName();
#endif
	
	auto ButtonPublish = dynamic_cast<Button*>(m__pPopBox->getChildByTag(MenuItem_OK));
	switch(type)
	{
		case TextField::EventType::INSERT_TEXT:
			SetButtonEnabled(ButtonPublish,true);
			break;
		case TextField::EventType::DELETE_BACKWARD:
			if(TextTitle->getString().size()>0)
			{
				//log("typied already");
			}
			else
			{
				//log("empty");
				SetButtonEnabled(ButtonPublish,false);
			}
			break;
#if (CC_TARGET_PLATFORM == CC_IOS)
		case TextField::EventType::ATTACH_WITH_IME:
			if(!m_bPosFlag && !s.compare("TextField_Description"))
			{
				m_bPosFlag=true;
				m__pPopBox->setPosition(Vec2(m__pPopBox->getPosition().x,m__pPopBox->getPosition().y+100));
			}
			break;
		case TextField::EventType::DETACH_WITH_IME:
			if(m_bPosFlag && !s.compare("TextField_Description"))
			{
				m_bPosFlag=false;
				m__pPopBox->setPosition(Vec2(m__pPopBox->getPosition().x,m__pPopBox->getPosition().y-100));
			}
			break;
#endif
		default:
			break;
	}
}

/*bool 	PublishLayer::onTextFieldAttachWithIME (TextFieldTTF *sender)
{
	return true;
}
bool 	PublishLayer::onTextFieldDetachWithIME (TextFieldTTF *sender)
{
	return true;
}
bool 	PublishLayer::onTextFieldInsertText (TextFieldTTF *sender, const char *text, size_t nLen)
{
	
	return true;
}
bool 	PublishLayer::onTextFieldDeleteBackward (TextFieldTTF *sender, const char *delText, size_t nLen)
{
	return true;
}
bool 	PublishLayer::onVisit (TextFieldTTF *sender, Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	return true;
}*/