//===========Tweebaa==============
//category.cpp	--Jly 27 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#include "Category.h"

#include "cocostudio/ActionTimeline/CSLoader.h"

bool Category::bLongPressed=false;
Layer *	Category::DragLayer;
Node * Category::_detailsLayer;
Sprite * Category::selSprite;
EventListenerTouchOneByOne * Category::listener;
Point	Category::curPos;
unsigned int Category::sel_ID,Category::nProductListOffset,Category::nDecoListOffset;


Category::Category(Node* rootNode)
{
	//nCurrentFile = 0;
	gd_direction = kGDirectionNo;
	nListCurIndex = -1;
	nFlag=0;
	m_waitingCtrl=NULL;
	nProductListOffset = 0;
	nDecoListOffset = 0;
	filePath = FileUtils::getInstance()->getWritablePath();
	http_connect = new HttpConnection();
	initialize(rootNode);

}

Category::~Category(void)
{
	delete http_connect;
}


void  Category::initialize(Node * rootNode) {

	bLongPressed=false;
	bIsMoved=false;

	string	sCache;
	//nMaxFile=HttpConnection::v_cache_filename.size();
	SetButtonEnabled((Button*)rootNode->getParent()->getParent()->getChildByName("Button_Trends"),false);
	SetButtonEnabled((Button*)rootNode->getParent()->getParent()->getChildByName("Button_Feed"),false);
	SetButtonEnabled((Button*)rootNode->getParent()->getParent()->getChildByName("Button_Shop"),false);
	
	_createLayer = CSLoader::createNode("Collage/Category.csb");
	_createLayer->setPosition(Vec2(Window_Width,0));
	rootNode->getParent()->addChild(_createLayer);
	_createLayer->setTag(MenuItem_AddItem);

	auto actionBy = MoveBy::create(0.3,Vec2(-Category_Width,0));
	_createLayer->runAction(actionBy);

	auto buttonQuit = dynamic_cast<Button*>(Category::_createLayer->getChildByName("Button_Quit"));
	buttonQuit->addTouchEventListener(Category::_createLayer,toucheventselector(Category::CatQuit));

	//product list
	_listProduct = static_cast<ListView*>(_createLayer->getChildByName("ListView_Product"));
	static_cast<cocos2d::ui::ScrollView*>(_listProduct)->addEventListener(CC_CALLBACK_2(Category::ProductViewMoveCallback, this));
	Size _listProductSize = _listProduct->getSize();
	
	_listProduct->setContentSize(Size(_listProductSize.width, _listProductSize.height));

	_listProduct->setTouchEnabled(true);
	_listProduct->setDirection(SCROLLVIEW_DIR_VERTICAL);
	_listProduct->setBounceEnabled(true);
	_listProduct->setClippingEnabled(true);		//hide off by window size
	_listProduct->setItemsMargin(Items_Margin);
	
	
	int nAmount=HttpConnection::v_product_info.size();
	int nRow = nAmount/Items_InaRow+(nAmount%Items_InaRow==0?0:1);
	

	for(int j=0;j<nRow;j++)
	{
		Layout* custom_item = Layout::create();
		custom_item->setSize(Size((Products_Size+Items_Margin)*Items_InaRow,Products_Size));
			
		for(int i=0;i<(nAmount<Items_InaRow?nAmount:Items_InaRow);i++)
		{
			//Button* custom_button = Button::create(HttpConnection::v_cache_filename.at(j*4+i),"");
			auto custom_button = ImageView::create();
			sCache = HttpConnection::v_product_info.at(j*Items_InaRow+i).cache_filename;
			custom_button->loadTexture(sCache);
			custom_button->setName(sCache);
			custom_button->setTag(HttpConnection::v_product_info.at(j*Items_InaRow+i).id);
			custom_button->setScale9Enabled(true);
			custom_button->setTouchEnabled(true);
			//custom_button->setZoomScale(0.5f);
			
			custom_button->setScaleX(Products_Size/custom_button->getContentSize().width);
			custom_button->setScaleY(Products_Size/custom_button->getContentSize().height);
			//custom_button->setSize(Size(Products_Size,Products_Size));
			custom_button->setAnchorPoint(Vec2::ZERO);
       
			custom_button->setPosition(Vec2(5+i*Products_Size+i*Items_Margin,0));
			//custom_button->addClickEventListener(CC_CALLBACK_1(Category::clickBtnCallback, this));
			custom_button->addTouchEventListener(CC_CALLBACK_2(Category::clickImgCallback, this));
			custom_item->addChild(custom_button);
			
		}
		_listProduct->pushBackCustomItem(custom_item);
		nAmount-=Items_InaRow;
	}

	//decoration list
	_listDecoration = static_cast<ListView*>(_createLayer->getChildByName("ListView_Embellish"));
	static_cast<cocos2d::ui::ScrollView*>(_listDecoration)->addEventListener(CC_CALLBACK_2(Category::DecoViewMoveCallback, this));
	Size _listDecoSize = _listDecoration->getSize();
	
	_listDecoration->setContentSize(Size(_listDecoSize.width, _listDecoSize.height));

	_listDecoration->setTouchEnabled(true);
	_listDecoration->setDirection(SCROLLVIEW_DIR_VERTICAL);
	_listDecoration->setBounceEnabled(true);
	_listDecoration->setClippingEnabled(true);		//hide off by window size
	_listDecoration->setItemsMargin(Items_Margin);
	
	
	nAmount=HttpConnection::v_decoration_info.size();	//decoration cache files//////////////////
	nRow = nAmount/Items_InaRow+(nAmount%Items_InaRow==0?0:1);

	for(int j=0;j<nRow;j++)
	{
		Layout* custom_item = Layout::create();
		custom_item->setSize(Size((Products_Size+Items_Margin)*Items_InaRow,Products_Size));
			
		for(int i=0;i<(nAmount<Items_InaRow?nAmount:Items_InaRow);i++)
		{
			auto custom_button = ImageView::create();
			sCache = HttpConnection::v_decoration_info.at(j*Items_InaRow+i).cache_filename;
			
			custom_button->loadTexture(sCache);
			custom_button->setName(sCache);
			custom_button->setTag(HttpConnection::v_decoration_info.at(j*Items_InaRow+i).id+Product_ID_Flag);	//>=Product_ID_Flag for distinguish id from product ID
			custom_button->setScale9Enabled(true);
			custom_button->setTouchEnabled(true);
			
			custom_button->setScaleX(Products_Size/custom_button->getContentSize().width);
			custom_button->setScaleY(Products_Size/custom_button->getContentSize().height);
			custom_button->setAnchorPoint(Vec2::ZERO);
       
			custom_button->setPosition(Vec2(5+i*Products_Size+i*Items_Margin,0));
			custom_button->addTouchEventListener(CC_CALLBACK_2(Category::clickImgCallback, this));
			custom_item->addChild(custom_button);
			
		}
		_listDecoration->pushBackCustomItem(custom_item);
		nAmount-=Items_InaRow;
	}

	DragLayer = Layer::create();
	rootNode->getParent()->addChild(DragLayer);

	//响应拖动事件
	listener = EventListenerTouchOneByOne::create();
	this->setTouchEnabled(true);
	
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Category::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Category::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Category::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, DragLayer);

	//this->scheduleUpdate();
	//scheduleUpdateWithPriority(-100);
	//Director::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Category::Update), this, 1, false);
}


