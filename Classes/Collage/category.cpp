//===========Tweebaa==============
//category.cpp	--Jly 27 2015	
//Version 1.2 by Raymond Zhuang at Toronto
// last modified at Oct 2015
#include "Category.h"

#include "cocostudio/ActionTimeline/CSLoader.h"

bool Category::bLongPressed=false;
bool Category::bMainCated=false;
bool Category::bProductCated=false;
Layer *	Category::DragLayer;

Node * Category::_detailsLayer;
Node* Category::MainCatLayer;
Node* Category::_createLayer;
Sprite * Category::selSprite;
EventListenerTouchOneByOne * Category::listener;
Vec2	Category::curPos,Category::StartPos;
unsigned int Category::sel_ID,Category::nProductListOffset,Category::nDecoListOffset;


Category::Category(Node* rootNode)
{
	//nCurrentFile = 0;
	StartPos=Vec2::ZERO;
	gd_direction = kGDirectionNo;
	nListCurIndex = -1;
	nFlag=0;
	bLoadedFlag=false;
	bIsNewItemDone=true;
	m_waitingCtrl=NULL;
	label_ratio=nullptr;
	label_Search = nullptr ;
	nProductListOffset = 0;
	nDecoListOffset = 0;
	nDisplayedItem = 0;
	nErrorFile = 0;
	http_connect = new HttpConnection();
	initialize(rootNode);

}

Category::~Category(void)
{
	//DragLayer->onExit();
	//DragLayer->release();
	Layer::onExit();
	//_eventDispatcher->removeAllEventListeners();
	listener=nullptr;
	DragLayer->removeFromParent();
	DragLayer=nullptr;
	MainCatLayer->removeFromParent();
	_createLayer->removeFromParent();
	//MainCatLayer->onExit();
	//MainCatLayer->release();
	MainCatLayer=nullptr;
	_createLayer=nullptr;
	CC_SAFE_DELETE(http_connect);
	
}


void  Category::initialize(Node * rootNode) {

	bLongPressed=false;
	bIsMoved=false;
	bProductCated=false;
	bMainCated=false;
	_fileName="";

	//nMaxFile=HttpConnection::v_cache_filename.size();

	if(HttpConnection::v_design_time_info.size()>0)
		SetButtonEnabled((Button*)rootNode->getParent()->getParent()->getChildByName("Button_Trends"),false);
	if(HttpConnection::v_design_like_info.size()>0)
		SetButtonEnabled((Button*)rootNode->getParent()->getParent()->getChildByName("Button_Feed"),false);
	SetButtonEnabled((Button*)rootNode->getParent()->getParent()->getChildByName("Button_Shop"),false);
	
	if(!MainCatLayer)
	{
		MainCatLayer = CSLoader::createNode("Collage/Main_CG.csb");
		MainCatLayer->setPosition(Vec2(R_Menu_Width,0));
		rootNode->getParent()->addChild(MainCatLayer);	//base on right menu
		MainCatLayer->setTag(MenuItem_AddItem);

		_scrollDecoration = dynamic_cast<ScrollView*>(MainCatLayer->getChildByName("ScrollView_Decoration"));
		
		Vector<Node*>& Dec_items = _scrollDecoration->getChildren();
		int i=1;
		for(auto item : Dec_items)
		{
			if(i==1)
				dynamic_cast<Widget*>(item)->addTouchEventListener(this,toucheventselector(Category::textCallback));
			else
				dynamic_cast<Widget*>(item)->addTouchEventListener(CC_CALLBACK_2(Category::clickBtnCallback, this));
			i++;
		}
		
		_listMainCat = dynamic_cast<ListView*>(MainCatLayer->getChildByName("ListView_Product"));
		//static_cast<cocos2d::ui::ScrollView*>(_listMainCat)->addEventListener(CC_CALLBACK_2(Category::ProductViewMoveCallback, this));
		//Size _listMainCatSize = _listMainCat->getSize();
		//_listMainCat->setContentSize(Size(_listMainCatSize.width, _listMainCatSize.height));

		//_listMainCat->setTouchEnabled(true);
		//_listMainCat->setDirection(SCROLLVIEW_DIR_VERTICAL);
		//_listMainCat->setBounceEnabled(true);
		_listMainCat->setClippingEnabled(true);		//hide off by window size
		_listMainCat->setItemsMargin(Items_Margin/5);

		Vector<Widget*>& items = _listMainCat->getItems();
		for(auto item : items)
		{
			item->addTouchEventListener(CC_CALLBACK_2(Category::clickBtnCallback, this));
		}

		auto buttonSearch = dynamic_cast<Button*>(MainCatLayer->getChildByName("Search_Item"));
		buttonSearch->addTouchEventListener(CC_CALLBACK_2(Category::clickBtnCallback, this));
		
		editBox = EditBox::create(Size(280,50), Scale9Sprite::create("Collage/search_text.png"));
		editBox->setPosition(Vec2(220,Window_Heigh-80));
		editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
		editBox->setFontColor(Color3B::YELLOW);
		editBox->setFontSize(12);
		editBox->setPlaceholderFontSize(12);
		editBox->setPlaceHolder("Search Item");
		editBox->setMaxLength(15);
		//editBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
		editBox->setReturnType(EditBox::KeyboardReturnType::SEARCH);
		editBox->setInputFlag(EditBox::InputFlag::SENSITIVE);
		editBox->setDelegate(this);
		editBox->setName("Search_Box");
		MainCatLayer->addChild(editBox);

		DragLayer = Layer::create();
		//rootNode->getParent()->addChild(DragLayer);
		rootNode->getParent()->getParent()->addChild(DragLayer);

		//响应拖动事件
		listener = EventListenerTouchOneByOne::create();
		this->setTouchEnabled(true);
	
		//listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(Category::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(Category::onTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(Category::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, DragLayer);
	}

	auto actionBy = MoveBy::create(0.3,Vec2(-Category_Width,0));
	MainCatLayer->runAction(actionBy);
	bMainCated=true;
	
	if(!_createLayer)
	{
		_createLayer = CSLoader::createNode("Collage/Category.csb");
		_createLayer->setPosition(Vec2(R_Menu_Width,0));
		rootNode->getParent()->addChild(_createLayer);
		_createLayer->setTag(MenuItem_AddItem);

		auto buttonBack = dynamic_cast<Button*>(_createLayer->getChildByName("Button_Back"));
		buttonBack->addTouchEventListener(this,toucheventselector(Category::CatBack));

		_listviewPic = static_cast<ListView*>(_createLayer->getChildByName("ListView_Product"));
		static_cast<cocos2d::ui::ScrollView*>(_listviewPic)->addEventListener(CC_CALLBACK_2(Category::ProductViewMoveCallback, this));
		Size _listProductSize = _listviewPic->getSize();
		_listviewPic->setContentSize(Size(_listProductSize.width, _listProductSize.height));
		_listviewPic->setTouchEnabled(true);
		_listviewPic->setDirection(SCROLLVIEW_DIR_VERTICAL);
		_listviewPic->setBounceEnabled(true);
		_listviewPic->setClippingEnabled(true);		//hide off by window size
		_listviewPic->setItemsMargin(Items_Margin);
	}


	/*auto buttonQuit = dynamic_cast<Button*>(Category::_createLayer->getChildByName("Button_Quit"));
	buttonQuit->addTouchEventListener(Category::_createLayer,toucheventselector(Category::CatQuit));*/

	/*DragLayer = Layer::create();
	//rootNode->getParent()->addChild(DragLayer);
	rootNode->getParent()->getParent()->addChild(DragLayer);

	//响应拖动事件
	listener = EventListenerTouchOneByOne::create();
	this->setTouchEnabled(true);
	
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Category::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Category::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Category::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, DragLayer);*/
	
}

