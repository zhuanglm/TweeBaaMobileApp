//===========Tweebaa==============
//MainMenu.cpp	--Jly 20 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#include "CreateLayer.h"
#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../C2DXShareSDK/C2DXShareSDK.h"

//Add by Long for theme mode
#include "AppDelegate.h"

using namespace cn::sharesdk;

CreateLayer::right_menuItem CreateLayer::currentMenuItem=CreateLayer::right_menuItem::Save;
Size CreateLayer::visiableSize;
bool CreateLayer::m_bIsTheme=false;

USING_NS_CC;
using namespace ui;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CL_shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
			cocos2d::MessageBox("Share Successful","Share");
           // CCLog("分享成功");
            break;
        case C2DXResponseStateFail:
			cocos2d::MessageBox("Share Failed","Share");
           // CCLog("分享失败");
            break;
        default:
            break;
    }
}
#endif

CreateLayer::CreateLayer(Node* rootNode):m__pMain(nullptr)
{
	designLayer=nullptr;
	categoryLayer=nullptr;
	
	http_connect = new HttpConnection();
	initialize(rootNode);
}

CreateLayer::~CreateLayer(void)
{
	if( categoryLayer)
		categoryLayer->release();
	categoryLayer=nullptr;
	
	if( designLayer)
		designLayer->release();
	designLayer=nullptr;
	
	Layer::onExit();

	CC_SAFE_DELETE(http_connect);
}


/*void CreateLayer::ResetRMButton(Node* object)
{
	if(currentMenuItem==right_menuItem::AddItem) SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Trends"),true);
	else if(currentMenuItem==right_menuItem::Clear)SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Create"),true);
	else if(currentMenuItem==right_menuItem::Flip)SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Feed"),true);
	else if(currentMenuItem==right_menuItem::Save)SetButtonEnabled((Button*)object->getParent()->getChildByName("Button_Shop"),true);
}*/

void CreateLayer::touchAddItem(Node* object,TouchEventType type)
{

	if(DesignLayer::sp_vec.size()>MAX_PIC-1)
	{
		DesignLayer::MsgBox(Director::getInstance()->getRunningScene(),Vec2::ZERO,"ATTENTION",MSG_WARN_ITEM);
		return;
	}

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Add"),false);
			//if( categoryLayer)
			//	categoryLayer->release();
			if( !categoryLayer)	
				categoryLayer = new Category(object);
			else
				categoryLayer->initialize(object);

			//categoryLayer->autorelease();
		
			currentMenuItem=right_menuItem::AddItem;
			

			break;  
		  
		default:  
			break;  
    }
	
}

void CreateLayer::touchClone(Node* object,TouchEventType type)
{
	SPRITE_INFO sprite_info;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			
			if(DesignLayer::sp_vec.size()>0 && DesignLayer::selSprite)	
			{
				auto sprite = Sprite::createWithSpriteFrame(DesignLayer::selSprite->getSpriteFrame());
				
				sprite_info._filename = DesignLayer::selItem_info._filename;
				sprite_info.id=DesignLayer::selItem_info.id;
				strcpy(sprite_info.uid,DesignLayer::selItem_info.uid);
				sprite_info._position=Vec2(PIC_MATRIX/2,PIC_MATRIX/2);
				sprite_info.nZorder=DesignLayer::sp_info_vec.back().nZorder+1;
				sprite_info._seat=DesignLayer::sp_vec.size();
				sprite_info._type=ElementType::kETypeSprite;

				sprite->setPosition(sprite_info._position);
				DesignLayer::designArea->addChild(sprite,sprite_info.nZorder);

				DesignLayer::sp_vec.pushBack(sprite);
				DesignLayer::sp_info_vec.push_back(sprite_info);	// save properties for sprite
				DesignLayer::UndoAdd(OperationType::kOTypeAdd,sprite_info);
			}
			break;  
		  
		default:  
			break;  
    }
	
}