void  Category::clickImgCallback(cocos2d::Ref *pSender, Widget::TouchEventType event) {
    /*if (event != Widget::TouchEventType::ENDED) {
        return;
    }*/
    auto img = static_cast<ImageView *>(pSender);

//log("click img Name = %s", img->getName().c_str());
	auto sprite = Sprite::create(img->getName());
	Size st = sprite->getContentSize();

	auto Root_Window = static_cast<Node*>(pSender)->getParent()->getParent()->getParent()->getParent()->getParent()->getParent();
	Rect rectDesign = DesignLayer::designArea->getBoundingBox();

	switch (event)  
	{ 
		case Widget::TouchEventType::CANCELED:
			bLongPressed=false;
			sprite_info._position=Vec2::ZERO;
			static_cast<Node*>(pSender)->unschedule(schedule_selector(Category::longPress));
			if(selSprite) selSprite=NULL;
			break;
		case Widget::TouchEventType::BEGAN:
			sel_ID=img->getTag();	//get this product's location in vector list

			sprite_info._position=Vec2::ZERO;
			static_cast<Node*>(pSender)->schedule(schedule_selector(Category::longPress),0.2f);

			if(!selSprite)
			{
				selSprite=sprite;
				selSprite->setScale(.7);
				selSprite->setPosition(Vec2(Window_Width+PIC_MATRIX/2,Window_Heigh+PIC_MATRIX/2));
				DragLayer->addChild(selSprite);
			}
			
			break;
		case Widget::TouchEventType::MOVED:

			static_cast<Node*>(pSender)->unschedule(schedule_selector(Category::longPress));
			if (!bLongPressed)
			{
				break;
			}

			bIsMoved=true;

			break;

		case Widget::TouchEventType::ENDED:
			
			static_cast<Node*>(pSender)->unschedule(schedule_selector(Category::longPress));
			//listener->setSwallowTouches(false);
			
			if(bLongPressed)
			{
				if(bIsMoved)
				{
					bIsMoved=false;
				}
				DragLayer->removeAllChildren();
				selSprite=NULL;
				bLongPressed=false;
				break;
			}

			//set postion according to draglayer
//log("x=%f,y=%f,offset= %f,%f",sprite_info._position.x,sprite_info._position.y,rectDesign.origin.x,rectDesign.origin.y);
			if(sprite_info._position==Vec2::ZERO)
				sprite_info._position=Vec2(PIC_MATRIX/2,PIC_MATRIX/2);	
			else if(sprite_info._position.x>Window_Width-Category_Width || sprite_info._position.x<rectDesign.origin.x || sprite_info._position.y>rectDesign.origin.y+PIC_MATRIX || sprite_info._position.y<rectDesign.origin.y)
			{
				sprite_info._position=Vec2::ZERO;
				break;
			}
			else
			{
				sprite_info._position.x-=rectDesign.origin.x;
				sprite_info._position.y-=rectDesign.origin.y;
			}

			if(!DesignLayer::sp_info_vec.empty())	sprite_info.nZorder=DesignLayer::sp_info_vec.back().nZorder+1;
			else sprite_info.nZorder=0;
			DesignLayer::sp_vec.pushBack(sprite);
			
			if(st.width > PIC_MATRIX)	sprite->setScale(PIC_MATRIX/st.width);
			if(st.height > PIC_MATRIX)	sprite->setScale(PIC_MATRIX/st.height);
			
			sprite->setPosition(sprite_info._position);
			Root_Window->getChildByTag(Design_Tag)->addChild(sprite,sprite_info.nZorder);
	
			sprite_info.id=img->getTag();
			DesignLayer::sp_info_vec.push_back(sprite_info);	// save properties for sprite

			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Save"),true);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Publish"),true);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Clear"),true);

	/*auto actionBy = MoveBy::create(0.6,Vec2(Category_Width,0));
	Category_Window->runAction(actionBy);*/
	//static_cast<Node*>(pSender)->getParent()->getParent()->getParent()->getParent()->getParent()->getChildByTag(Design_Tag)->addChild(sprite);
	//static_cast<Node*>(pSender)->getParent()->getParent()->getParent()->getParent()->getParent()->removeChildByTag(MenuItem_AddItem,true);
	//Root_Window->removeChildByTag(MenuItem_Create,true);
			break;
		default:  
			break;
	}

	
}