bool	Category::LoadCatImg(string fileName)
{
	char filenamebuffer[80], sUrl[256];
	sctPicList	fileInfo;
	
	if(!updateMutex.try_lock())
		return false;
	
	//updateMutex.lock();

	if(nFlag)		//listview没有更新完
	{
		updateMutex.unlock();
		return false;
	}

	nFlag=1;

//log("loading %s starting",fileName.c_str());
//Sleep(3000);

	//Load pics
	pCTL=CollageTemplateList::ParseXMLWithfile(fileName.c_str());
	if(!pCTL) 
	{
		nFlag=2;
		updateMutex.unlock();
		return false;
	}
	Number_Resources=pCTL->vTemplate.size()>MAX_CacheFile?MAX_CacheFile:pCTL->vTemplate.size();

	HttpConnection::numberOfLoadSprites=0;
	HttpConnection::v_product_info.clear();

	int i=0;
	for(auto vT: pCTL->vTemplate )
	{
		i++;
		if(i==MAX_CacheFile+1)	break;				//装载的缓冲文件个数
		
		if(!strcmp(vT.strListImage,"None"))
		{
			i--;
			continue;
		}

		char cF='P';
		/*if(fileName.find("Decoration_List")!=std::string::npos)
			cF='E';
		else if(fileName.find("Articles_List")!=std::string::npos)
			cF='A';
		else
			cF='P';*/

		auto cPos = strrchr(vT.strListImage,'/');
		if(!cPos)
			sprintf(filenamebuffer,"$%c_%s",cF,vT.strListImage);
		else
			sprintf(filenamebuffer,"$%c_%s",cF,cPos+1);

		fileInfo.cache_filename= TEMP_filePath+Cache_Path +filenamebuffer;
		sprintf(fileInfo.uid,"%s",vT.uid);
		fileInfo._filename=vT.strListImage;
		fileInfo.list_filename=fileName;
		fileInfo._objecttitle=vT.sTitle;
		fileInfo._object4th=vT.sFourthEle;		//price
		fileInfo._object5th=vT.sFifthEle;		//description

		fileInfo.id=vT.id;


		if(fileName.find("Decoration_List")!=std::string::npos || fileName.find("Event_List")!=std::string::npos || fileName.find("Background_List")!=std::string::npos)
		{
			strcpy(sUrl,Decoration_URL);
			fileInfo._Cat = kPTypeDecoration;
		}
		else if(fileName.find("Articles_List")!=std::string::npos)
		{
			strcpy(sUrl,Article_URL);
			fileInfo._Cat = PicType::kPTypeArticle;
		}
		else
		{
			strcpy(sUrl,Product_URL);
			fileInfo._Cat = PicType::kPTypeProduct;
		}
		//判断文件是否存在
		ifstream ifs(fileInfo.cache_filename);
		if(!ifs)
		{	
			strcat(sUrl,vT.strListImage);
			//http_connect->HttpReqToCache(sUrl,vT.strListImage,fileInfo.cache_filename.c_str());
			if(http_connect->CurlReq(sUrl,fileInfo.cache_filename.c_str())!=CURLE_OK)
			{
				//download failed 
				//HttpConnection::numberOfLoadSprites++;
				nErrorFile++;
				i--;
				m_nItemNum--;
				log("%s download error from %d",sUrl,m_nItemNum);
				continue;
			}
			
		}
		else
		{
			// verify file format
			streampos pos = ifs.tellg();  // save current position
			ifs.seekg(0, ios::end);
			if(!ifs.tellg())
			{
				ifs.close();
				nErrorFile++;
				i--;
				m_nItemNum--;
				auto r = std::remove(fileInfo.cache_filename.c_str());
				log("%s length error(del: %d)",fileInfo.cache_filename.c_str(),errno);
				continue;
			}
			ifs.seekg(pos);  // restore saved position
		}
		ifs.close();
		HttpConnection::v_product_info.push_back(fileInfo);	//save cache filename for BrowseLayer
		HttpConnection::numberOfLoadSprites++;

		m_Load_ratio = (float)HttpConnection::numberOfLoadSprites / Number_Resources;
log("%d of %d[%f]",HttpConnection::numberOfLoadSprites,Number_Resources,m_Load_ratio);

		if(HttpConnection::numberOfLoadSprites%Items_InaRow==0)
		{
//log("%d row finished",HttpConnection::numberOfLoadSprites/Items_InaRow);
			nFlag=5;
		}

	}
	delete pCTL;
	nFlag=5;	//if items < Items_InaRow
	bLoadedFlag=true;
	updateMutex.unlock();
	return true;
}

