//===========Tweebaa==============
//ProfileLayer.cpp	--Nov 25 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#include "ProfileLayer.h"
#include "AppDelegate.h"

int	ProfileLayer::nCurPage;

ProfileLayer::ProfileLayer():
	 m__pPopBox(NULL)
{
	//initialize(pNode);
}


ProfileLayer::~ProfileLayer(void)
{
	 CC_SAFE_RELEASE(m__pPopBox);
}


bool ProfileLayer::init(){
	if(!Layer::init()){
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ProfileLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setContentSize(Size::ZERO);
	
	return true;
}

bool ProfileLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
//log("PopupLayer touch");
    return true;
}

ProfileLayer * ProfileLayer::create(const char *backgroundCSB)
{
	ProfileLayer* ml = ProfileLayer::create();
	ml->setBackGround(CSLoader::createNode(backgroundCSB));

    return ml;
}

void ProfileLayer::onEnter()
{
    Layer::onEnter();

	if (getContentSize().equals(Size::ZERO)) {
		getBackGround()->setPosition(Vec2::ZERO);
		auto layerColor = LayerColor::create();
		layerColor->setColor(Color3B(0, 0, 0));  
		layerColor->setOpacity(255*0.85);
        layerColor->addChild(getBackGround(), 0, 0);
		this->addChild(layerColor);
    } 
	
	buttonDel = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_Del"));
	buttonDel->setVisible(false);
	//buttonDel->addTouchEventListener(this,toucheventselector(ProfileLayer::Del_Callback));
	buttonDel->addTouchEventListener(CC_CALLBACK_2(ProfileLayer::touchDel,this));

	auto buttonClose = dynamic_cast<Button*>(m__pPopBox->getChildByName("Button_Close"));
	buttonClose->addTouchEventListener(this,toucheventselector(ProfileLayer::Close_Callback));

	auto buttonAvatar = dynamic_cast<ImageView*>(m__pPopBox->getChildByName("Image_Avatar"));
	buttonAvatar->setTouchEnabled(true);
	buttonAvatar->addTouchEventListener(this,toucheventselector(ProfileLayer::Avatar_Callback));

	auto textUserName = dynamic_cast<Text*>(m__pPopBox->getChildByName("Text_UserName"));
	textUserName->setString(G_UNM);
	auto textScore = dynamic_cast<Text*>(m__pPopBox->getChildByName("Text_Score"));
	textScore->setString(G_Score);
	auto textLevel = dynamic_cast<Text*>(m__pPopBox->getChildByName("Text_Level"));
	textLevel->setString(G_Level);
	auto textPercentage = dynamic_cast<Text*>(m__pPopBox->getChildByName("Text_Level_Percentage"));
	textPercentage->setString(G_Percentage);
	textPercentage->setVisible(false);
	auto imgPercentage = dynamic_cast<ImageView*>(m__pPopBox->getChildByName("Image_Level_Percentage"));
	imgPercentage->setScaleX(G_fP);
	
	auto browse = dynamic_cast<PageView*>(m__pPopBox->getChildByName("Panel_Main")->getChildByName("PageView_Draft"));
	browse->setTouchEnabled(true);
	visiableSize = browse->getSize();
	browse-> setCustomScrollThreshold(visiableSize.width/10);
	browse->addEventListenerPageView(this, pagevieweventselector(ProfileLayer::TrendsEvent));

	char sSQL[512];
	string sSavedFile,sDate;

	sprintf(sSQL,"select * from DraftList where UUID = '%s'",G_UID.c_str());
	char **re;
	int r,c;
	auto pDB = CollageTemplateList::initDB(DB_File);
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK) 
	{
		sqlite3_close(pDB);
        log("load * from table[Draft_List] failure.");
		return;
    }
	if(r==0)
	{
		sqlite3_free_table(re);
		sqlite3_close(pDB);
		log("no record in table[Draft_List] for %s.",G_UNM.c_str());
		return;
	}
	buttonDel->setVisible(true);
	for(int i=1;i<r+1;i++)
	{
		if(!re[i*c])
		{
			continue;
		}
		else
		{
			sSavedFile = TEMP_filePath + re[i*c];
			if(sSavedFile.find(".png")==string::npos && sSavedFile.find(".jpg")==string::npos)
				continue;
		}
		
		sDate = re[i*c+2];

		auto pageLayer=Layout::create();
		pageLayer->setContentSize(Size(visiableSize.width,visiableSize.height));
		pageLayer->setName(sDate);
		browse->addPage(pageLayer);

		auto sprite = ImageView::create(sSavedFile);
		if (sprite){
			sprite->setName(re[i*c]);
			sprite->setTag(Current_Item_Tag);
			sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			sprite->setPosition(Vec2(visiableSize.width / 2, visiableSize.height / 2 ));
			sprite->setScaleX(visiableSize.width/sprite->getContentSize().width);
			sprite->setScaleY(visiableSize.height/sprite->getContentSize().height);

			sprite->setScale9Enabled(true);
			sprite->setTouchEnabled(true);
			sprite->addTouchEventListener(CC_CALLBACK_2(ProfileLayer::clickImgCallback, this));
			pageLayer->addChild(sprite);
		}
	}
	sqlite3_free_table(re);
	sqlite3_close(pDB);
	browse->scrollToPage(0);

}