void Category::onTouchMoved(Touch* touch, Event* event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);
    Point oldTouchLocation = touch->getPreviousLocation();
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    Point translation = touchLocation - oldTouchLocation;

	if(translation.y>0)
		gd_direction=kGDirectionUp;
	else if(translation.y<0)
		gd_direction=kGDirectionDown;

	if(!selSprite || !bLongPressed) return;

	if(_detailsLayer)
	{
		_detailsLayer->removeFromParent();
		_detailsLayer=nullptr;
	}

	listener->setSwallowTouches(true);
	
	Point oldPos = selSprite->getPosition();
    
	selSprite->setPosition(touchLocation);

}

bool Category::onTouchBegan(Touch* touch, Event* event)
{
	nListCurIndex = -1;
	
	curPos=this->convertTouchToNodeSpace(touch);
	return true;
}
void Category::onTouchEnded(Touch* touch, Event* event)
{
	if (!selSprite) return;
	//DragLayer->removeAllChildren();
	listener->setSwallowTouches(false);
	if(bLongPressed) 
	{
		sprite_info._position=selSprite->getPosition();
		if(_detailsLayer)
		{
			_detailsLayer->removeFromParent();
			_detailsLayer=nullptr;
		}
	}
	selSprite->removeFromParent();
	selSprite=NULL;
	bLongPressed=false;

}