void CreateLayer::touchClear(Node* object,TouchEventType type)
{
	char *title=MSGBOX_TITLE;
	std::string msg = MSG_WARN_CLR;
	
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			//MsgBox_Clear(Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg);
			DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","CANCEL"
				,NULL,callfuncND_selector(CreateLayer::MsgBox_Clear_Callback));
			
			break;  
		  
		default:  
			break;  
    }
	
}

void CreateLayer::touchUndo(Node* object,TouchEventType type)
{
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			designLayer->Undo();
			break;
		default:  
			break;
	}
}

void CreateLayer::touchRedo(Node* object,TouchEventType type)
{
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			designLayer->Redo();
			break;
		default:  
			break;
	}
}

void CreateLayer::touchSave(Node* object,TouchEventType type)
{
	rectDesign = DesignLayer::designArea->getBoundingBox();

	char *title=MSGBOX_TITLE;
	std::string msg;

	AppDelegate *app = dynamic_cast<AppDelegate*>(Application::getInstance());
	_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
	G_UID = userInfo.strUserGuid;

	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(DesignLayer::v_undo.size()>0 && DesignLayer::v_undo.back().m_nSaved==OpSaveType::kSaved)
			{
				msg = MSG_SAVED_MSG;
				MsgBox_Published(Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg);
				break;
			}
			//G_UID = "eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8";
			if(G_UID.empty())		// for testing when un_login
			{
				msg = MSG_REGISTER_MSG;
				DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","NOT NOW"
					,NULL,callfuncND_selector(CreateLayer::MsgBox_Publish_Callback));
				break;
			}
			
			if(!DesignLayer::sp_vec.empty() || !DesignLayer::lb_vec.empty())
			{
				RenderTexture* render =  RenderTexture::create(PIC_MATRIX,PIC_MATRIX,Texture2D::PixelFormat::RGBA8888);
				render->beginWithClear(244, 225, 197, 1.0f);

				for(auto item : DesignLayer::sp_info_vec)
				{
					if(item._type==kETypeSprite)
					{
						DesignLayer::sp_vec.at(item._seat)->visit();
					}
					else
					{
						DesignLayer::lb_vec.at(item._seat)->visit(Director::getInstance()->getRenderer()
						,Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW),true);
					}
				}
			
				render->end();

				//auto filePath = FileUtils::getInstance()->getWritablePath() +"temp_design.png";
				render->saveToFile(Default_Draft_Design,kCCImageFormatPNG,false,CC_CALLBACK_2(CreateLayer::onSaveToFile,this));

			}
			break;
		default:  
			break;  
    }
	
}

void CreateLayer::onSaveToFile(RenderTexture* rt, const std::string s)
{
	char filename[128];
	int	nType;	//1 Draft; 2 Publish

	sprintf(filename,"%s",strrchr(s.c_str(),'/')+1);	// temp_design.png || temp_publish.png
	string fn=filename;
	nType=(fn=="temp_design.png")?1:2;
	
	//DesignLayer::designArea->setScale(1);
	//DesignLayer::designArea->setAnchorPoint(ancPos);
	
	/*for(auto item : DesignLayer::sp_info_vec)
	{
		if(item._type==kETypeSprite)
		{
			DesignLayer::sp_vec.at(item._seat)->setScale(DesignLayer::sp_vec.at(item._seat)->getScale()*2);
			DesignLayer::sp_vec.at(item._seat)->setPosition(item._position);
		}
		else
		{
			DesignLayer::lb_vec.at(item._seat)->setScale(DesignLayer::lb_vec.at(item._seat)->getScale()*2);
			DesignLayer::lb_vec.at(item._seat)->setPosition(item._position);
		}
	}*/
	//popup
	auto filePath = TEMP_filePath;
	filePath+=filename;
	
	strcpy(m_sFN,filePath.c_str());
	if(nType==1)
	{
		char *title=MSGBOX_TITLE;
		std::string msg = MSG_SAVE_DRAFT_MSG;

		DesignLayer::MsgBox_Doit(this,DesignLayer::designArea->getParent(),Vec2::ZERO,title,msg,"OK","CANCEL"
			,static_cast<void*>(m_sFN),callfuncND_selector(CreateLayer::MsgBox_Save_Callback));

	}
	else
	{
		popupLayer(DesignLayer::designArea->getParent(),Vec2::ZERO,false,m_sFN);	//true: published
	}
	
	return;

}

