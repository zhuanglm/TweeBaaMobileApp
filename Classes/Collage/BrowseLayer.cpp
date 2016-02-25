//===========Tweebaa==============
//BrowseLayer.cpp	--Jly 20 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#include "BrowseLayer.h"
#include "AppDelegate.h"

Text*	BrowseLayer::textLike;
Text*	BrowseLayer::textShare;
Layer*	BrowseLayer::infoArea;
Label *BrowseLayer::label_Designer;
Label *BrowseLayer::label_Title;
Label *BrowseLayer::label_Description;
int	BrowseLayer::nCurPage;


BrowseLayer::BrowseLayer(cocos2d::Node* pNode)
{
	if(nCurPage>MAX_CacheFile || nCurPage<0) nCurPage = 0;
	http_connect = new HttpConnection();
	initialize(pNode);
}


BrowseLayer::~BrowseLayer(void)
{
	infoArea->removeFromParent();
	Layer::onExit();
	CC_SAFE_DELETE(http_connect);
	CC_SAFE_DELETE(pCTL);
}


bool BrowseLayer::init(){
	if(!Layer::init()){
		return false;
	}
	//auto BrowseNode = CSLoader::createNode("BrowseLayer.csb");
	//addChild(BrowseNode);
	
	return true;

}

void BrowseLayer::initialize(cocos2d::Node* pNode)
{
	pCTL = CollageTemplateList::LoadXMLFromDB("Collage.db3","CollageDesigns","share_count");

	textLike = dynamic_cast<Text*>(pNode->getChildByName("Text_Like"));
	buttonLike = dynamic_cast<CheckBox*>(pNode->getChildByName("Button_Like"));
	buttonLike->setSelectedState(false);

	//AppDelegate *app = (AppDelegate*)Application::getInstance();
	//auto G_UID = app->GetUserGuid();
	//string G_UID = "";
	if(G_UID.empty())
	{
		buttonLike->setVisible(false);
		textLike->setVisible(false);
	}
	else
	{
		buttonLike->addEventListenerCheckBox(this,checkboxselectedeventselector(BrowseLayer::touchLike));
	}
	textShare = dynamic_cast<Text*>(pNode->getChildByName("Text_Share"));

	//infoArea = dynamic_cast<ListView*>(pNode->getChildByName("ListView_Menu"));
	infoArea = Layer::create();
	//Director::getInstance()->getRunningScene()->addChild(infoArea);
	pNode->getParent()->getParent()->addChild(infoArea);

	buttonShare = dynamic_cast<Button*>(pNode->getChildByName("Button_Share"));
	buttonShare->addTouchEventListener(CC_CALLBACK_2(BrowseLayer::touchShare, this));

	auto buttonDel = dynamic_cast<Button*>(pNode->getChildByName("Button_Del"));
	buttonDel->setVisible(false);

	buttonLeft = dynamic_cast<Button*>(pNode->getChildByName("Button_Left"));
	buttonLeft->addTouchEventListener(CC_CALLBACK_2(BrowseLayer::touchLeft,this));
	buttonRight = dynamic_cast<Button*>(pNode->getChildByName("Button_Right"));
	buttonRight->addTouchEventListener(CC_CALLBACK_2(BrowseLayer::touchRight,this));
	
	label_Designer = Label::createWithTTF(HttpConnection::v_design_time_info.at(nCurPage)._object5th, Default_Chinese_Font, 16);
	label_Designer->setColor(Color3B(0, 0, 0));
	label_Designer->setAnchorPoint(Vec2(0,1));
	label_Designer->setPosition(850,30);
	label_Designer->setDimensions(80,50);
	infoArea->addChild(label_Designer);

	auto browseTrends = static_cast<BrowsePage*>(pNode->getChildByName("PageView_Product"));
	//pNode->reorderChild(browseTrends,-20);
	browseTrends->setTouchEnabled(true);
	browseTrends->setClippingEnabled(true);
	browseTrends-> setCustomScrollThreshold(PIC_MATRIX/10);
	
	visiableSize = browseTrends->getSize();
	browseTrends->addEventListener(CC_CALLBACK_2(BrowseLayer::TrendsEvent,this));

	auto rightBar = static_cast<ImageView*>(pNode->getChildByName("Image_RightSide"));
	rightBar->addTouchEventListener(CC_CALLBACK_2(BrowseLayer::RightBarCallBack, this));
	rightBar->setTouchEnabled(true);

	m_Slider = static_cast<Slider*>(pNode->getChildByName("Slider_Browse"));
	m_Slider->addEventListener(CC_CALLBACK_2(BrowseLayer::sliderEvent, this));

	label_Title = Label::createWithTTF(HttpConnection::v_design_time_info.at(nCurPage)._objecttitle, Default_Chinese_Font, 24);
	label_Title->setColor(Color3B(0, 0, 0));
	label_Title->setAnchorPoint(Vec2(0,1));
	label_Title->setPosition(460,30);
	//label_Title->setDimensions(100,160);
	infoArea->addChild(label_Title);

	label_Description = Label::createWithTTF(HttpConnection::v_design_time_info.at(nCurPage)._object4th, Default_Chinese_Font, 20);
	label_Description->setColor(Color3B(106, 72, 36));
	label_Description->setAnchorPoint(Vec2(0,1));
	label_Description->setPosition(820,400);
	label_Description->setDimensions(R_Menu_Width-10,100);
	label_Description->setLineBreakWithoutSpace(true);
	infoArea->addChild(label_Description);

	
	for(sctPicList cache_Design:HttpConnection::v_design_time_info)
	{
		auto pageLayer=Layout::create();
		pageLayer->setContentSize(Size(visiableSize.width,visiableSize.height));
		pageLayer->setTag(cache_Design.id);
		browseTrends->addPage(pageLayer);
		

		auto sprite = Sprite::create(cache_Design.cache_filename);
		if (sprite){
			sprite->setPosition(Vec2(visiableSize.width / 2, visiableSize.height / 2 + 5));
			sprite->setScaleX((PIC_MATRIX-20)/sprite->getContentSize().width);
			sprite->setScaleY((PIC_MATRIX-20)/sprite->getContentSize().height);
			pageLayer->addChild(sprite);
		}
		
	}
	browseTrends->scrollToPage(nCurPage);
	if(nCurPage==0)
		SetButtonEnabled(buttonLeft,false);
}

