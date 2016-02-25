
//===========Tweebaa==============
//PopupLayer.cpp	--Sep 16 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#include "PopupLayer.h"

PopupLayer::PopupLayer():
m__pMenu(NULL)
, m__pContentMenu(0)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m_pData(NULL)
, m__sfBackGround(NULL)
, m__s9BackGround(NULL)
, m__ltContentText(NULL)
, m__ltTitle(NULL)
, m__pEditBox(NULL)
{
    isEditBox=false; 
}
 
PopupLayer::~PopupLayer(){
    CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__pContentMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
    CC_SAFE_RELEASE(m__s9BackGround);
	//CC_SAFE_RELEASE(m__pEditBox);
}
 
bool PopupLayer::init(){
    if(!Layer::init()){
        return false;
    }
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setContentSize(Size::ZERO);
    
    // 初始化需要的 Menu
    Menu* menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
    setMenuButton(menu);

	Menu* content_menu = Menu::create();
	content_menu->setPosition(Vec2::ZERO);
    setContentButton(content_menu);
         
    setTouchEnabled(true);
 
    return true;
}
 
bool PopupLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    //log("PopupLayer touch");
    return true;
}
 
PopupLayer* PopupLayer::create(const char *backgroundImage){
    
	PopupLayer* ml = PopupLayer::create();
    ml->setSpriteBackGround(Sprite::create(backgroundImage));
    ml->setSprite9BackGround(Scale9Sprite::create(backgroundImage));

    return ml;
}
 
void PopupLayer::setTitle(const char *title, const Color3B & color, int fontsize ){
    Label* ltfTitle = Label::create(title, Default_Title_Font, fontsize);
	ltfTitle->setColor(color);
    setLabelTitle(ltfTitle);
}
 
void PopupLayer::setContentText(const char *text, const Color3B & color, int fontsize, int padding, int paddingTop){
    Label* ltf = Label::create(text, Default_Font, fontsize);
	ltf->setColor(color);
    setLabelContentText(ltf);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
}
 
void PopupLayer::setCallbackFunc(cocos2d::Object *target, SEL_CallFuncND callfun,void *data){
    m_callbackListener = target;
    m_callback = callfun; 
	m_pData = data;
}
 

bool PopupLayer::addEditBox(const char* normalImage, const char* pre_text ,const char* text, const Color3B & color,int fontsize ,const Size rectBox, int nMaxChars)
{
	Size winSize = Director::sharedDirector()->getWinSize();
    Vec2 pCenter = Vec2(Design_Pos_X, winSize.height / 2);
	
	EditBox* editBox = EditBox::create(rectBox, Scale9Sprite::create(normalImage));
	editBox->setPosition(pCenter);
	editBox->setName("EditBox");
	editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
    editBox->setFontColor(color);
	editBox->setFontSize(fontsize);
	editBox->setPlaceHolder(pre_text);
	editBox->setText(text);
	editBox->setMaxLength(nMaxChars);
	editBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT); 
	editBox->setInputFlag(EditBox::InputFlag::SENSITIVE);
	
	setEditbox(editBox);

	isEditBox=true;
	return true;
}


bool PopupLayer::addContentButton(const char *normalImage, const char *selectedImage, const char *title, const Color3B & color, int tag){
    Size winSize = Director::sharedDirector()->getWinSize();
    Vec2 pCenter = Vec2(winSize.width / 2, winSize.height / 2);
     
    // 创建图片菜单按钮
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopupLayer::buttonCallback));
    menuImage->setTag(tag);
    menuImage->setPosition(pCenter);
     
    // 添加文字说明并设置位置
    Size imenu = menuImage->getContentSize();
    Label* ttf = Label::create(title, Default_Title_Font, 20);
	ttf->setColor(color);
    ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2));
    menuImage->addChild(ttf);
     
    getContentButton()->addChild(menuImage);
    return true;
}

bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, const Color3B & color, int tag){
    Size winSize = Director::sharedDirector()->getWinSize();
    Vec2 pCenter = Vec2(winSize.width / 2, winSize.height / 2);
     
    // 创建图片菜单按钮
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopupLayer::buttonCallback));
    menuImage->setTag(tag);
    menuImage->setPosition(pCenter);
     
    // 添加文字说明并设置位置
    Size imenu = menuImage->getContentSize();
    Label* ttf = Label::create(title, Default_Title_Font, 20);
	ttf->setColor(color);
    ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2));
    menuImage->addChild(ttf);
     
    getMenuButton()->addChild(menuImage);
    return true;
}
 