void CreateLayer::touchPublish(Node* object,TouchEventType type)
{
	rectDesign = DesignLayer::designArea->getBoundingBox();
	//origin = Director::getInstance()->getVisibleOrigin();
	char *title=MSGBOX_TITLE;
	std::string msg;
	
	AppDelegate *app = dynamic_cast<AppDelegate*>(Application::getInstance());
	_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
	G_UID = userInfo.strUserGuid;
	
	switch (type)  
    {  
		case TouchEventType::TOUCH_EVENT_ENDED:
			if(DesignLayer::v_undo.size()>0 && DesignLayer::v_undo.back().m_nSaved==OpSaveType::kPublished)
			{
				msg = MSG_PUBLISHED_MSG;
				MsgBox_Published(Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg);
				break;
			}
			
			//G_UID = "eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8";
			if(G_UID.empty())		// for testing when un_login
			{
				
				
				msg = MSG_REGISTER_MSG;
				//MsgBox_Publish(Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg);
				DesignLayer::MsgBox_Doit(this,Director::getInstance()->getRunningScene(),Vec2::ZERO,title,msg,"OK","NOT NOW"
					,NULL,callfuncND_selector(CreateLayer::MsgBox_Publish_Callback));
				break;
			}
			
			if(!DesignLayer::sp_vec.empty() || !DesignLayer::lb_vec.empty())
			{
				//pScene = Director::getInstance()->getRunningScene();
				//pScene = Director::sharedDirector()->getRunningScene();
				ancPos = DesignLayer::designArea->getAnchorPoint();
				
				RenderTexture* render =  RenderTexture::create(PIC_MATRIX,PIC_MATRIX,Texture2D::PixelFormat::RGBA8888); 
				//render->setKeepMatrix(true);
				
				/*render->setVirtualViewport(rectDesign.origin,Rect(origin.x,origin.y,visiableSize.width,visiableSize.height),
					Rect(origin.x,origin.y,visiableSize.width,visiableSize.height));*/
			
				render->beginWithClear(244, 225, 197, 1.0f);
				
				//DesignLayer::designArea->setScale(.5);
				//DesignLayer::designArea->setAnchorPoint(Vec2(PIC_MATRIX,PIC_MATRIX));
				
				for(auto item : DesignLayer::sp_info_vec)
				{
					if(item._type==kETypeSprite)
					{
						//DesignLayer::sp_vec.at(item._seat)->setScale(DesignLayer::sp_vec.at(item._seat)->getScale()*0.5);		//some pictures was scaled before
						//DesignLayer::sp_vec.at(item._seat)->setPosition(item._position/2);
						DesignLayer::sp_vec.at(item._seat)->visit();
					}
					else
					{
						//DesignLayer::lb_vec.at(item._seat)->setScale(DesignLayer::lb_vec.at(item._seat)->getScale()*0.5);
						//DesignLayer::lb_vec.at(item._seat)->setPosition(item._position/2);
						DesignLayer::lb_vec.at(item._seat)->visit(Director::getInstance()->getRenderer()
						,Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW),true);
					}
				}
			
				render->end();

				//auto filePath = FileUtils::getInstance()->getWritablePath() +"temp1.png";
				render->saveToFile(Default_Publish_Design,Image::Format::JPG,true,CC_CALLBACK_2(CreateLayer::onSaveToFile,this));
				//utils::captureScreen(CC_CALLBACK_2(CreateLayer::afterCapture, this), "temp_publish.png");
				
			}
			break;
		default:  
			break;  
    }
	
}