void BrowseLayer::TrendsEvent(Ref * pSender, PageView::EventType type)
{
	BrowsePage * pageView = static_cast<BrowsePage *>(pSender);
	ssize_t	nCurIndex = pageView->getCurPageIndex();
	int	nCurItem = pageView->getPage(nCurIndex)->getTag();
	int	nCurPos;
	std::stringstream ss;

	buttonLike->setSelectedState(false);

	ss<<HttpConnection::v_design_time_info.at(nCurIndex)._object7th;
	ss>>m_LikeFlag;

	//string filePath = FileUtils::getInstance()->getWritablePath();
	string fileName = TEMP_filePath + "Design_Temp_Time.xml";
	//CollageTemplateList *pCTL;	//product list from xml
	char filenamebuffer[128], sUrl[128];
	sctPicList	fileInfo;

	m_fRatio = float(nCurIndex) / (HttpConnection::v_design_time_info.size()-1);
	m_Slider->setPercent((int)(m_fRatio*100));
	/*for(nCurPage=0;nCurPage<HttpConnection::v_design_time_info.size();nCurPage++)
		if(HttpConnection::v_design_time_info.at(nCurPage).id==nCurItem)
			break;*/
	if(nCurIndex>nCurPage)
	{
		nCurPage++;
		SetButtonEnabled(buttonLeft,true);
	}
	else if(nCurIndex<nCurPage)
	{
		nCurPage--;
		SetButtonEnabled(buttonRight,true);
	}
	else
	{
		if(m_LikeFlag>0)
			buttonLike->setSelectedState(true);
		else
			buttonLike->setSelectedState(false);
		textShare->setText(HttpConnection::v_design_time_info.at(nCurPage)._object8th);
		textLike->setText(HttpConnection::v_design_time_info.at(nCurPage)._object6th);
		label_Designer->setString(HttpConnection::v_design_time_info.at(nCurPage)._object5th);
		label_Title->setString(HttpConnection::v_design_time_info.at(nCurPage)._objecttitle);
		label_Description->setString(HttpConnection::v_design_time_info.at(nCurPage)._object4th);
		return;
	}

	switch (type)
	{
		case PageView::EventType::TURNING:
			if(m_LikeFlag>0)
				buttonLike->setSelectedState(true);
			else
				buttonLike->setSelectedState(false);
			textShare->setText(HttpConnection::v_design_time_info.at(nCurPage)._object8th);
			textLike->setText(HttpConnection::v_design_time_info.at(nCurPage)._object6th);
			label_Designer->setString(HttpConnection::v_design_time_info.at(nCurPage)._object5th);
			label_Title->setString(HttpConnection::v_design_time_info.at(nCurPage)._objecttitle);
			label_Description->setString(HttpConnection::v_design_time_info.at(nCurPage)._object4th);
log("current:[page %ld],[ID %s]",nCurIndex + 1,HttpConnection::v_design_time_info.at(nCurPage).uid);

			if(nCurPage>0 && nCurPage<HttpConnection::v_design_time_info.size()-1)	
				return;

			/*pCTL=CollageTemplateList::ParseXMLWithfile(fileName.c_str());
			if(!pCTL) 
			{
				log("%s file format error",fileName.c_str());
				//exit(0);
				return;
			}*/

			if(nCurIndex==pageView->getPages().size()-1)
			{
				
				if(HttpConnection::v_design_time_info.at(nCurPage).id<pCTL->vTemplate.back().id)
				{
					int	i=1;
					nCurPos=HttpConnection::v_design_time_info.back().id;

					while((nCurPos+i)<pCTL->vTemplate.size())
					{
						std::sprintf(filenamebuffer,"$D_%s",strrchr(pCTL->vTemplate.at(nCurPos+i).strListImage,'/')+1);
						fileInfo.cache_filename= TEMP_filePath+Cache_Path +filenamebuffer;
						ifstream ifs(fileInfo.cache_filename);
						if(!ifs)
						{	
							strcpy(sUrl,Design_URL);
							strcat(sUrl,pCTL->vTemplate.at(nCurPos+i).strListImage);
							if(http_connect->CurlReq(sUrl,fileInfo.cache_filename.c_str())!=CURLE_OK)
							{
								//download failed
								i++;
								log("%s download error",sUrl);
								continue;
							}
						}

						nCurPos+=i;
						i=0;	//set i=0 for if(!i)

						fileInfo.id=pCTL->vTemplate.at(nCurPos).id;

						std::sprintf(fileInfo.uid,"D_%s",pCTL->vTemplate.at(nCurPos).uid);
						fileInfo._filename=pCTL->vTemplate.at(nCurPos).strListImage;
						fileInfo._objecttitle=pCTL->vTemplate.at(nCurPos).sTitle;
						fileInfo._object4th=pCTL->vTemplate.at(nCurPos).sFourthEle;		//inspiration
						fileInfo._object5th=pCTL->vTemplate.at(nCurPos).sFifthEle;		//username
						fileInfo._object6th=pCTL->vTemplate.at(nCurPos).sSixthEle;		//city
						fileInfo._object7th=pCTL->vTemplate.at(nCurPos).sSeventhEle;		//country
						fileInfo._object8th=pCTL->vTemplate.at(nCurPos).sEighthEle;		//sharecount
						fileInfo._Cat = kPTypeNone;

						HttpConnection::v_design_time_info.push_back(fileInfo);
						
						break;
					}
					if(!i)
					{
log("append new page:%s to end, delete the first page",filenamebuffer);
						
						
						//int	nHead=pageView->getPages().size()-MAX_CacheFile;

						visiableSize = pageView->getSize();
						auto pageLayer=Layout::create();
						pageLayer->setContentSize(Size(visiableSize.width,visiableSize.height));
						pageLayer->setTag(fileInfo.id);

						//auto img = ImageView::create("mascot.png");
						auto img = Sprite::create(fileInfo.cache_filename);
						if (img){
							img->setPosition(Vec2(visiableSize.width / 2, visiableSize.height / 2 + 5));
							img->setScaleX((PIC_MATRIX-20)/img->getContentSize().width);
							img->setScaleY((PIC_MATRIX-20)/img->getContentSize().height);
							pageLayer->addChild(img);
						}

						pageView->addPage(pageLayer);
					
						//pageView->update(nCurIndex);
						//pageView->removePage(pageView->getPages().at(0));
						if(pageView->getPages().size()>MAX_CacheFile)
						{
							pageView->removePageAtIndex(0);
							HttpConnection::v_design_time_info.erase(HttpConnection::v_design_time_info.begin());
							pageView->scrollToPage(nCurPage-1);
						}

					}
					else
					{
						log("no item could be loaded!");
					}
					
				}
				else
				{
					log("end at %d[%d]!",HttpConnection::v_design_time_info.at(nCurPage).id,pCTL->vTemplate.back().id);
					SetButtonEnabled(buttonRight,false);
				}
			}
			else if (nCurIndex==0)
			{
				if(HttpConnection::v_design_time_info.at(nCurPage).id>pCTL->vTemplate.begin()->id)
				{
					nCurPos=HttpConnection::v_design_time_info.begin()->id;
					int	i=1;

					while((nCurPos-i)>=0)
					{
						std::sprintf(filenamebuffer,"$D_%s",strrchr(pCTL->vTemplate.at(nCurPos-i).strListImage,'/')+1);
						fileInfo.cache_filename= TEMP_filePath+Cache_Path +filenamebuffer;
						ifstream ifs(fileInfo.cache_filename);
						if(!ifs)
						{	
							strcpy(sUrl,Design_URL);
							strcat(sUrl,pCTL->vTemplate.at(nCurPos-i).strListImage);
							if(http_connect->CurlReq(sUrl,fileInfo.cache_filename.c_str())!=CURLE_OK)
							{
								//download failed
								i++;
								log("%s download error",sUrl);
								continue;
							}
						}

						nCurPos-=i;
						i=0;	//set i=0 for if(!i)

						fileInfo.id=pCTL->vTemplate.at(nCurPos).id;

						std::sprintf(fileInfo.uid,"D_%s",pCTL->vTemplate.at(nCurPos).uid);
						fileInfo._filename=pCTL->vTemplate.at(nCurPos).strListImage;
						fileInfo._objecttitle=pCTL->vTemplate.at(nCurPos).sTitle;
						fileInfo._object4th=pCTL->vTemplate.at(nCurPos).sFourthEle;		//inspiration
						fileInfo._object5th=pCTL->vTemplate.at(nCurPos).sFifthEle;		//username
						fileInfo._object6th=pCTL->vTemplate.at(nCurPos).sSixthEle;		//city
						fileInfo._object7th=pCTL->vTemplate.at(nCurPos).sSeventhEle;		//country
						fileInfo._object8th=pCTL->vTemplate.at(nCurPos).sEighthEle;		//sharecount
						fileInfo._Cat = kPTypeNone;

						HttpConnection::v_design_time_info.insert(HttpConnection::v_design_time_info.begin(),fileInfo);
						
						break;
					}
					if(!i)
					{
log("insert new page:%s to head, delete the last page",filenamebuffer);
						
						visiableSize = pageView->getSize();
						auto pageLayer=Layout::create();
						pageLayer->setContentSize(Size(visiableSize.width,visiableSize.height));
						pageLayer->setTag(fileInfo.id);

						auto img = Sprite::create(fileInfo.cache_filename);
						if (img){
							img->setPosition(Vec2(visiableSize.width / 2, visiableSize.height / 2 + 5));
							img->setScaleX((PIC_MATRIX-20)/img->getContentSize().width);
							img->setScaleY((PIC_MATRIX-20)/img->getContentSize().height);
							pageLayer->addChild(img);
						}

						pageView->insertPage(pageLayer,0);
						if(pageView->getPages().size()>MAX_CacheFile)
						{
							pageView->removePage(pageView->getPages().back());
							HttpConnection::v_design_time_info.pop_back();
							pageView->scrollToPage(nCurPage+1);
						}

					}
					else
					{
						log("no item could be loaded!");
					}
				}
				else
				{
					log("head at %d[%d]!",HttpConnection::v_design_time_info.at(nCurPage).id,pCTL->vTemplate.begin()->id);
					SetButtonEnabled(buttonLeft,false);
				}
			}
			//delete pCTL;
			break;

		
		default:
			break;
	}
}