void Category::popupLayer(Node* baseLayer,Vec2 popPos){
    // 定义一个弹出层，传入一张背景图
    PopupLayer* pl = PopupLayer::create("popup_sq.png");
	
    // ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
    pl->setContentSize(Size(500, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle("ENTER TEXT",Color3B(106,72,36));
    //pl->setContentText("Dialogbox context",Color3B(0,0,0), 20, 60, 250);
	pl->addEditBox("text_square.png", "Less than 20 characters","",Color3B(106,72,36),6 ,Size(350,30), 20);
    // 设置回调函数，回调传回一个 Node 以获取 tag 判断点击的按钮
    // 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
    pl->setCallbackFunc(this, callfuncND_selector(Category::popupCallback),NULL);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "DONE",Color3B(255,255,255), 1);
	pl->addButton("button_normal.png", "button_dis.png", "CANCEL",Color3B(255,255,255), 0);
    // 添加到指定层
	baseLayer->addChild(pl);
}

void Category::popupCallback(cocos2d::Node *pNode,void* data)
{
//log("button call back. tag: %d", pNode->getTag());
	if(pNode->getTag())
	{
		auto editText = dynamic_cast<EditBox*>(pNode->getParent()->getParent()->getChildByName("EditBox"));
		string s=editText->getText();
		if(s.size()>0)
		{
//log("%s,%d",s.c_str(),s.size());
			Label* ltf = Label::createWithTTF(s.c_str(), Default_Chinese_Font, Default_FontSize);
			//if(sprite_info._position==Vec2::ZERO)
			sprite_info._position=Vec2(PIC_MATRIX/2,PIC_MATRIX/2);
			if(!DesignLayer::sp_info_vec.empty())	sprite_info.nZorder=DesignLayer::sp_info_vec.back().nZorder+1;
			else sprite_info.nZorder=0;
			sprite_info.id=DesignLayer::sp_info_vec.size()+MAX_PIC+1;
			sprite_info._type = ElementType::kETypeLabel;
			sprite_info._seat = DesignLayer::lb_vec.size();
			sprite_info._filename = "NONE";
			sprite_info._Cat = kPTypeNone;
			strcpy(sprite_info.uid,"label");
			DesignLayer::sp_info_vec.push_back(sprite_info);
			DesignLayer::lb_vec.pushBack(ltf);
			DesignLayer::UndoAdd(OperationType::kOTypeAdd,sprite_info);

			ltf->setPosition(sprite_info._position);
			ltf->setColor(Color3B::BLACK);
			DesignLayer::designArea->addChild(ltf,sprite_info.nZorder);

			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Save"),true);
			//SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Publish"),true);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Clear"),true);
			
		}
		//DesignLayer::sp_vec.pushBack(sprite);
		//DesignLayer::sp_info_vec.push_back(sprite_info);	// save properties for sprite
	}
}

void Category::textCallback(cocos2d::Ref* object,TouchEventType type)
{
	MoveBy	*actionBy;
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			CatQuit(object,TouchEventType::TOUCH_EVENT_ENDED);
			if(DesignLayer::sp_info_vec.size()>=MAX_PIC)
			{
				DesignLayer::MsgBox(Director::getInstance()->getRunningScene(),Vec2::ZERO,"ATTENTION",MSG_WARN_ITEM);
				return;
			}
			popupLayer(DragLayer,Vec2::ZERO);
			break;  
		  
		default:  
			break;  
    }
	
}

void  Category::clickBtnCallback(cocos2d::Ref *pSender, Widget::TouchEventType event) {
   
	auto btn = static_cast<Button *>(pSender);
	
	MoveBy	*actionBy1,*actionBy2;
	ImageView	*Cat_Title;
	std::string	ListFile,PicFile,strSearch;

	char sUrl[256],buffer[128];
	Vector<Node*>& Dec_items = _scrollDecoration->getChildren();

	switch (event)  
	{ 
		case Widget::TouchEventType::CANCELED:
			
			break;
		case Widget::TouchEventType::BEGAN:
						
			break;
		case Widget::TouchEventType::MOVED:

			break;

		case Widget::TouchEventType::ENDED:

			ListFile = TEMP_filePath+btn->getName()+".xml";
			_fileName = ListFile;
			PicFile = "Collage/"+btn->getName()+".png";
//log("click Name = %s,%s", ListFile.c_str(),PicFile.c_str());

			/*actionBy1 = MoveBy::create(0.3,Vec2(Category_Width,0));
			MainCatLayer->runAction(actionBy1);
			bMainCated=false;*/
			
			Cat_Title = dynamic_cast<ImageView *>(_createLayer->getChildByName("Image_Category"));
			
			if(btn->getName()=="Search_Item")		//pictures from search
			{
				auto editText = dynamic_cast<EditBox*>(btn->getParent()->getChildByName("Search_Box"));
				strSearch=editText->getText();
				if(!strSearch.size())
					return;
			}
				
			
			if(btn->getName()!=_createLayer->getName() || (btn->getName()=="Search_Item" && strSearch!=_searchKeyword))
			{
				Cat_Title->loadTexture(PicFile,TextureResType::LOCAL);
				_createLayer->setName(btn->getName());

				if(label_Search)
				{
					label_Search ->removeFromParent();
					label_Search = nullptr;
				}

				if(btn->getName()=="Search_Item")
				{
					_searchKeyword=strSearch;

					label_Search = Label::createWithSystemFont(_searchKeyword, "arial", 50);
					label_Search ->setColor(Color3B::YELLOW);
					label_Search ->setAnchorPoint(Vec2(0,1));
					label_Search ->setPosition(30,60);
					label_Search ->setDimensions(200,50);
					label_Search ->setName("Item_Label");
					Cat_Title->addChild(label_Search);

					std::strcpy(sUrl,Req_URL);
					std::strcat(sUrl,Search_Product);
					std::strcat(sUrl,strSearch.c_str());

					if(http_connect->CurlReq(sUrl,ListFile.c_str())!=CURLE_OK)
					{
						log("search (%s) ",strSearch.c_str());
						MessageBox("Can not download Search Result list","SERVER ERROR");
						return;
					}
				}
				
				_listviewPic->removeAllChildren();
				HttpConnection::v_product_info.clear();
				nDisplayedItem=0;

				pCTL=CollageTemplateList::ParseXMLWithfile(ListFile.c_str());
				auto textNum = dynamic_cast<Text *>(_createLayer->getChildByName("Text_ItemNum"));
				if(pCTL)
				{
					m_nItemNum = pCTL->vTemplate.size();
					char sTempNum[10];
					delete pCTL;
					if(m_nItemNum>0 && m_nItemNum<65535)
						sprintf(sTempNum,"(%d)",m_nItemNum);
					else
					{
						log("item number:[%d] illegal",m_nItemNum);
					}

					string sNum=sTempNum;
					textNum->setString(sNum);
				}
				else
				{
					if(btn->getName()=="Search_Item")
					{
						sprintf(buffer,MSG_WARN_NORESULT,strSearch.c_str());
						string sInfo = buffer;
						DesignLayer::MsgBox(Director::getInstance()->getRunningScene(),Vec2::ZERO,"NO RESULT",sInfo);
						return;
					}
					else
						sprintf(buffer,MSG_WARN_NORESULT,btn->getName().c_str());
					
					textNum->setString("");
					
					
				}
				
				Director::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this, .2f, false);
				std::thread t1(&Category::LoadCatImg,this,ListFile);
				t1.detach();

				auto actionBy2 = MoveBy::create(0.3,Vec2(-Category_Width,0));
				_createLayer->runAction(actionBy2);
				bProductCated=true;
				
			}
			else
			{
				//same click with last time
				if(!bProductCated)
				{
					actionBy2 = MoveBy::create(0.3,Vec2(-Category_Width,0));
					_createLayer->runAction(actionBy2);
					bProductCated=true;
				}
			}
			editBox->setTouchEnabled(false); 
			
			for(auto item : Dec_items)
			{
				dynamic_cast<Widget*>(item)->setTouchEnabled(false);
			}

			break;
		default:  
			break;
	}

}