void Category::longPress(float dt)
{
	bLongPressed=true;
	this->unschedule(schedule_selector(Category::longPress));
	listener->setSwallowTouches(true);

	if(selSprite)
	{
		if(sel_ID>=Product_ID_Flag)	
			selSprite->setPosition(curPos);
		else
		{
			_detailsLayer = CSLoader::createNode("Collage/Product_Details.csb");
			_detailsLayer->setPosition(Vec2(Window_Width-Category_Width-Detail_Width+10,25));
			DragLayer->addChild(_detailsLayer);
			_detailsLayer->setTag(P_Detail_Tag);

			auto product_img = Sprite::createWithTexture(selSprite->getTexture());
			product_img->setScaleX(Detail_MATRIX/product_img->getContentSize().width);
			product_img->setScaleY(Detail_MATRIX/product_img->getContentSize().height);
			product_img->setAnchorPoint(Vec2::ZERO);
			product_img->setPosition(Vec2(8,230));
			_detailsLayer->addChild(product_img);

			//products info
			//HttpConnection::v_product_info.at(sel_ID)._filename;
			int i;
			for(i=0;i<HttpConnection::v_product_info.size();i++)
			{
				if(HttpConnection::v_product_info.at(i).id==sel_ID)
					break;
			}

			auto *label_name = Label::createWithTTF(HttpConnection::v_product_info.at(i)._objecttitle.c_str(), "fonts/Marker Felt.ttf", 24);
			label_name->setColor(Color3B(0, 0, 0));
			label_name->setAnchorPoint(Vec2(0,1));
			label_name->setPosition(10,230);
			label_name->setDimensions(Detail_Width-12,80);
			//label_name->setAlignment(TextHAlignment::LEFT);
	
			_detailsLayer->addChild(label_name);

			auto *label_price = Label::createWithTTF(HttpConnection::v_product_info.at(i)._object4th.c_str(), "fonts/Marker Felt.ttf", 26);
			label_price->setColor(Color3B(255, 0, 0));
			label_price->setAnchorPoint(Vec2(0,1));
			label_price->setPosition(10,170);
			_detailsLayer->addChild(label_price);

			auto *label_description = Label::createWithTTF(HttpConnection::v_product_info.at(i)._object5th.c_str(), "fonts/Marker Felt.ttf", 18);
			label_description->setColor(Color3B(0, 0, 0));
			label_description->setAnchorPoint(Vec2(0,1));
			label_description->setPosition(10,130);
			label_description->setDimensions(Detail_Width-12,100);
			_detailsLayer->addChild(label_description);
		}

	}
	
}

void Category::CatQuit(Node* object,TouchEventType type)
{
	Widget * menunode;
	switch (type)  
	{  
		case TouchEventType::TOUCH_EVENT_ENDED:
			static_cast<Node*>(object)->getParent()->getParent()->removeChildByTag(MenuItem_AddItem,true);
			menunode = Helper::seekWidgetByTag(static_cast<Widget *>(DesignLayer::designArea->getParent()),L_Menu_Tag);
			SetButtonEnabled((Button*)menunode->getChildByName("Button_Trends"),true);
			SetButtonEnabled((Button*)menunode->getChildByName("Button_Feed"),true);
			SetButtonEnabled((Button*)menunode->getChildByName("Button_Shop"),true);
			
			break;
			
		default:  
			break;  
    }
}