void BrowseLayer::touchLike(Ref* object,CheckBoxEventType type)
{
	char url[128],param[128],buffer[128];
	char * userID = "eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8";

	int nLike;
	std::stringstream ss;

	strcpy(url,Req_URL);

	//AppDelegate *app = (AppDelegate*)Application::getInstance();
	//G_UID = app->GetUserGuid();
	if(G_UID.empty())		// for testing when un_login
		G_UID=userID;

	std::sprintf(param,"&user_id=%s&design_id=%s",G_UID.c_str(),HttpConnection::v_design_time_info.at(nCurPage).uid+2);

	switch (type)  
    {  
		case CHECKBOX_STATE_EVENT_SELECTED:  
			//选中时响应  
			strcat(url,Post_Like);
			strcat(url,param);

			http_connect->CurlPost(url,buffer,false);
			if(atoi(HttpConnection::res_buf)!=0)
			{
				log("like point:missed server return");
				return;
			}
			HttpConnection::v_design_time_info.at(nCurPage)._object7th="1";
			ss<<HttpConnection::v_design_time_info.at(nCurPage)._object6th;
			ss>>nLike;
			nLike++;

			break;  
		case CHECKBOX_STATE_EVENT_UNSELECTED:  
			//未选中时响应  
			strcat(url,Post_Unlike);
			strcat(url,param);

			http_connect->CurlPost(url,buffer,false);
			if(atoi(HttpConnection::res_buf)!=0)
			{
				log("like point:missed server return");
				return;
			}
			HttpConnection::v_design_time_info.at(nCurPage)._object7th="0";
			ss<<HttpConnection::v_design_time_info.at(nCurPage)._object6th;
			ss>>nLike;
			nLike--;

			break;  
		default:  
			break;  
	}
	
	std::sprintf(buffer,"%d",nLike);
	HttpConnection::v_design_time_info.at(nCurPage)._object6th=buffer;
	textLike->setText(HttpConnection::v_design_time_info.at(nCurPage)._object6th);
	
	auto id = HttpConnection::v_design_time_info.at(nCurPage).id;
	strcpy(pCTL->vTemplate.at(id).sSixthEle,buffer);
	strcpy(pCTL->vTemplate.at(id).sSeventhEle,HttpConnection::v_design_time_info.at(nCurPage)._object7th.c_str());
	
	auto pDB = CollageTemplateList::initDB("Collage.db3");
	char sSQL[256],*err;
	std::sprintf(sSQL,"update CollageDesigns set like_count = '%s', personal_flag = '%s' where id = '%s'",
		HttpConnection::v_design_time_info.at(nCurPage)._object6th.c_str(),HttpConnection::v_design_time_info.at(nCurPage)._object7th.c_str(),
		HttpConnection::v_design_time_info.at(nCurPage).uid+2);
	
	if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
	{
	   //sqlite3_close(pDB);
	   log("update table[designs] failure.");
	}
	sqlite3_close(pDB);
}