void CreateLayer::afterCapture(bool succeed, const std::string& outputFile)
{
}

void CreateLayer::popupLayer(Node* baseLayer,Vec2 popPos,bool Done_Flag,const char * pic){
    // 定义一个弹出层，传入cocostudio界面
    PublishLayer* pl = PublishLayer::create("Collage/Publish_Layer.csb",Done_Flag,pic);
	
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);

    pl->setCallbackFunc(this, callfuncN_selector(CreateLayer::popupCallback));
	
    // 添加到指定层
	baseLayer->addChild(pl);
	
}

void CreateLayer::popupCallback(cocos2d::Node *pNode)
{
//log("button call back. tag: %d:%s", pNode->getTag(),pNode->getName().c_str());

	char url[128];
	char buffer[4096],segment[4096];
	string	tempDesignFile;
	char * userID = "eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8";
	char tmp[512],uid[64];
	unsigned int	nItemNumber = DesignLayer::sp_info_vec.size();
	unsigned int	nItem=0,nLen=0;

	//string filePath = FileUtils::getInstance()->getWritablePath();
	string newFile,sBuffer;
	sctPicList	newDesign;
	char * cp;

	AppDelegate *app = dynamic_cast<AppDelegate*>(Application::getInstance());

	sqlite3* pDB;
	char sSQL[512],sTitle[24],sDes[128],*err;

	segment[0]='\0';
	auto lt = time(NULL);
	auto cdate = ctime(&lt);
	sqlite3_int64	x;

	switch(pNode->getTag())
	{
		case MenuItem_OK:
			//upload file
			strcpy(url,Req_URL);
			std::strcat(url,Post_FileName);
			HttpConnection::nShift=0;	//init receive buffer
			tempDesignFile = pNode->getName();
			http_connect->CurlPost(url,tempDesignFile.c_str(),true);
			if(HttpConnection::nShift==0)
			{
				cocos2d::log("return NULL");
				cocos2d::MessageBox("Null Return","Network Service Error");
				return;
			}

			//save as cache file
			cp = strrchr(HttpConnection::res_buf,'/');
			if(!cp)
			{
				cocos2d::log("return filename error");
				cocos2d::MessageBox("Filename Format Return ERROR!","Network Service Error");
				return;
			}
			std::sprintf(buffer,"$D_%s",cp+1);
			newFile=TEMP_filePath+Cache_Path+buffer;
			std::remove(newFile.c_str());
			std::rename(tempDesignFile.c_str(),newFile.c_str());

			newDesign.id=0;
			newDesign.cache_filename=newFile;
			newDesign._filename=HttpConnection::res_buf;
			newDesign._objecttitle=PublishLayer::InputCont1;
			newDesign._object4th=PublishLayer::InputCont2;		//inspiration
			newDesign._object6th="1";
			newDesign._object5th=G_UNM;	//username
			newDesign._object8th="0";

			//post collage parameter
	
			//G_UID = app->GetUserGuid();
			if(G_UID.empty())		// for testing when un_login
				G_UID=userID;

			for(int i=0;i<nItemNumber;i++)
			{
				if(DesignLayer::sp_info_vec.at(i)._type!=kETypeSprite)
					continue;
			
				nItem++;
				std::sprintf(tmp,"&img%d=%s&x%d=%f&y%d=%f&s%d=%f&a%d=%f&p%d=%s",nItem,DesignLayer::sp_info_vec.at(i)._filename.c_str(),nItem,
					DesignLayer::sp_info_vec.at(i)._position.x,nItem,DesignLayer::sp_info_vec.at(i)._position.y,nItem,
					DesignLayer::sp_vec.at(DesignLayer::sp_info_vec.at(i)._seat)->getScale(),nItem
					,DesignLayer::sp_vec.at(DesignLayer::sp_info_vec.at(i)._seat)->getRotation(),nItem,DesignLayer::sp_info_vec.at(i).uid);
				std::strcat(segment,tmp);
			}

			std::sprintf(buffer,"IMG=%s&SaveType=%d&title=%s&description=%s&Designer=%s&ItemTotal=%d",HttpConnection::res_buf,2
				,PublishLayer::InputCont1.c_str(),PublishLayer::InputCont2.c_str(),G_UID.c_str(),nItem);
			sBuffer = buffer;
			nLen = sBuffer.size();
			//std::strcat(buffer,segment);
			strcpy(buffer+nLen,segment);

			//upload parameters
			strcpy(url,Req_URL);
			std::strcat(url,Post_Parameter);
			HttpConnection::nShift=0;	//init receive buffer
			http_connect->CurlPost(url,buffer,false);

//cocos2d::log("upload:%s",buffer);
//cocos2d::log("return[%d]:%s",HttpConnection::nShift,HttpConnection::res_buf);
			
			app->updateCollageScore(UPDATE_SCORE_PUBLISH);
			
			for(auto& v : DesignLayer::v_undo)
			{
				v.m_nSaved = OpSaveType::kPublished;
			}
			SetButtonEnabled(buttonPublish,false);
			//popupLayer(DesignLayer::designArea->getParent(),Vec2::ZERO,true,newDesign.cache_filename.c_str());	//true: published
			MsgBox_Share(DesignLayer::designArea->getParent(),Vec2::ZERO,"Share your design","");

			std::sprintf(newDesign.uid,"D_%s",HttpConnection::res_buf);
			for(auto& vTmp : HttpConnection::v_design_like_info)
			{
				vTmp.id++;
			}
			HttpConnection::v_design_like_info.insert(HttpConnection::v_design_like_info.begin(),newDesign);
			strcpy(sTitle,newDesign._objecttitle.c_str());
			strcpy(sDes,newDesign._object4th.c_str());
			CollageTemplateList::Str2SQL(sTitle);
			CollageTemplateList::Str2SQL(sDes);
			
			pDB = CollageTemplateList::initDB("Collage.db3");
			sprintf(sSQL,"insert into CollageDesigns (id,cdate,image,title,inspiration,user,like_count,personal_flag,share_count) values ('%s','%s','%s','%s','%s','%s','%s','0','%s')"
				,HttpConnection::res_buf,cdate,newDesign._filename.c_str(),sTitle,sDes,newDesign._object5th.c_str(),"0","0");
			if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
			{
				   log("insert new published design to table[designs] failure.");
			}
			
			sqlite3_close(pDB);
			SetButtonEnabled((Button*)DesignLayer::designArea->getParent()->getChildByName("Button_Feed"),true);

			if(HttpConnection::v_design_like_info.size()>MAX_CacheFile)
			{
				HttpConnection::v_design_like_info.pop_back();
			}


			break;
		case MenuItem_CANCEL:
			PublishLayer::InputCont1.clear();
			PublishLayer::InputCont2.clear();
			break;
		default:
			
			/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				CCDictionary *content = CCDictionary::create();
				content -> setObject(CCString::create(PublishLayer::InputCont1), "content");
				content -> setObject(CCString::create(pNode->getName()), "image");
				content -> setObject(CCString::create("Tweebaa App"), "title");
				content -> setObject(CCString::create("I win the game"), "description");
				content -> setObject(CCString::create("http://tweebaa.com"), "url");
				content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
				content -> setObject(CCString::create("http://tweebaa.com"), "siteUrl");
				content -> setObject(CCString::create("Tweebaa"), "site");
				//content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
				content -> setObject(CCString::create("extInfo"), "extInfo");
    
				C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, CL_shareResultHandler);
	
			#endif*/
//log("id=%s",HttpConnection::v_design_like_info.begin()->uid);
			sprintf(url,"%s%s",COllage_Share,HttpConnection::v_design_like_info.begin()->uid+2);
			Application::getInstance()->openURL(url);

			PublishLayer::InputCont1.clear();	//Title
			PublishLayer::InputCont2.clear();	//description

			break;
	}
}