// deletet&add a row for product category listview 
void Category::ProductViewMoveCallback(cocos2d::Ref *pSender, cocos2d::ui::ScrollView::EventType eventType)  
{  
	string fileName;
	
	fileName = filePath + "Product_List.xml";

	Vector<Widget*> items;

	//do something Items_InaRow
	switch (eventType) {
        case ui::ScrollView::EventType::SCROLLING:
			//滚动时根据index判断与边界距离小于CriticalRows就开始向手势方向增加按钮行，增加行的同时改变index
			items = _listProduct->getItems();
			if(nListCurIndex<0)
				nListCurIndex = _listProduct->getCurSelectedIndex();

			if(gd_direction==kGDirectionUp)
			{
				if(items.size()-nListCurIndex<CriticalRows && !nFlag)
					AffixItemRow(_listProduct,HttpConnection::v_product_info,fileName,END);
			}
			else if(gd_direction==kGDirectionDown)
			{
				if(nListCurIndex<CriticalRows && !nFlag)
					AffixItemRow(_listProduct,HttpConnection::v_product_info,fileName,HEAD);
			}
log("scrolling at:%ld -> ",nListCurIndex);
			 //log("total are :%d",items.size());
log("direction:%d",gd_direction);
            break;

		case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
//log("scrolling bottom[product:%d]",HttpConnection::v_product_info.back().id);
			
			AffixItemRow(_listProduct,HttpConnection::v_product_info,fileName,END);
            break;

        case ui::ScrollView::EventType::SCROLL_TO_TOP:
//log("scrolling TOP[product:%d]",HttpConnection::v_product_info.begin()->id);
			AffixItemRow(_listProduct,HttpConnection::v_product_info,fileName,HEAD);
            break;

        default:
            break;
    }
	
}

// deletet&add a row for decoration category listview 
void Category::DecoViewMoveCallback(cocos2d::Ref *pSender, cocos2d::ui::ScrollView::EventType eventType)  
{  
	string fileName;
	
	fileName = filePath + "Decoration_List.xml";

	//do something Items_InaRow
	long in;
	switch (eventType) {
        case ui::ScrollView::EventType::SCROLLING:
			//y = static_cast<ScrollView*>(pSender)->getTouchMovePosition();
			//in = _listDecoration->getCurSelectedIndex();
			//log("scrolling at:%ld",in);
            break;
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
//log("scrolling bottom[decoration:%d]",HttpConnection::v_decoration_info.back().id);
			AffixItemRow(_listDecoration,HttpConnection::v_decoration_info,fileName,END);
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
//log("scrolling TOP[decoration:%d]",HttpConnection::v_decoration_info.begin()->id);
			AffixItemRow(_listDecoration,HttpConnection::v_decoration_info,fileName,HEAD);
            break;
        default:
            break;
    }
}