void Category::ActionCallback(Node *pSender,void*data)
{
	auto sp = dynamic_cast<Sprite *>(pSender);
	sp->removeFromParent();
	sp->setScale(1);
	sp->setPosition(DesignLayer::sp_info_vec.back()._position);
//log("pos: x %f, y %f",sprite_info._position.x,sprite_info._position.y);
	auto rootwindow = (Node *)data;
	rootwindow->getChildByTag(Design_Tag)->addChild(sp,sprite_info.nZorder);
	bIsNewItemDone=true;
}

void Category::ActionCancelCallback(Node *pSender)
{
	//log("action finished");
	auto sp = dynamic_cast<Sprite *>(pSender);
	sp->removeFromParent();
	bIsNewItemDone=true;
}

void  Category::clickImgCallback(cocos2d::Ref *pSender, Widget::TouchEventType event) {
    /*if (event != Widget::TouchEventType::ENDED) {
        return;
    }*/
    auto img = static_cast<ImageView *>(pSender);

//log("click img Name = %s", img->getName().c_str());
	auto sprite = Sprite::create(img->getName());
	//**********************透明测试******************************
	/*unsigned char *pData;
	string filePath = FileUtils::getInstance()->getWritablePath();
	string filename = filePath + "ceshi.png";
	auto pImage = new Image();
	pImage->autorelease();
	//pImage->initWithImageFile(filename);
	pImage->initWithImageFile(img->getName());
	pData = pImage->getData();
	if(pImage->getFileType()==Image::Format::JPG)
	{
		auto OL = pImage->getDataLen();
		unsigned char *pRGBA_Data = (unsigned char *)malloc(OL*4/3);
		long int j=0;
		for(int i=0;i<OL;i++)
		{
			if(i>0 && i%3==0)
			{
				pRGBA_Data[j]=255;
				j++;
			}
			pRGBA_Data[j]=pData[i];
			j++;
		}
		auto pImage1 = new Image();
		pImage1->initWithImageData(pRGBA_Data,j+1);
		pData = pImage1->getData();
	}

	
	auto sz = sizeof(pData);
	int nPixelIndex = 0;
    for (int nCol = 0; nCol < pImage->getHeight(); nCol ++)
    {
		for (int nRow = 0; nRow < pImage->getWidth(); nRow ++)
        {
			//取图片的RGB值.
            int nBeginPos = nPixelIndex;
            unsigned int nRValue = pData[nPixelIndex];
            nPixelIndex ++;
            unsigned int nGValue = pData[nPixelIndex];
            nPixelIndex ++;
            unsigned int nBValue = pData[nPixelIndex];
            nPixelIndex ++;
            unsigned int nAValue = pData[nPixelIndex];
            nPixelIndex ++;
            
            int nAlphaRatio = 0;
                //本代码的核心:取RGB中的最大值赋给nAlphaRatio。如果nAlphaRatio为0,则像素中的alpha通道就为0,否则像素中的 
                //alpha通道值就是nAlphaRatio。这样做是为了在图片中颜色渐变过渡比较大的区域实现平滑的过渡。让最终形成的
                //图片看起来不粗糙.
            nAlphaRatio = nRValue>nGValue?(nRValue>nBValue?nRValue:nBValue):(nGValue>nBValue?nGValue:nBValue);
            if(nAlphaRatio != 255)
            {
                nAValue = nAlphaRatio;
            }
            else
            {
                nAValue= 0;
            }

            pData[nBeginPos] = (unsigned char)nRValue;
            pData[nBeginPos+ 1] = (unsigned char)nGValue;
            pData[nBeginPos + 2] = (unsigned char)nBValue;
            //修改原图的alpha值.
            pData[nBeginPos + 3] = (unsigned char)nAValue;
        }
    }

	Texture2D *pTexture = new CCTexture2D;
    pTexture->autorelease();
    pTexture->initWithImage(pImage);
	auto sprite = Sprite::createWithTexture(pTexture);*/
	//============================================================
	Size st = sprite->getContentSize();

	auto Root_Window = static_cast<Node*>(pSender)->getParent()->getParent()->getParent()->getParent()->getParent()->getParent();
	Rect rectDesign = DesignLayer::designArea->getBoundingBox();

	MoveTo *action1;
	ScaleTo *action2;

	auto func = CCCallFuncND::create(this,callfuncND_selector(Category::ActionCallback),(void *)Root_Window);
	auto funcCancel = CallFuncN::create(this,callfuncN_selector(Category::ActionCancelCallback));

	auto draw = DrawNode::create();

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
				//selSprite->setScale(.7);
				selSprite->setScale(1);
				selSprite->setOpacity(255*0.75);
				selSprite->setAnchorPoint(Vec2::ZERO);
				draw->drawRect(selSprite->getBoundingBox().origin,Vec2(selSprite->getBoundingBox().getMaxX(),selSprite->getBoundingBox().getMaxY()),Color4F::RED);
				selSprite->setAnchorPoint(Vec2(.5,.5));
				draw->setName("BOUNDING");
				selSprite->addChild(draw);
				draw->setVisible(false);
				selSprite->setPosition(Vec2(Window_Width+PIC_MATRIX/2,Window_Heigh+PIC_MATRIX/2));
				DragLayer->addChild(selSprite);
			}
			
			break;
		case Widget::TouchEventType::MOVED:

			//static_cast<Node*>(pSender)->unschedule(schedule_selector(Category::longPress));
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

			if(DesignLayer::sp_info_vec.size()>=MAX_PIC)
			{
				DesignLayer::MsgBox(Director::getInstance()->getRunningScene(),Vec2::ZERO,"ATTENTION",MSG_WARN_ITEM);
				return;
			}
			//set postion according to draglayer