void CreateLayer::initialize(Node * rootNode) 
{
	m_bIsTheme = false;
	m__pMain = rootNode->getParent()->getParent();
	
	origin = Director::getInstance()->getVisibleOrigin();
	visiableSize = Director::getInstance()->getVisibleSize();
	
	_design = CSLoader::createNode("Collage/design_area.csb");
	_design->setAnchorPoint(Vec2(.5,.5));
	_design->setPosition(Vec2(Design_Pos_X,visiableSize.height/2));
	//_design->setOpacity(150);
	rootNode->getParent()->addChild(_design,-5);
	_design->setTag(Design_Tag);

	DesignLayer::_rightMenu = CSLoader::createNode("Collage/RightMenu.csb");
	DesignLayer::_rightMenu->setAnchorPoint(Vec2::ZERO);
	DesignLayer::_rightMenu->setPosition(Vec2(Window_Width-R_Menu_Width,0));
	rootNode->getParent()->addChild(DesignLayer::_rightMenu);
	DesignLayer::_rightMenu->setTag(MenuItem_Create);

	SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Clone"),false);
	designLayer = new DesignLayer(rootNode);
		
    auto buttonAdd = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Add"));
	auto buttonClear = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Clear"));
	auto buttonClone = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Clone"));
	auto buttonUndo = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Undo"));
	auto buttonRedo = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Redo"));
	
	buttonSave = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Save"));
	buttonPublish = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Publish"));
	//auto buttonText = dynamic_cast<Button*>(DesignLayer::_rightMenu->getChildByName("Button_Text"));
	//buttonRedo->setVisible(false);
	//buttonSave->setVisible(false);
	
	
	if(!DesignLayer::v_undo.size())
	{
		SetButtonEnabled(buttonUndo,false);
	}
	
	if(!DesignLayer::v_redo.size())
		SetButtonEnabled(buttonRedo,false);

	if(!DesignLayer::sp_vec.size()) //no items
	{
		SetButtonEnabled(buttonPublish,false);
		SetButtonEnabled(buttonSave,false);
		SetButtonEnabled(buttonClear,false);
	}
	else
	{
		if(!DesignLayer::v_undo.size())		//loaded from draft
		{
			SetButtonEnabled(buttonSave,false);
			//SetButtonEnabled(buttonPublish,true);
		}
		else
		{
			//shift from other main menu
			if(DesignLayer::v_undo.back().m_nSaved==OpSaveType::kSaved)
			{
				SetButtonEnabled(buttonSave,false);
				SetButtonEnabled(buttonPublish,true);
			}
			else if(DesignLayer::v_undo.back().m_nSaved==OpSaveType::kPublished)
			{
				SetButtonEnabled(buttonSave,true);
				SetButtonEnabled(buttonPublish,false);
			}
			else
			{
				SetButtonEnabled(buttonSave,true);
				SetButtonEnabled(buttonPublish,true);
			}
		}
	}
		
	//buttonClear->addTouchEventListener(CC_CALLBACK_2(CreateLayer::touchClear,_rightMenu));
	buttonClear->addTouchEventListener(this,toucheventselector(CreateLayer::touchClear));
	buttonAdd->addTouchEventListener(this,toucheventselector(CreateLayer::touchAddItem));
	buttonClone->addTouchEventListener(this,toucheventselector(CreateLayer::touchClone));
	buttonUndo->addTouchEventListener(this,toucheventselector(CreateLayer::touchUndo));
	buttonRedo->addTouchEventListener(this,toucheventselector(CreateLayer::touchRedo));
	buttonSave->addTouchEventListener(this,toucheventselector(CreateLayer::touchSave));
	buttonPublish->addTouchEventListener(this,toucheventselector(CreateLayer::touchPublish));

	
}