void BrowseLayer::touchShare(Ref* object,cocos2d::ui::Widget::TouchEventType type)
{
	char url[128],buffer[16];
	std::stringstream ss;
	int nShare;

	ss<<HttpConnection::v_design_time_info.at(nCurPage)._object8th;
	ss>>nShare;

	switch (type)  
    {  
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			std::sprintf(url,"%s%s",COllage_Share,HttpConnection::v_design_time_info.at(nCurPage).uid+2);
			Application::getInstance()->openURL(url);
			//nShare++;
			dynamic_cast<AppDelegate*>(Application::getInstance())->updateCollageScore(UPDATE_SCORE_SHARE);
			break;
		default:  
			break;  
    }
	
	std::sprintf(buffer,"%d",nShare);
	HttpConnection::v_design_time_info.at(nCurPage)._object8th=buffer;
	textShare->setText(HttpConnection::v_design_time_info.at(nCurPage)._object8th);

	/*auto pDB = CollageTemplateList::initDB("Collage.db3");
	char sSQL[256],*err;
	std::sprintf(sSQL,"update CollageDesigns set share_count = '%s' where id = '%s'",
		HttpConnection::v_design_time_info.at(nCurPage)._object8th.c_str(),HttpConnection::v_design_time_info.at(nCurPage).uid+2);
	
	if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
	{
	   //sqlite3_close(pDB);
	   log("update table[designs] failure.");
	}
	sqlite3_close(pDB);*/
}