void ProfileLayer::onExit()
{
    Layer::onExit();
}

void  ProfileLayer::clickImgCallback(cocos2d::Ref *pSender, Widget::TouchEventType event) 
{
	char *title=MSGBOX_TITLE;
	std::string msg = MSG_LOAD_DRAFT_MSG;
	auto pImgName = dynamic_cast<ImageView*>(pSender)->getName();

	switch (event)  
	{ 
		case Widget::TouchEventType::ENDED:
			memcpy(m_pData,pImgName.c_str(),pImgName.size()+1);
			DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","CANCEL"
				,m_pData,callfuncND_selector(ProfileLayer::MsgBox_Load_Callback));
			break;
		default:
			break;
	}
}

void ProfileLayer::MsgBox_Load_Callback(cocos2d::Node *pNode,void *data)
{
	if(pNode->getTag())
	{
//log("click back: %d",pNode->getTag());
		Node* node = dynamic_cast<Node*>(pNode);
		if (m_callback && m_callbackListener){
			(m_callbackListener->*m_callback)(node,data);
		}
		this->removeAllChildren();
		this->removeFromParent();
	}
}


void ProfileLayer::TrendsEvent(Ref * pSender, PageView::EventType type)
{
	PageView * pageView = static_cast<PageView *>(pSender);
	ssize_t	nCurIndex = pageView->getCurPageIndex();
	int	nCurItem = pageView->getPage(nCurIndex)->getTag();
	int nTotalItems = pageView->getChildrenCount();
	int	nCurPos;
	char sTmp[16];
	auto textDate = dynamic_cast<Text*>(m__pPopBox->getChildByName("Text_Date"));
	auto textPage = dynamic_cast<Text*>(m__pPopBox->getChildByName("Text_Page"));
	sprintf(sTmp,"Page %d of %d",nCurIndex+1,nTotalItems);
	
	switch (type)
	{
		case PageView::EventType::TURNING:
			textDate->setString(pageView->getPage(nCurIndex)->getName());
			textPage->setString(sTmp);
			nCurPage = nCurIndex;
			break;

		
		default:
			break;
	}
}

void ProfileLayer::MsgBox_Del_Callback(cocos2d::Node *pNode,void*)
{
    char sSQL[512],*err;
	
	if(pNode->getTag())
	{
		auto browse = dynamic_cast<PageView*>(m__pPopBox->getChildByName("Panel_Main")->getChildByName("PageView_Draft"));
		ssize_t	nCurIndex = browse->getCurPageIndex();
		auto sCurName = browse->getPage(nCurIndex)->getChildByTag(Current_Item_Tag)->getName();
		//delete from DB
		sprintf(sSQL,"select rowid from DraftList where filename='%s'",sCurName.c_str());
		char **re;
		int r,c;
		auto pDB = CollageTemplateList::initDB(DB_File);
		if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK) 
		{
			sqlite3_close(pDB);
			log("load rowid from table[draftlist] failure.");
			return;
		}
		if(r==0)
		{
			sqlite3_free_table(re);
			sqlite3_close(pDB);
			log("no record match %s.",sCurName.c_str());
			return;
		}
		unsigned long long RecNo = atoi(re[1]);

		//sprintf(sSQL,"delete from DraftList where filename='%s'",sCurName.c_str());
		sprintf(sSQL,"delete from DraftList where rowid=%lld",RecNo);
		if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
		{
		   //sqlite3_close(pDB);
		   log("delete table[Draft_List] failure.");
		}
		sprintf(sSQL,"delete from DraftInfo where id=%lld",RecNo);
		if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
		{
		   //sqlite3_close(pDB);
		   log("delete table[Draft_Info] failure.");
		}

		sqlite3_free_table(re);
		sqlite3_close(pDB);
		browse->removePageAtIndex(nCurIndex);
		string sTexture = TEMP_filePath + sCurName;
		Director::getInstance()->getTextureCache()->removeTextureForKey(sTexture);
		std::remove(sTexture.c_str());
		if(browse->getChildrenCount()==0)
			buttonDel->setVisible(false);
		//this->removeFromParent();
	}
}

void ProfileLayer::setCallbackFunc(cocos2d::Object *target, SEL_CallFuncND callfun,void * data){
    m_callbackListener = target;
    m_callback = callfun; 
	m_pData = data;
}

void ProfileLayer::Close_Callback(cocos2d::Object *pSender,TouchEventType type)
{
	if(type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		this->removeFromParent();
	}
}

void ProfileLayer::Avatar_Callback(cocos2d::Object *pSender,TouchEventType type)
{

	if(type == TouchEventType::TOUCH_EVENT_ENDED)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//GetImage::getInstance()->callGetImageWithPhotoAndCamera([=](std::string path)	{		});
		//GetImage::destroyInstance();
#endif
	}

}

void ProfileLayer::touchDel(Ref* object,cocos2d::ui::Widget::TouchEventType type)
{
    char *title=MSGBOX_TITLE;
	std::string msg = MSG_WARN_Del;

	if(type == Widget::TouchEventType::ENDED)
	{
		DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","CANCEL"
				,NULL,callfuncND_selector(ProfileLayer::MsgBox_Del_Callback));
	}
}