void CreateLayer::MsgBox_Share(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent)
{

    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	//pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 150);
	
    pl->setCallbackFunc(this, callfuncND_selector(CreateLayer::MsgBox_Share_Callback),NULL);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "RETURN",Color3B(255,255,255), 0);
	pl->addContentButton("facebook.png", "facebook_d.png", "",Color3B(255,255,255), 1);
	pl->addContentButton("google.png", "google_d.png", "",Color3B(255,255,255), 1);
	pl->addContentButton("pinterest2.png", "pinterest2_d.png", "",Color3B(255,255,255), 1);
	pl->addContentButton("twitter2.png", "twitter2_d.png", "",Color3B(255,255,255), 1);

    // 添加到指定层
	baseLayer->addChild(pl);
}

void CreateLayer::MsgBox_Share_Callback(cocos2d::Node *pNode,void* data)
{
	char url[128];

	if(pNode->getTag())
	{
		strcpy(url,Req_URL);
		sprintf(url,"%s%s",COllage_Share,HttpConnection::v_design_like_info.begin()->uid+2);
		Application::getInstance()->openURL(url);
	}
	PublishLayer::InputCont1.clear();	//Title
	PublishLayer::InputCont2.clear();	//description
	
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if(app->iGameMode==MobileGameMode::Theme_Mode)
	{
		pNode->setTag(1);
		m_bIsTheme = true;
		dynamic_cast<MainMenu *>(m__pMain)->MsgBox_Quit_Callback(pNode,NULL);
		//app->GameRunRoundNext();
	}
}