//log("x=%f,y=%f,offset= %f,%f",sprite->getBoundingBox().origin.x,sprite->getBoundingBox().origin.y,rectDesign.origin.x,rectDesign.origin.y);

			if(!bIsNewItemDone)
				return;
			bIsNewItemDone=false;
			if(sprite_info._position==Vec2::ZERO)		//click button
			{
				sprite_info._position=Vec2(PIC_MATRIX/2,PIC_MATRIX/2);
				
			}
			else if(sprite_info._position.x>Window_Width-Category_Width 
				|| sprite_info._position.x+sprite->getBoundingBox().origin.x<rectDesign.origin.x 
				|| sprite_info._position.y+sprite->getBoundingBox().size.height/2>rectDesign.origin.y+PIC_MATRIX 
				|| sprite_info._position.y+sprite->getBoundingBox().origin.y<rectDesign.origin.y)
			{
				//over boundary
				action2 = ScaleTo::create(.3,.1);
				action1 = MoveTo::create(.3,clickPos);
				sprite->setPosition(curPos);
				sprite->setScale(.7);
				DragLayer->addChild(sprite);
				sprite->runAction(Sequence::create(Spawn::create(action1,action2,NULL),funcCancel,NULL));
				sprite_info._position=Vec2::ZERO;
				break;
			}
			else
			{
				//drag into edit area
				sprite_info._position.x-=rectDesign.origin.x;
				sprite_info._position.y-=rectDesign.origin.y;
			}

			if(!DesignLayer::sp_info_vec.empty())	sprite_info.nZorder=DesignLayer::sp_info_vec.back().nZorder+1;
			else sprite_info.nZorder=0;

			sprite_info._seat = DesignLayer::sp_vec.size();
			DesignLayer::sp_vec.pushBack(sprite);
			
			if(st.width > PIC_MATRIX)	sprite->setScale(PIC_MATRIX/st.width);
			if(st.height > PIC_MATRIX)	sprite->setScale(PIC_MATRIX/st.height);
			
			//sprite->setPosition(sprite_info._position);
			sprite->setPosition(clickPos);
			sprite->setScale(.7);
			//Root_Window->getChildByTag(Design_Tag)->addChild(sprite,sprite_info.nZorder);
	
			action2 = ScaleTo::create(.3,1);
			action1 = MoveTo::create(.3,Vec2(sprite_info._position.x+Design_Pos_X-PIC_MATRIX/2,sprite_info._position.y+(Window_Heigh-PIC_MATRIX)/2));
			DragLayer->addChild(sprite);
	
			sprite_info.id=img->getTag();
			for(sctPicList v_product : HttpConnection::v_product_info)
			{
				if(v_product.id==sprite_info.id)
				{
					sprite_info._filename=v_product._filename;
					sprite_info._Cat=v_product._Cat;
					strcpy(sprite_info.uid,v_product.uid);
					break;
				}
			}
			sprite_info._type = ElementType::kETypeSprite;
			DesignLayer::sp_info_vec.push_back(sprite_info);	// save properties for sprite
			DesignLayer::UndoAdd(OperationType::kOTypeAdd,sprite_info);

			sprite->runAction(Sequence::create(Spawn::create(action1,action2,NULL),func,NULL));

			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Save"),true);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Publish"),true);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Clear"),true);

			break;
		default:  
			break;
	}

	
}

void Category::onTouchMoved(Touch* touch, Event* event)
{
	auto touchLocation = this->convertTouchToNodeSpace(touch);
    auto oldTouchLocation = touch->getPreviousLocation();
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    auto translation = touchLocation - oldTouchLocation;
	Rect rectDesign = DesignLayer::designArea->getBoundingBox();

	curPos = touchLocation;

	if(translation.y>0)
		gd_direction=kGDirectionUp;
	else if(translation.y<0)
		gd_direction=kGDirectionDown;

	if(!selSprite || !bLongPressed) return;

	if(_detailsLayer)
	{
		if(StartPos.distance(curPos)<10)	//for sensitive touch screen
			return;
		_detailsLayer->removeFromParent();
		_detailsLayer=nullptr;
	}

	listener->setSwallowTouches(true);
	
	Vec2 oldPos = selSprite->getPosition();
    auto dw = dynamic_cast<DrawNode*>(selSprite->getChildByName("BOUNDING"));
	selSprite->setPosition(touchLocation);
//log("x=%f,y=%f,org=%f",touchLocation.x,touchLocation.y,selSprite->getBoundingBox().origin.x);
	if(touchLocation.x>Window_Width-Category_Width || selSprite->getBoundingBox().origin.x<rectDesign.origin.x 
				|| touchLocation.y+selSprite->getBoundingBox().size.height/2>rectDesign.origin.y+PIC_MATRIX 
				|| selSprite->getBoundingBox().origin.y<rectDesign.origin.y)
	{
		log("out of range");
		dw->setVisible(true);
		//selSprite->setOpacity(255*0.25);
		
	}
	else
	{
		//selSprite->setOpacity(255*0.75);
		dw->setVisible(false);
	}
	
}