void 	Category::UpdateIcon(vector<sctPicList>* p_v_info,string fileName,AppendFlag flag)
{
	CollageTemplateList *pCTL;	//product list from xml
	string filename;
	sctPicList	fileInfo;
	char filenamebuffer[128], sUrl[128];
	int nOffset;

	/*#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
    JavaVM *vm;   
    JNIEnv *env;   
    vm = JniHelper::getJavaVM();   
    
    JavaVMAttachArgs thread_args;   
    
    thread_args.name = "Resource Load";   
    thread_args.version = JNI_VERSION_1_4;   
    thread_args.group = NULL;   
    
    vm->AttachCurrentThread(&env, &thread_args);  
	#endif  */
	
	/*if(!updateMutex.try_lock())
		return;*/
	updateMutex.lock();

	if(nFlag)		//listview没有更新完
	{
		updateMutex.unlock();
		return;
	}
	
	nFlag=1;			//waiting spin start

	pCTL=CollageTemplateList::ParseXMLWithfile(fileName.c_str());
	if(!pCTL) 
	{
		log("%s file format error",fileName.c_str());
		exit(0);
	}

	if(flag==END)
	{
		nOffset=p_v_info->back().id+1;
		if(nOffset>=pCTL->vTemplate.size())
		{
			updateMutex.unlock();
			nFlag=2;
			return;
		}
	}
	else
	{
		nOffset=p_v_info->begin()->id-1;
		if(nOffset<0) 
		{
			updateMutex.unlock();
			nFlag=2;
			return;
		}
	}
	
	char cF = *(strrchr(p_v_info->back().cache_filename.c_str(),'/')+2);
	
	int nLastItemCount=p_v_info->size();
	
	for(int i=0;i<Items_InaRow;i++)
	{
		//load new cache file to v_info
		int nLastOffset=nOffset;	//save current offset

		while(nOffset<(pCTL->vTemplate.size()) && nOffset >=0)
		{
			strcpy(filenamebuffer,p_v_info->back().cache_filename.c_str());
			if(cF=='P')
				std::sprintf(strrchr(filenamebuffer,'/')+4,"%s",strrchr(pCTL->vTemplate.at(nOffset).strListImage,'/')+1); //$P
			else
				std::sprintf(strrchr(filenamebuffer,'/')+4,"%s",pCTL->vTemplate.at(nOffset).strListImage); //$E_
			
			ifstream ifs(filenamebuffer);
			if(!ifs)
			{
				
				if(cF=='P')
					strcpy(sUrl,Product_URL);
				else if(cF=='E')
					strcpy(sUrl,Decoration_URL);
				else
				{
					log("%s  format error",filenamebuffer);
					nFlag=2;		//waiting spin off
					updateMutex.unlock();
					exit(0);
				}
				std::strcat(sUrl,pCTL->vTemplate.at(nOffset).strListImage);
				if(!http_connect->CurlReq(sUrl,filenamebuffer))
				{
					//download failed 
					flag==END?nOffset++:nOffset--;
					log("%s download error",sUrl);
					continue;
				}
				
			}
			
			fileInfo.cache_filename=filenamebuffer;
			std::sprintf(fileInfo.uid,"%c_%s",cF,pCTL->vTemplate.at(nOffset).uid);
			fileInfo._filename=pCTL->vTemplate.at(nOffset).strListImage;
			fileInfo._objecttitle=pCTL->vTemplate.at(nOffset).sTitle;
			fileInfo._object4th=pCTL->vTemplate.at(nOffset).sFourthEle;		//price
			fileInfo._object5th=pCTL->vTemplate.at(nOffset).sFifthEle;		//description

			fileInfo.id=pCTL->vTemplate.at(nOffset).id;
			if(flag==END)
			{
				p_v_info->push_back(fileInfo);
				v_temp_info.push_back(fileInfo);	// for listview push_item
				nOffset++;
			}
			else
			{
				p_v_info->insert(p_v_info->begin(),fileInfo);
				v_temp_info.insert(v_temp_info.begin(),fileInfo);
				nOffset--;
			}
			
			break;
		}

		if(nLastItemCount==p_v_info->size())
		{
			nFlag=2;		//waiting spin off
			updateMutex.unlock();
			return;
		}
		if(nOffset==nLastOffset)	break;		//no item loaded
			
	}
	delete pCTL;

	//items were added
	if(flag==END)
		nFlag=4;				
	else if(flag==HEAD)
		nFlag=3;				

	updateMutex.unlock();

	/*#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
    vm->DetachCurrentThread();  
	#endif*/
	
	return;
}



void 	Category::AffixItemRow(ListView* list_view,vector<sctPicList>& v_info,string fileName,AppendFlag flag)
{
	
	if(v_info.size()<=0)	return;

	/*auto viewSize = Director::getInstance()->getVisibleSize();
	gif = InstantGif::create("loading_c.gif");
	gif->setPosition(350,300);
	DragLayer->addChild(gif);
	auto shade = Sprite::create("Collage/shade2.png");
	auto custom_item_size = custom_item->getSize();
	
	ProgressTimer *ct= ProgressTimer::create(shade);
	ct->setScaleX((Products_Size+Items_Margin)*Items_InaRow/shade->getContentSize().width);
	ct->setScaleY(Products_Size/shade->getContentSize().height);
	ct->setAnchorPoint(Vec2::ZERO);
	ct->setPosition(Vec2(Window_Width-Category_Width+Items_Margin,300));
	ct->setTag(10);
	DragLayer->addChild(ct);
	ct->setPercentage(0);
	ct->setType(kCCProgressTimerTypeBar);
	ct->setMidpoint(Vec2(0,0));
	ct->setBarChangeRate(Vec2(1,0));*/

	
	std::thread t1(&Category::UpdateIcon,this,&v_info,fileName,flag);
	t1.detach();
	Director::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this, .1f, false);

}