void BrowseLayer::touchLeft(Ref* object,cocos2d::ui::Widget::TouchEventType type)
{
	auto p = static_cast<BrowsePage*>(dynamic_cast<Node*>(object)->getParent()->getChildByName("PageView_Product"));

	switch (type)  
    {  
		//case TouchEventType::TOUCH_EVENT_ENDED:
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			if(nCurPage>0)
				p->scrollToPage(nCurPage-1);

			p->setTouchDirection(PageView::TouchDirection::RIGHT);
			
			break;
		default:  
			break;  
    }
}

void BrowseLayer::touchRight(Ref* object,cocos2d::ui::Widget::TouchEventType type)
{
	auto p = static_cast<BrowsePage*>(dynamic_cast<Node*>(object)->getParent()->getChildByName("PageView_Product"));

	switch (type)  
    {  
		//case TouchEventType::TOUCH_EVENT_ENDED:
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			p->scrollToPage(nCurPage+1);
			p->setTouchDirection(PageView::TouchDirection::LEFT);
			
			break;
		default:  
			break;  
    }
}

void BrowseLayer::sliderEvent(Ref * pSender, Slider::EventType type)
{
	auto p = static_cast<BrowsePage*>(dynamic_cast<Node*>(pSender)->getParent()->getChildByName("PageView_Product"));
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider * slider = dynamic_cast<Slider *>(pSender);
		int percent = slider->getPercent();
		nCurPage = (HttpConnection::v_design_time_info.size()-1)*percent/100;
		//log("%d%%,%d",percent,nCurPage);
		if(nCurPage==0)
			nCurPage+=1;
		else if(nCurPage==MAX_CacheFile-1)
			nCurPage-=1;
		p->scrollToPage(nCurPage);
		TrendsEvent(p, PageView::EventType::TURNING);
	}
}

void BrowseLayer::RightBarCallBack(cocos2d::Ref *pSender, Widget::TouchEventType event)
{
	auto rBar = dynamic_cast<ImageView*>(pSender);

	if(event==Widget::TouchEventType::ENDED || event==Widget::TouchEventType::CANCELED)
	{
		auto e = rBar->getTouchEndPosition();
		auto b = rBar->getTouchBeganPosition();
		if(b.getDistance(e)>PIC_MATRIX/20 && abs(b.x-e.x)>abs(b.y-e.y))
		{
			if(b.x>e.x)
				touchRight(pSender,cocos2d::ui::Widget::TouchEventType::ENDED);
			else
				touchLeft(pSender,cocos2d::ui::Widget::TouchEventType::ENDED);
		}

	}

}