bool Category::onTouchBegan(Touch* touch, Event* event)
{
	nListCurIndex = -1;
	
	curPos=this->convertTouchToNodeSpace(touch);
	clickPos=curPos;
	if(StartPos==Vec2::ZERO)
	{
		StartPos=curPos;
	}
	return true;
}
void Category::onTouchEnded(Touch* touch, Event* event)
{
	StartPos=Vec2::ZERO;

	if (!selSprite)
	{
//log("ox=%f,oy=%f",touch->getLocation().x,touch->getLocation().y);
		curPos=this->convertTouchToNodeSpace(touch);
//log("x=%f,y=%f",curPos.x,curPos.y);
		if(((bMainCated && MainCatLayer) || (bProductCated && _createLayer)) && curPos.x<Window_Width-Category_Width )
			
			CatQuit(_listMainCat,TouchEventType::TOUCH_EVENT_ENDED);
		return;
	}
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
	this->unschedule(schedule_selector(Category::longPress));

	if(StartPos.distance(curPos)>10 && dt)	//dt==0 : cancel selected
		return;
	bLongPressed=true;

	listener->setSwallowTouches(true);

	if(selSprite)
	{
		if(sel_ID>=Product_ID_Flag)	
			selSprite->setPosition(curPos);
		else
		{
			_detailsLayer = CSLoader::createNode("Collage/Product_Details.csb");
			_detailsLayer->setPosition(Vec2(Window_Width-Category_Width-Detail_Width,25));
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

			auto *label_name = Label::createWithTTF(HttpConnection::v_product_info.at(i)._objecttitle.c_str(), Default_Font, 24);
			label_name->setColor(Color3B(0, 0, 0));
			label_name->setAnchorPoint(Vec2(0,1));
			label_name->setPosition(10,230);
			label_name->setDimensions(Detail_Width-12,80);
			//label_name->setAlignment(TextHAlignment::LEFT);
	
			_detailsLayer->addChild(label_name);

			auto *label_price = Label::createWithTTF(HttpConnection::v_product_info.at(i)._object4th.c_str(), Default_Font, 26);
			label_price->setColor(Color3B(255, 0, 0));
			label_price->setAnchorPoint(Vec2(0,1));
			label_price->setPosition(10,170);
			_detailsLayer->addChild(label_price);

			auto *label_description = Label::createWithTTF(HttpConnection::v_product_info.at(i)._object5th.c_str(), Default_Font, 18);
			label_description->setColor(Color3B(0, 0, 0));
			label_description->setAnchorPoint(Vec2(0,1));
			label_description->setPosition(10,130);
			label_description->setDimensions(Detail_Width-12,100);
			_detailsLayer->addChild(label_description);
		}

	}
	
}

void Category::CatBack(cocos2d::Ref* object,TouchEventType type)
{
	MoveBy * actionBy1,* actionBy2;
	Vector<Node*>& Dec_items = _scrollDecoration->getChildren();
	switch (type)  
	{  
		case TouchEventType::TOUCH_EVENT_ENDED:
			/*actionBy1 = MoveBy::create(0.3,Vec2(-Category_Width,0));
			MainCatLayer->runAction(actionBy1);
			bMainCated=true;*/

			actionBy2 = MoveBy::create(0.3,Vec2(Category_Width,0));
			_createLayer->runAction(actionBy2);
			bProductCated=false;
			editBox->setTouchEnabled(true); 
			for(auto item : Dec_items)
			{
				dynamic_cast<Widget*>(item)->setTouchEnabled(true);
			}
			break;
			
		default:  
			break;  
    }
}

void Category::CatQuit(cocos2d::Ref* object,TouchEventType type)
{
	Widget * menunode;
	MoveBy * actionBy;
	Vector<Node*>& Dec_items = _scrollDecoration->getChildren();
	switch (type)  
	{  
		case TouchEventType::TOUCH_EVENT_ENDED:
			//static_cast<Node*>(object)->getParent()->getParent()->removeChildByTag(MenuItem_AddItem,true);
	
			//MainCatLayer=nullptr;
			actionBy = MoveBy::create(0.3,Vec2(Category_Width,0));
			if(bProductCated)
			{
				_createLayer->runAction(actionBy);
				bProductCated=false;
				editBox->setTouchEnabled(true); 
				for(auto item : Dec_items)
				{
					dynamic_cast<Widget*>(item)->setTouchEnabled(true);
				}
			}
			else if(bMainCated)
			{
				MainCatLayer->runAction(actionBy);
				bMainCated=false;
			}

			menunode = Helper::seekWidgetByTag(static_cast<Widget *>(DesignLayer::designArea->getParent()),L_Menu_Tag);
			if(HttpConnection::v_design_time_info.size()>0)
				SetButtonEnabled((Button*)menunode->getChildByName("Button_Trends"),true);
			if(HttpConnection::v_design_like_info.size()>0)
				SetButtonEnabled((Button*)menunode->getChildByName("Button_Feed"),true);
			SetButtonEnabled((Button*)menunode->getChildByName("Button_Shop"),true);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Add"),true);
			
			
			break;
			
		default:  
			break;  
    }
}

// deletet&add a row for product category listview 
void Category::ProductViewMoveCallback(cocos2d::Ref *pSender, cocos2d::ui::ScrollView::EventType eventType)  
{  
	Vector<Widget*> items;

	if(_fileName=="" || HttpConnection::v_product_info.size()<=0)
		return;
	//do something Items_InaRow
	switch (eventType) {
        case ui::ScrollView::EventType::SCROLLING:
			//滚动时根据index判断与边界距离小于CriticalRows就开始向手势方向增加按钮行，增加行的同时改变index
			items = _listviewPic->getItems();
			if(nListCurIndex<0)
				nListCurIndex = _listviewPic->getCurSelectedIndex();

			if(gd_direction==kGDirectionUp)
			{
				if(items.size()-nListCurIndex<CriticalRows && !nFlag)
					AffixItemRow(_listviewPic,HttpConnection::v_product_info,_fileName,END);
			}
			else if(gd_direction==kGDirectionDown)
			{
				if(nListCurIndex<CriticalRows && !nFlag)
					AffixItemRow(_listviewPic,HttpConnection::v_product_info,_fileName,HEAD);
			}
log("scrolling at:%ld -> ",nListCurIndex);
			 //log("total are :%d",items.size());
log("direction:%d",gd_direction);
            break;

		case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
//log("scrolling bottom[product:%d]",HttpConnection::v_product_info.back().id);
			
			AffixItemRow(_listviewPic,HttpConnection::v_product_info,_fileName,END);
            break;

        case ui::ScrollView::EventType::SCROLL_TO_TOP:
//log("scrolling TOP[product:%d]",HttpConnection::v_product_info.begin()->id);
			
			AffixItemRow(_listviewPic,HttpConnection::v_product_info,_fileName,HEAD);
            break;

        default:
            break;
    }
	
}