void 	Category::UpdateListview(float dt)
{
	if(nFlag==1)
	{
		listener->setSwallowTouches(true);
		if (m_waitingCtrl == NULL)
		{
			m_waitingCtrl =  TTWaitting::createWithBgImage("Collage/spinner-2.png");
        
			m_waitingCtrl->setPosition(Vec2(Window_Width/2,Window_Heigh/2));
        
        
			DragLayer->addChild(m_waitingCtrl, 1);
		}
		else
		{
			m_waitingCtrl->setVisible(true);
		}
    
		m_waitingCtrl->startAnimation();
	}
	else if(nFlag==2)
	{
		nFlag=0;
		Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this);
		//停止 动画，隐藏提示
		if (m_waitingCtrl)
		{
			m_waitingCtrl->stopAnimation();
			m_waitingCtrl->setVisible(false);
		}
		listener->setSwallowTouches(false);
	}
	else if (nFlag==3 || nFlag==4)
	{
		Layout* custom_item;
		Vector<Widget*> items;
		ListView* list_view;
		AppendFlag flag;
		vector<sctPicList>* v_info;

		if(nFlag==3)
			flag=HEAD;
		else
			flag=END;

		custom_item = Layout::create();
		custom_item->setSize(Size((Products_Size+Items_Margin)*Items_InaRow,Products_Size));
		
		char cF = *(strrchr(v_temp_info.back().cache_filename.c_str(),'/')+2);	//product? decoration?
		if(cF=='P')
		{
			list_view=_listProduct;
			v_info = &HttpConnection::v_product_info;
		}
		else
		{
			list_view=_listDecoration;
			v_info = &HttpConnection::v_decoration_info;
		}

		int	nTimes = v_temp_info.size();
		nTimes = nTimes<Items_InaRow?nTimes:Items_InaRow;
		for(int i=0;i<nTimes;i++)
		{
			auto custom_button = ImageView::create();
			custom_button->loadTexture(v_temp_info.begin()->cache_filename);
			custom_button->setName(v_temp_info.begin()->cache_filename);
			cF=='P'?custom_button->setTag(v_temp_info.begin()->id):custom_button->setTag(v_temp_info.begin()->id+Product_ID_Flag);
			custom_button->setScale9Enabled(true);
			custom_button->setTouchEnabled(true);
			
			custom_button->setScaleX(Products_Size/custom_button->getContentSize().width);
			custom_button->setScaleY(Products_Size/custom_button->getContentSize().height);
			custom_button->setAnchorPoint(Vec2::ZERO);
       
			custom_button->setPosition(Vec2(5+i*Products_Size+i*Items_Margin,0));
			custom_button->addTouchEventListener(CC_CALLBACK_2(Category::clickImgCallback, this));
			custom_item->addChild(custom_button);
			v_temp_info.erase(v_temp_info.begin());
		}

		flag==END?list_view->pushBackCustomItem(custom_item):list_view->insertCustomItem(custom_item,0);
		items = _listProduct->getItems();
		if(items.size()>Items_MaxRow)
		{
			if(flag==END)
			{
				list_view->removeItem(0);
				nListCurIndex--;
				for(int i=0;i<Items_InaRow;i++)
					v_info->erase(v_info->begin());
			}
			else
			{
				nTimes = items.back()->getChildrenCount();
				list_view->removeLastItem();
				nListCurIndex++;
				for(int i=0;i<nTimes;i++)
					v_info->pop_back();
			}
		}

		nFlag=0;
		Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this);
		//停止 动画，隐藏提示
		if (m_waitingCtrl)
		{
			m_waitingCtrl->stopAnimation();
			m_waitingCtrl->setVisible(false);
		}
		listener->setSwallowTouches(false);
	}
	
}