void PopupLayer::buttonCallback(cocos2d::Object *pSender){
    Node* node = dynamic_cast<Node*>(pSender);
//log("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener){
        (m_callbackListener->*m_callback)(node,m_pData);
    }
    this->removeFromParent();
}
 
void PopupLayer::onEnter(){
    Layer::onEnter();
     
    Size winSize = Director::sharedDirector()->getWinSize();
    Vec2 pCenter = Vec2(winSize.width / 2, winSize.height / 2);
     
    Size contentSize;

    // 设定好参数，在运行时加载
	if (getContentSize().equals(Size::ZERO)) {
        getSpriteBackGround()->setPosition(Vec2(Design_Pos_X, winSize.height / 2));
        this->addChild(getSpriteBackGround(), 0, 0);
        contentSize = getSpriteBackGround()->getTexture()->getContentSize();
    } else {
        Scale9Sprite *background = getSprite9BackGround();
        background->setContentSize(getContentSize());
        background->setPosition(Vec2(Design_Pos_X, winSize.height / 2));
        this->addChild(background, 0, 0);
        contentSize = getContentSize();
    }
     
     
    // 添加按钮，并设置其位置
    this->addChild(getMenuButton());
    float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1);
     
    Vector<Node*> vecArray = getMenuButton()->getChildren();
    Object* pObj = NULL;
    int i = 0;
    for(auto& e : vecArray){
        Node* node = dynamic_cast<Node*>(e);
		auto bPosX = Design_Pos_X - contentSize.width/2+btnWidth*(i+1);
		if(vecArray.size()>1)
		{
			if(i==0)
				bPosX-=contentSize.width/20;
			else if(i==vecArray.size()-1)
				bPosX+=contentSize.width/20;
		}
        node->setPosition(Vec2(bPosX,winSize.height/2-contentSize.height/3));
        i++;
    }

	if(getContentButton())
	{
		this->addChild(getContentButton());
		btnWidth = contentSize.width / (getContentButton()->getChildrenCount() + 1);
     
		vecArray = getContentButton()->getChildren();
		i = 0;
		for(auto& e : vecArray){
			Node* node = dynamic_cast<Node*>(e);
			auto bPosX = Design_Pos_X - contentSize.width/2+btnWidth*(i+1);
			node->setPosition(Vec2(bPosX,winSize.height/2));
			i++;
		}
	}
  /*  CCARRAY_FOREACH(array, pObj){
        CCNode* node = dynamic_cast<ccnode*>(pObj);
        node->setPosition(ccp( winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
        i++;
    }*/
     
     
    // 显示对话框标题
    if (getLabelTitle()){

		if(isEditBox)
		{
			//getLabelTitle()->setPosition(ccpAdd(pCenter, Vec2(-contentSize.width / 4, contentSize.height / 2 - 35.0f)));

			/*auto info_label = LabelTTF::create("30 Left", "", 18);
			info_label->setColor(Color3B(0,0,0));
			info_label->setAnchorPoint(Vec2::ZERO);
			info_label->setPosition(ccpAdd(pCenter, Vec2(contentSize.width / 4, contentSize.height / 2 - 50.0f)));
			this->addChild(info_label);*/

			EditBox* ebln = getEditbox();
			this->addChild(ebln);
		}
		
		getLabelTitle()->setPosition(ccpAdd(pCenter, Vec2(30, contentSize.height / 2 - 35.0f)));
        this->addChild(getLabelTitle());
    }
     
    // 显示文本内容
    if (getLabelContentText()){
        Label* ltf = getLabelContentText();
        ltf->setPosition(Vec2(Design_Pos_X, winSize.height / 2));
        ltf->setDimensions(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop);
        ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
        this->addChild(ltf);
    }
 
    // 弹出效果
   /* Action* popupLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
                                              ScaleTo::create(0.09, 1.05),
                                              ScaleTo::create(0.12, 0.95),
                                              ScaleTo::create(0.12, 1.0), NULL);
    this->runAction(popupLayer);*/
 
}
 
void PopupLayer::onExit(){
     
//log("popup on exit.");
    Layer::onExit();
}