/*void CreateLayer::MsgBox_Publish(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent){

    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 150);
	
    pl->setCallbackFunc(this, callfuncN_selector(CreateLayer::MsgBox_Publish_Callback));
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "OK",Color3B(255,255,255), 1);
	pl->addButton("button_normal.png", "button_dis.png", "NOT NOW",Color3B(255,255,255), 0);

    // 添加到指定层
	baseLayer->addChild(pl);
}*/

void CreateLayer::MsgBox_Publish_Callback(cocos2d::Node *pNode,void* data)
{
	if(pNode->getTag())
	{
		auto scene = RegisterView::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}
	
}

void CreateLayer::MsgBox_Published(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent){

    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 250);
	
    pl->setCallbackFunc(this, callfuncND_selector(CreateLayer::MsgBoxCallback),NULL);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "OK",Color3B(255,255,255), 1);
    // 添加到指定层
	baseLayer->addChild(pl);
}

void CreateLayer::MsgBoxCallback(cocos2d::Node *pNode,void* data)
{
//log("button call back. tag: %d", pNode->getTag());
	
}

/*void CreateLayer::MsgBox_Clear(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent){

    PopupLayer* pl = PopupLayer::create("popup_sq.png");

    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 200);
	
    pl->setCallbackFunc(this, callfuncN_selector(CreateLayer::MsgBox_Clear_Callback));
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "OK",Color3B(255,255,255), 1);
	pl->addButton("button_normal.png", "button_dis.png", "CANCEL",Color3B(255,255,255), 0);

    // 添加到指定层
	baseLayer->addChild(pl);
}*/

void CreateLayer::MsgBox_Clear_Callback(cocos2d::Node *pNode,void* data)
{
	if(pNode->getTag())
	{
			if(DesignLayer::sp_vec.size()>0)	
				for(Sprite * sp:DesignLayer::sp_vec)
				{
					sp->removeFromParent();
				}
			if(DesignLayer::lb_vec.size()>0)	
				for(Label * lb:DesignLayer::lb_vec)
				{
					lb->removeFromParent();
				}
			DesignLayer::sp_vec.clear();
			DesignLayer::lb_vec.clear();
			DesignLayer::sp_info_vec.clear();
			DesignLayer::UndoClear();
			DesignLayer::RedoClear();
			DesignLayer::selSprite=nullptr;
			DesignLayer::selLabel=nullptr;
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Clone"),false);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Save"),false);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Publish"),false);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Clear"),false);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Undo"),false);
			SetButtonEnabled((Button*)DesignLayer::_rightMenu->getChildByName("Button_Redo"),false);
	}
	
}