// deletet&add a row for decoration category listview 
/*void Category::DecoViewMoveCallback(cocos2d::Ref *pSender, cocos2d::ui::ScrollView::EventType eventType)  
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
}*/

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
	//don't need to lock multi-thread
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
	
	int nLastItemCount=p_v_info->size();
	
	for(int i=0;i<Items_InaRow;i++)
	{
		//load new cache file to v_info
		int nLastOffset=nOffset;	//save current offset

		
		auto cPos = strrchr(p_v_info->back()._filename.c_str(),'/');		// no '/' in the filename of decoration & article
		if(nOffset<pCTL->vTemplate.size())
			cPos = strrchr(pCTL->vTemplate.at(nOffset).strListImage,'/');

		while(nOffset<(pCTL->vTemplate.size()) && nOffset >=0)
		{

			if(!cPos)
				sprintf(filenamebuffer,"$P_%s",pCTL->vTemplate.at(nOffset).strListImage);
			else
				sprintf(filenamebuffer,"$P_%s",strrchr(pCTL->vTemplate.at(nOffset).strListImage,'/')+1);
			
			fileInfo.cache_filename= TEMP_filePath+Cache_Path +filenamebuffer;
			std::sprintf(fileInfo.uid,"%s",pCTL->vTemplate.at(nOffset).uid);
			fileInfo._filename=pCTL->vTemplate.at(nOffset).strListImage;
			fileInfo.list_filename=fileName;
			fileInfo._objecttitle=pCTL->vTemplate.at(nOffset).sTitle;
			fileInfo._object4th=pCTL->vTemplate.at(nOffset).sFourthEle;		//price
			fileInfo._object5th=pCTL->vTemplate.at(nOffset).sFifthEle;		//description

			fileInfo.id=pCTL->vTemplate.at(nOffset).id;
			
			ifstream ifs(fileInfo.cache_filename);
			if(!ifs)
			{
				
				if(fileName.find("Decoration_List")!=std::string::npos || fileName.find("Event_List")!=std::string::npos || fileName.find("Background_List")!=std::string::npos)
					strcpy(sUrl,Decoration_URL);
				else if(fileName.find("Articles_List")!=std::string::npos)
					strcpy(sUrl,Article_URL);
				else
					strcpy(sUrl,Product_URL);

				std::strcat(sUrl,pCTL->vTemplate.at(nOffset).strListImage);
				if(http_connect->CurlReq(sUrl,fileInfo.cache_filename.c_str())!=CURLE_OK)
				{
					//download failed 
					Number_Resources--;
					m_nItemNum--;
					flag==END?nOffset++:nOffset--;
					log("%s download error",sUrl);
					continue;
				}
				
			}
			m_Load_ratio = (float)i / Items_InaRow;

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

	auto items = _listviewPic->getItems();
	if(items.size()<MAX_CacheFile/Items_InaRow)
		return;
	
	if(!nFlag)
	{
		std::thread t1(&Category::UpdateIcon,this,&v_info,fileName,flag);
		t1.detach();
		Director::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this, .1f, false);
	}

}

void 	Category::UpdateListview(float dt)
{
	
if(nFlag==2)
{
	nFlag=0;
	Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this);
	//停止 动画，隐藏提示
	if (m_waitingCtrl)
	{
		m_waitingCtrl->stopAnimation();
		m_waitingCtrl->setVisible(false);
	}
	if(label_ratio)
	{
		label_ratio->removeFromParent();
		label_ratio=nullptr;
	}
	listener->setSwallowTouches(false);
	return;
}
else if(nFlag==1 || nFlag>2)
{
//cocos2d::log("spin starting... %f",m_Load_ratio);
	listener->setSwallowTouches(true);
	if (m_waitingCtrl == NULL)
	{
		m_waitingCtrl =  TTWaitting::createWithBgImage("Collage/spinner-2.png");
       
		m_waitingCtrl->setPosition(Vec2(Window_Width-Category_Width/2-30,Window_Heigh/2-30));
       
		DragLayer->addChild(m_waitingCtrl, 1);
	}
	else
	{
		m_waitingCtrl->setVisible(true);
	}
    
	m_waitingCtrl->startAnimation();

	char sTempNum[10];
	unsigned int nRatio = (int)(m_Load_ratio*100);
	if(nRatio > 100)
	{
		log("Ratio:[%f] illegal",m_Load_ratio);
		return;
	}
	sprintf(sTempNum,"%d%%",nRatio);
	string sRatio=sTempNum;

	if(!label_ratio)
	{
		label_ratio = Label::createWithSystemFont("", "arial", 16);
		label_ratio->setColor(Color3B::RED);
		label_ratio->setPosition(Vec2(Window_Width-Category_Width/2,Window_Heigh/2));
		DragLayer->addChild(label_ratio);
	}
	label_ratio->setString(sRatio);

	if (nFlag==3 || nFlag==4)
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
		
		list_view=_listviewPic;
		v_info = &HttpConnection::v_product_info;
		/*char cF = *(strrchr(v_temp_info.back().cache_filename.c_str(),'/')+2);	//product? decoration?
		if(cF=='P')
		{
			list_view=_listviewPic;
			v_info = &HttpConnection::v_product_info;
		}
		else
		{
			//list_view=_listDecoration;
			//v_info = &HttpConnection::v_decoration_info;
		}*/

		int	nTimes = v_temp_info.size();
		nTimes = nTimes<Items_InaRow?nTimes:Items_InaRow;
		for(int i=0;i<nTimes;i++)
		{
			auto img_background = ImageView::create("Collage/background.jpg");
			auto custom_button = ImageView::create(v_temp_info.begin()->cache_filename);
			//custom_button->loadTexture(v_temp_info.begin()->cache_filename);
			custom_button->setName(v_temp_info.begin()->cache_filename);
			//cF=='P'?custom_button->setTag(v_temp_info.begin()->id):custom_button->setTag(v_temp_info.begin()->id+Product_ID_Flag);
			custom_button->setTag(v_temp_info.begin()->id);
			custom_button->setScale9Enabled(true);
			custom_button->setTouchEnabled(true);
			
			custom_button->setScaleX((Products_Size-4)/custom_button->getContentSize().width);
			custom_button->setScaleY((Products_Size-4)/custom_button->getContentSize().height);
			custom_button->setAnchorPoint(Vec2::ZERO);
			img_background->setScaleX(Products_Size/img_background->getContentSize().width);
			img_background->setScaleY(Products_Size/img_background->getContentSize().height);
			img_background->setAnchorPoint(Vec2::ZERO);
			img_background->setPosition(Vec2(4+i*Products_Size+i*Items_Margin,0));
			custom_button->setPosition(Vec2(4+i*Products_Size+i*Items_Margin+2,0+2));
			custom_button->addTouchEventListener(CC_CALLBACK_2(Category::clickImgCallback, this));
			custom_item->addChild(img_background);
			custom_item->addChild(custom_button);
			v_temp_info.erase(v_temp_info.begin());
		}

		flag==END?list_view->pushBackCustomItem(custom_item):list_view->insertCustomItem(custom_item,0);
		items = _listviewPic->getItems();
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
				nTimes = items.back()->getChildrenCount()/2;	//img_background+custom_button
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
		if(label_ratio)
		{
			label_ratio->removeFromParent();
			label_ratio=nullptr;
		}
		listener->setSwallowTouches(false);
	}
	else if(nFlag==5)
	{
//log("%d of %d",HttpConnection::numberOfLoadSprites,Number_Resources);
		
		
		if(HttpConnection::numberOfLoadSprites-nDisplayedItem>=Items_InaRow && 
			nDisplayedItem+Items_InaRow<=HttpConnection::v_product_info.size())	//loaded a row
		{
			Layout* custom_item = Layout::create();
			custom_item->setSize(Size((Products_Size+Items_Margin)*Items_InaRow,Products_Size));
			
			for(int i=0;i<Items_InaRow;i++)
			{
				
				auto sCache = HttpConnection::v_product_info.at(nDisplayedItem+i).cache_filename;
				auto img_background = ImageView::create("Collage/background.jpg");
				auto custom_button = ImageView::create(sCache);
				
				//custom_button->loadTexture(sCache);
				custom_button->setName(sCache);
				custom_button->setTag(HttpConnection::v_product_info.at(nDisplayedItem+i).id);
				custom_button->setScale9Enabled(true);
				custom_button->setTouchEnabled(true);
						
				custom_button->setScaleX((Products_Size-4)/custom_button->getContentSize().width);
				custom_button->setScaleY((Products_Size-4)/custom_button->getContentSize().height);
				custom_button->setAnchorPoint(Vec2::ZERO);

				img_background->setScaleX(Products_Size/img_background->getContentSize().width);
				img_background->setScaleY(Products_Size/img_background->getContentSize().height);
				img_background->setAnchorPoint(Vec2::ZERO);
       
				custom_button->setPosition(Vec2(4+i*Products_Size+i*Items_Margin+2,0+2));
				img_background->setPosition(Vec2(4+i*Products_Size+i*Items_Margin,0));
				custom_button->addTouchEventListener(CC_CALLBACK_2(Category::clickImgCallback, this));
				custom_item->addChild(img_background);
				custom_item->addChild(custom_button);
				
			}
			_listviewPic->pushBackCustomItem(custom_item);

//log("loaded 1 row from %d",nDisplayedItem);
			nDisplayedItem+=Items_InaRow;
		}

		if ((HttpConnection::numberOfLoadSprites+nErrorFile) >= Number_Resources 
			&& HttpConnection::v_product_info.size()-nDisplayedItem<Items_InaRow && bLoadedFlag)	//all items are loaded and left items less than a row
		{
//log("%d of %d",nDisplayedItem,HttpConnection::v_product_info.size());

			nFlag=0;
			bLoadedFlag=false;

			if(HttpConnection::v_product_info.size()-nDisplayedItem>0)
			{
				auto last_item = Layout::create();
				last_item->setSize(Size((Products_Size+Items_Margin)*Items_InaRow,Products_Size));
				for(int i=0;i<HttpConnection::v_product_info.size()-nDisplayedItem;i++)
				{
					auto img_background = ImageView::create("Collage/background.jpg");
					auto sCache = HttpConnection::v_product_info.at(nDisplayedItem+i).cache_filename;
					auto custom_button = ImageView::create(sCache);
					custom_button->setName(sCache);
					custom_button->setTag(HttpConnection::v_product_info.at(nDisplayedItem+i).id);
					custom_button->setScale9Enabled(true);
					custom_button->setTouchEnabled(true);
						
					custom_button->setScaleX((Products_Size-4)/custom_button->getContentSize().width);
					custom_button->setScaleY((Products_Size-4)/custom_button->getContentSize().height);
					custom_button->setAnchorPoint(Vec2::ZERO);

					img_background->setScaleX(Products_Size/img_background->getContentSize().width);
					img_background->setScaleY(Products_Size/img_background->getContentSize().height);
					img_background->setAnchorPoint(Vec2::ZERO);
       
					custom_button->setPosition(Vec2(4+i*Products_Size+i*Items_Margin+2,0+2));
					img_background->setPosition(Vec2(4+i*Products_Size+i*Items_Margin,0));
					custom_button->addTouchEventListener(CC_CALLBACK_2(Category::clickImgCallback, this));
					last_item->addChild(img_background);
					last_item->addChild(custom_button);
				}
				_listviewPic->pushBackCustomItem(last_item);
			}

			Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&Category::UpdateListview), this);
//log("total:%d",m_nItemNum);
			auto textNum = dynamic_cast<Text *>(_createLayer->getChildByName("Text_ItemNum"));
			char sTempNum[10];
			if(m_nItemNum>0 && m_nItemNum<65535)
			{
				sprintf(sTempNum,"(%d)",m_nItemNum);
				string sNum=sTempNum;
				textNum->setString(sNum);
			}
			else
			{
				textNum->setString("");
			}
			
			//停止 动画，隐藏提示
			if (m_waitingCtrl)
			{
				m_waitingCtrl->stopAnimation();
				m_waitingCtrl->setVisible(false);
			}
			if(label_ratio)
			{
				label_ratio->removeFromParent();
				label_ratio=nullptr;
			}
			listener->setSwallowTouches(false);

			/*auto actionBy2 = MoveBy::create(0.3,Vec2(-Category_Width,0));
			_createLayer->runAction(actionBy2);
			bProductCated=true;*/

		}	//if (HttpConnection::numberOfLoadSprites == Number_Resources) 
		
	}	//else if(nFlag==5)
}
	
}

void Category::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidEnd.", editBox);
}

void Category::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidBegin.", editBox);
}
 
void Category::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text)
{
    log("editBox %p TextChanged, text: %s", editBox, text.c_str());
	auto btn = dynamic_cast<Ref*>(editBox->getParent()->getChildByName("Search_Item"));
	clickBtnCallback(btn, Widget::TouchEventType::ENDED);
}
 
void Category::editBoxReturn(cocos2d::extension::EditBox *editBox)
{
    log("editBox %p was returned.", editBox);
}