void CreateLayer::MsgBox_Save_Callback(cocos2d::Node *pNode,void *pData)
{
	char sSource[128],sPostfix[16];
	strcpy(sSource,(char *)pData);
	strcpy(sPostfix,strrchr(sSource,'.'));

	if(pNode->getTag())
	{
//log("click back: %d,%s",pNode->getTag(),(char *)pData);
		auto lt = time(NULL);
		auto cdate = ctime(&lt);
		auto pDB = CollageTemplateList::initDB("Collage.db3");
		char sSQL[512],sFileName[128],*err;

		sprintf(sSQL,"insert into DraftList (UUID,cdate) values ('%s','%s')",G_UID.c_str(),cdate);
		if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
		{
		   //sqlite3_close(pDB);
		   log("insert table[Draft_List] failure.");
		}

		auto RecNo = sqlite3_last_insert_rowid(pDB) ;
		sprintf(sFileName,"_%02d",RecNo);
		string sFN = G_UNM+sFileName+sPostfix;
		std::sprintf(sSQL,"update DraftList set filename = '%s' where rowid = %lld",sFN.c_str(),RecNo);
//log("filename[%s] -SQL:%s",sFN.c_str(),sSQL);
		if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
		{
		   //sqlite3_close(pDB);
		   log("update table[Draft_List] failure.");
		}

		string newFile=TEMP_filePath+sFN;
		std::remove(newFile.c_str());
		std::rename((char *)pData,newFile.c_str());

		char	sLabel[64];
		float	fScale,fAngle;
		string	font_path="";
		unsigned int	label_color=0;
		unsigned short	flip_flag=0;

		for(auto vT : DesignLayer::sp_info_vec)
		{
			if(vT._type==ElementType::kETypeSprite)
			{
				sLabel[0] = '\0';
				fScale = DesignLayer::sp_vec.at(vT._seat)->getScale();
				fAngle = DesignLayer::sp_vec.at(vT._seat)->getRotation();
				if(DesignLayer::sp_vec.at(vT._seat)->isFlippedX())
					flip_flag+=1;
				if(DesignLayer::sp_vec.at(vT._seat)->isFlippedY())
					flip_flag+=2;
			}
			else
			{
				strcpy(sLabel,DesignLayer::lb_vec.at(vT._seat)->getString().c_str());
				CollageTemplateList::Str2SQL(sLabel);
				fScale = DesignLayer::lb_vec.at(vT._seat)->getScale();
				fAngle = DesignLayer::lb_vec.at(vT._seat)->getRotation();
				font_path = DesignLayer::lb_vec.at(vT._seat)->getTTFConfig().fontFilePath;
				label_color = RGB2I(DesignLayer::lb_vec.at(vT._seat)->getColor());
				
			}

			sprintf(sSQL,"insert into DraftInfo (uid,filename,id,type,string,pos_x,pos_y,pos_z,scale,angle,flip,color,font,cat) values\
						 ('%s','%s',%lld,%d,'%s',%f,%f,%d,%f,%f,%d,%d,'%s',%d)",vT.uid,vT._filename.c_str(),RecNo,vT._type
						 ,sLabel,vT._position.x,vT._position.y,vT.nZorder,fScale,fAngle,flip_flag,label_color,font_path.c_str(),vT._Cat);
//log("save:%s",sSQL);
			if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
			{
			   //sqlite3_close(pDB);
			   log("insert table[Draft_Info] failure.");
			}
//log("insert sucess!");
			font_path.clear();
		}

		sqlite3_close(pDB);

		for(auto& v : DesignLayer::v_undo)
		{
			v.m_nSaved = OpSaveType::kSaved;
		}
		SetButtonEnabled(buttonSave,false);
	}
}

