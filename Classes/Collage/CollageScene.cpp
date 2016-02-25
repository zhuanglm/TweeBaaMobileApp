//===========Tweebaa==============
//CollageScenen.cpp	--Jun 23 2015	
//Version 1.1 by Raymond Zhuang at Toronto
// last modified at Nov 17

#include "CollageScene.h"
#include "cocostudio/CocoStudio.h"
#include "ChooseLayer.h"
#include "ui/CocosGUI.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "unistd.h"
#endif
#include "AppDelegate.h"


USING_NS_CC;

using namespace cocostudio::timeline;

extern int m_iCurrentGameID ;

Scene* CollageScene::createScene()
{
    //_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	// 'scene' is an autorelease object
	auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = CollageScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CollageScene::init()
{
    
	
	string filePath = FileUtils::getInstance()->getWritablePath();
	TEMP_filePath = filePath;
	filePath += Cache_Path;
	if(access(filePath.c_str(),0)<0)
	{
		if(!CreateFolder(filePath.c_str()))
		{
			MessageBox("Can not create Cache Folder","Device ERROR");
			return false;
		}
	}
	//////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto loadingUI = CSLoader::createNode("Collage/LoadingScene.csb");
	addChild(loadingUI);

	textPercentage = dynamic_cast<Text*>(loadingUI->getChildByName("Text_Percentage"));
	textPercentage->setText("0%");
	auto Image_bar = Sprite::create("Collage/bar.png");
	
	ct= ProgressTimer::create(Image_bar);
	ct->setAnchorPoint(Vec2::ZERO);
	ct->setPosition(Vec2(319,317));
	
	ct->setType(ProgressTimer::Type::BAR);
	ct->setMidpoint(Vec2(0,0));
	ct->setBarChangeRate(Vec2(1,0));
	ct->setPercentage(0);
	addChild(ct);

	std::thread t1(&CollageScene::LoadResources,this);
	t1.detach();

	Director::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&CollageScene::loadingCallBack), this, .5f, false);

	////////////////////////////////loading coloar pattern//////////
	sctColorList ColorPattern;
	
	ColorPattern.R=255;
	ColorPattern.G=255;
	ColorPattern.B=255;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=255;
	ColorPattern.G=245;
	ColorPattern.B=114;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=255;
	ColorPattern.G=206;
	ColorPattern.B=81;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=247;
	ColorPattern.G=147;
	ColorPattern.B=78;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=240;
	ColorPattern.G=83;
	ColorPattern.B=63;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=226;
	ColorPattern.G=0;
	ColorPattern.B=137;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=158;
	ColorPattern.G=61;
	ColorPattern.B=150;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=182;
	ColorPattern.G=189;
	ColorPattern.B=224;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=0;
	ColorPattern.G=181;
	ColorPattern.B=239;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=0;
	ColorPattern.G=108;
	ColorPattern.B=163;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=210;
	ColorPattern.G=235;
	ColorPattern.B=226;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=109;
	ColorPattern.G=196;
	ColorPattern.B=188;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=88;
	ColorPattern.G=185;
	ColorPattern.B=71;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=0;
	ColorPattern.G=115;
	ColorPattern.B=102;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=224;
	ColorPattern.G=193;
	ColorPattern.B=169;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=190;
	ColorPattern.G=135;
	ColorPattern.B=105;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=142;
	ColorPattern.G=83;
	ColorPattern.B=42;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=223;
	ColorPattern.G=225;
	ColorPattern.B=223;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=121;
	ColorPattern.G=125;
	ColorPattern.B=130;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=52;
	ColorPattern.G=55;
	ColorPattern.B=65;
	HttpConnection::v_color_list.push_back(ColorPattern);

	ColorPattern.R=0;
	ColorPattern.G=0;
	ColorPattern.B=0;
	HttpConnection::v_color_list.push_back(ColorPattern);
	HttpConnection::numberOfLoadSprites++;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if ( app->getBGMstatus() == BGMusic_flag::ON){
		app->StartBGMusic(BGM_COLLAGE);
	}

	return true;
}

CollageScene::CollageScene(void)
{
	Number_Resources=Static_Resources;	// xml files + color pattern
	http_connect = new HttpConnection();
	
	nErrorFlag = 0;
	
	nTime=0;
}

CollageScene::~CollageScene(void)
{
	//delete mainMenu;
	CC_SAFE_DELETE(http_connect);
	//HttpConnection::v_design_like_info.clear();
	/*vector<sctPicList>().swap(HttpConnection::v_decoration_info);
	vector<sctPicList>().swap(HttpConnection::v_product_info);
	vector<sctPicList>().swap(HttpConnection::v_design_like_info);
	vector<sctPicList>().swap(HttpConnection::v_design_time_info);
	vector<string>().swap( HttpConnection::v_template_cache_filename);*/

}

void CollageScene::loadingTutorial(float dt)
{
	m_iCurrentGameID=-1;
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}
}

void CollageScene::loadingCallBack(Ref* pSender)
{
    char tmp[10],code[10];
    float value = ((float)HttpConnection::numberOfLoadSprites / Number_Resources)*100;   //已加载的百分比
    sprintf(tmp, "%d%%", (int)(value));
//log("%d%%[%d]of[%d]", (int)(value),HttpConnection::numberOfLoadSprites,Number_Resources);
    textPercentage->setString(tmp);
     
    ct->setPercentage(value);
   
	m_iCurrentGameID=-1;
	

	/*if( value>=10 && !bTutorialFlag )
	{
		//auto Tutorial_Scene = TutorialScene::create();
		auto Tutorial_Scene = MainMenu::createScene();

		bTutorialFlag = true;
		
		Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&CollageScene::loadingCallBack), this);
		Director::getInstance()->pushScene(TransitionFade::create(1, TutorialScene::create()));
		//Director::getInstance()->pushScene(transition);

	}*/
	
	
	if(nErrorFlag)
	{
		nTime++;
		if(nTime<=4)	//avoid popup quit
			return;
		Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&CollageScene::loadingCallBack), this);
		HttpConnection::numberOfLoadSprites=0;	//reset
		sprintf(code," : [%d]",nErrorFlag);
		sErrorMsg+=code;
		popupLayer(Director::getInstance()->getRunningScene(),Vec2::ZERO,"NETWORK ERROR",sErrorMsg);
	}
	
	//加载完成，切换场景
    if (HttpConnection::numberOfLoadSprites >= Number_Resources || value>=100) 
	{
		
		if(!HttpConnection::bTutorialFlag)
			nTime++;
		if(nTime>=3 && !HttpConnection::bTutorialFlag && !HttpConnection::bTutorialQuitFlag)		//timer
		{
			string keyvalue = DataAccess::getParamValue("Tutorial_IsFirst_Collage","\0");
			if(keyvalue.length()>0)//读到
			{
			 //不放tutorial;
				HttpConnection::bTutorialQuitFlag = true;
			}
			else
			{
				DataAccess::setParamValue("Tutorial_IsFirst_Collage","1");
				Tutorial_Scene = TutorialScene::create();
				Director::getInstance()->pushScene(TransitionFade::create(1, Tutorial_Scene));
			
			}
			HttpConnection::bTutorialFlag = true;
		
		}
		//tutorial has been run one time
		if(nTime>=3 && HttpConnection::bTutorialQuitFlag)
		{
			Director::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&CollageScene::loadingCallBack), this);
			HttpConnection::numberOfLoadSprites=0;	//reset

			Director::getInstance()->replaceScene(TransitionPageTurn::create(1,MainMenu::createScene(),false));
			
		}
    }
}

void CollageScene::LoadResources(void)
{
	if(!updateMutex.try_lock())
		return;
	
	//updateMutex.lock();
	
	char filenamebuffer[80], sUrl[256],sSQL[512], *err;
	string fileName;
	sctPicList	fileInfo;
	int	i;
	
	//TEMP_filePath = FileUtils::getInstance()->getWritablePath();
	pDB = CollageTemplateList::initDB("Collage.db3");
	sqlite3_busy_timeout(pDB,1000);
	
	////////////////////////////////loading design//////////
	std::strcpy(sUrl,Req_URL);
	std::strcat(sUrl,Design_Total);
	fileName = TEMP_filePath + "Design_Total.xml";

	auto nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
	if(nReturn!=CURLE_OK)
	{
		log("get (%s) file from %s error",fileName.c_str(),sUrl);
		nErrorFlag=nReturn;
		sErrorMsg=MSG_ERROR_NETWORK;
		sErrorMsg+="Download Designes quantity";
		updateMutex.unlock();
		return;
		//MessageBox("Can not download Like_list","Server ERROR");
		//exit(0);
	}
	HttpConnection::numberOfLoadSprites++;

	auto sNum = CollageTemplateList::GetElementFromXML(fileName.c_str(),"Total");
	std::stringstream ss;int nNum;
	ss<<sNum;
	ss>>nNum;
	strcpy(sSQL,"select * from CollageDesigns");
	char **re;
	int r,c;
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)  
	{
           //sqlite3_close(pDB);
           log("count table[designs] failure.");
		   r=0;
	}
	
	if(nNum!=r)
	{
	
		std::strcpy(sUrl,Req_URL);
		std::strcat(sUrl,Design_Template_Time);

		fileName = TEMP_filePath + "Design_Temp_Time.xml";
		//ifstream ifc(fileName);
		//if(!ifc)
		nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
		if(nReturn!=CURLE_OK)
		{
			log("get (%s) file from %s error",fileName.c_str(),sUrl);
			nErrorFlag=nReturn;
			sErrorMsg=MSG_ERROR_NETWORK;
			sErrorMsg+="Download Designes List";
			updateMutex.unlock();
			return;
			//MessageBox("Can not download Like_list","Server ERROR");
			//exit(0);
		}
		HttpConnection::numberOfLoadSprites++;

		//loading image from template
		pCTL=CollageTemplateList::ParseXMLWithfile(fileName.c_str());
		if(!pCTL) 
		{
			//log("%s file format error",fileName.c_str());
			sprintf(filenamebuffer,"%s file format error",fileName.c_str());
			nErrorFlag=99;
			sErrorMsg=filenamebuffer;
			updateMutex.unlock();
			return;
		}

		strcpy(sSQL,"delete from CollageDesigns");
		if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
		{
			   //sqlite3_close(pDB);
			   log("truncate table[designs] failure.");
		}

		for(auto vT: pCTL->vTemplate )
		{
			auto lt = time(NULL);
			auto cdate = ctime(&lt);
			sprintf(sSQL,"insert into CollageDesigns (id,cdate,image,title,inspiration,user,like_count,personal_flag,share_count) values ('%s','%s','%s','%s','%s','%s','%s','0','%s')"
				,vT.uid,cdate,vT.strListImage,vT.sTitle,vT.sFourthEle,vT.sFifthEle,vT.sSixthEle,vT.sEighthEle);
//log("insert design %s ",sSQL);
			if (sqlite3_exec(pDB,sSQL,NULL,NULL,&err) != SQLITE_OK) 
			{
				   //sqlite3_close(pDB);
				log("%s to table[designs] failure:%s",sSQL,err);
			}
		}
		CC_SAFE_DELETE(pCTL);
	}
	else
	{
		Number_Resources--;
	}
	sqlite3_free_table(re);
	sqlite3_close(pDB);

	pCTL = CollageTemplateList::LoadXMLFromDB("Collage.db3","CollageDesigns","share_count");	//set v_design_time_info by order of share_count
	if(!pCTL) 
	{
		sprintf(filenamebuffer,"load XML from table[designs] failure.");
		nErrorFlag=99;
		sErrorMsg=filenamebuffer;
		updateMutex.unlock();
		return;
	}
		
	Number_Resources+=pCTL->vTemplate.size()>MAX_CacheFile?MAX_CacheFile:pCTL->vTemplate.size();

	i=0;
	for(auto vT: pCTL->vTemplate )
	{
		i++;

		if(i>=MAX_CacheFile+1)	break;				//装载的缓冲文件个数
		
		if(!strcmp(vT.strListImage,"None"))
		{
			i--;
			continue;
		}

		sprintf(filenamebuffer,"$D_%s",strrchr(vT.strListImage,'/')+1);
		
		fileInfo.cache_filename= TEMP_filePath+Cache_Path +filenamebuffer;
		sprintf(fileInfo.uid,"D_%s",vT.uid);
		fileInfo._filename=vT.strListImage;
		fileInfo._objecttitle=vT.sTitle;
		fileInfo._object4th=vT.sFourthEle;		//inspiration
		fileInfo._object5th=vT.sFifthEle;		//username
		fileInfo._object6th=vT.sSixthEle;		//city
		fileInfo._object7th=vT.sSeventhEle;		//country
		fileInfo._object8th=vT.sEighthEle;		//sharecount
		fileInfo._Cat = kPTypeNone;

		fileInfo.id=vT.id;

		HttpConnection::v_design_time_info.push_back(fileInfo);	//save info for BrowseLayer

		
		//判断文件是否存在
		ifstream ifs(fileInfo.cache_filename);
		if(!ifs)
		{	
			std::strcpy(sUrl,Design_URL);
			std::strcat(sUrl,vT.strListImage);
			http_connect->HttpReqToCache(sUrl,vT.strListImage,fileInfo.cache_filename.c_str());
		}
		else
		{
			HttpConnection::numberOfLoadSprites++;
		}

	}
	CC_SAFE_DELETE(pCTL);

	////////////////////////////////loading published design//////////
	std::strcpy(sUrl,Req_URL);
	fileName = TEMP_filePath + "Design_Like.xml";

	//G_UNM = UserDefault::getInstance()->getStringForKey("tweebaa_username");
	AppDelegate *app = dynamic_cast<AppDelegate*>(Application::getInstance());
	_UserInfo userInfo = DataAccess::getUserInfoByID(m_UserID);
	G_UID = userInfo.strUserGuid;
	G_UNM = app->base64_xxtea_decode(userInfo.strUserName);

//G_UNM = "dragon2934";
//G_UID = "eff4dca6-fedc-4bd9-bbe5-79dec2bb39d8";
	if(G_UID.empty())
	{
		//HttpConnection::numberOfLoadSprites++;
		HttpConnection::v_design_like_info.clear();
		std::remove(fileName.c_str());
	}
	else
	{
		//Number_Resources--;					//re-caculate with init number temporarily
		pCTL = CollageTemplateList::LoadXMLFromDB("Collage.db3","CollageDesigns","user",G_UNM.c_str());
		if(pCTL)
		{
			Number_Resources+=pCTL->vTemplate.size()>MAX_CacheFile?MAX_CacheFile:pCTL->vTemplate.size();

			i=0;
			for(auto vT: pCTL->vTemplate )
			{
				i++;
				if(i==MAX_CacheFile+1)	break;				//装载的缓冲文件个数

				if(!strcmp(vT.strListImage,"None"))
				{
					i--;
					continue;
				}

				sprintf(filenamebuffer,"$D_%s",strrchr(vT.strListImage,'/')+1);
		
				fileInfo.cache_filename= TEMP_filePath+Cache_Path +filenamebuffer;
				sprintf(fileInfo.uid,"D_%s",vT.uid);
				fileInfo._filename=vT.strListImage;
				fileInfo._objecttitle=vT.sTitle;
				fileInfo._object4th=vT.sFourthEle;		//inspiration
				fileInfo._object5th=vT.sFifthEle;		//username
				fileInfo._object6th=vT.sSixthEle;		//city
				fileInfo._object7th=vT.sSeventhEle;		//country
				fileInfo._object8th=vT.sEighthEle;		//sharecount
				fileInfo._Cat = kPTypeNone;

				fileInfo.id=vT.id;

				HttpConnection::v_design_like_info.push_back(fileInfo);	//save info for FavoritLayer

		
				//判断文件是否存在
				ifstream ifs(fileInfo.cache_filename);
				if(!ifs)
				{	
					std::strcpy(sUrl,Design_URL);
					std::strcat(sUrl,vT.strListImage);
					http_connect->HttpReqToCache(sUrl,vT.strListImage,fileInfo.cache_filename.c_str());
				}
				else
				{
					HttpConnection::numberOfLoadSprites++;
				}

			}
		}
		else
		{
			HttpConnection::v_design_like_info.clear();
			std::remove(fileName.c_str());
		}
		CC_SAFE_DELETE(pCTL);
	}

	////////////////////////////////loading Background_List//////////
	std::strcpy(sUrl,Req_URL);
	std::strcat(sUrl,Background_List);

	fileName = TEMP_filePath + "Background_List.xml";
	nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
	if(nReturn!=CURLE_OK)
	{
		log("get (%s) file from %s error",fileName.c_str(),sUrl);
		nErrorFlag=nReturn;
		sErrorMsg=MSG_ERROR_NETWORK;
		sErrorMsg+="Download Background List";
		updateMutex.unlock();
		return;
	}

	HttpConnection::numberOfLoadSprites++;
	
	////////////////////////////////loading Event_List//////////
	std::strcpy(sUrl,Req_URL);
	std::strcat(sUrl,Event_List);

	fileName = TEMP_filePath + "Event_List.xml";
	nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
	if(nReturn!=CURLE_OK)
	{
		log("get (%s) file from %s error",fileName.c_str(),sUrl);
		nErrorFlag=nReturn;
		sErrorMsg=MSG_ERROR_NETWORK;
		sErrorMsg+="Download Event List";
		updateMutex.unlock();
		return;
	}

	HttpConnection::numberOfLoadSprites++;
	////////////////////////////////loading decoration//////////
	std::strcpy(sUrl,Req_URL);
	std::strcat(sUrl,Decoration_List);

	fileName = TEMP_filePath + "Decoration_List.xml";
	nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
	if(nReturn!=CURLE_OK)
	{
		log("get (%s) file from %s error",fileName.c_str(),sUrl);
		nErrorFlag=nReturn;
		sErrorMsg=MSG_ERROR_NETWORK;
		sErrorMsg+="Download Decoration List";
		updateMutex.unlock();
		return;
		//MessageBox("Can not download Like_list","Server ERROR");
		//exit(0);
	}

	HttpConnection::numberOfLoadSprites++;

	////////////////////////////////loading articles//////////
	std::strcpy(sUrl,Req_URL);
	std::strcat(sUrl,Article_List);

	fileName = TEMP_filePath + "Articles_List.xml";
	nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
	if(nReturn!=CURLE_OK)
	{
		log("get (%s) file from %s error",fileName.c_str(),sUrl);
		nErrorFlag=nReturn;
		sErrorMsg=MSG_ERROR_NETWORK;
		sErrorMsg+="Download Articles List";
		updateMutex.unlock();
		return;
		//MessageBox("Can not download Like_list","Server ERROR");
		//exit(0);
	}

	HttpConnection::numberOfLoadSprites++;

	
	////////////////////////////////loading product xml//////////
    
//log("file:%s ID:%s",HttpConnection::MainCatList[1][0].c_str(),HttpConnection::MainCatList[1][1].c_str());
    
 	for(i=0;i<Main_Categories;i++)
	{
		fileName = TEMP_filePath + HttpConnection::MainCatList[i][0] + ".xml";
		std::strcpy(sUrl,Req_URL);
		std::strcat(sUrl,Product_List);
		std::strcat(sUrl,HttpConnection::MainCatList[i][1].c_str());
		nReturn = http_connect->CurlReq(sUrl,fileName.c_str());
		if(nReturn!=CURLE_OK)
		{
			log("get (%s) file from %s error",fileName.c_str(),sUrl);
			nErrorFlag=nReturn;
			sErrorMsg=MSG_ERROR_NETWORK;
			sErrorMsg+=HttpConnection::MainCatList[i][0];
			updateMutex.unlock();
			return;
			//MessageBox("Can not download Like_list","Server ERROR");
			//exit(0);
		}
		HttpConnection::numberOfLoadSprites++;
	}
	updateMutex.unlock();
	HttpConnection::bTutorialFlag=false;
	return;
}

void CollageScene::popupLayer(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent){

    PopupLayer* pl = PopupLayer::create("popup_sq.png");
	
    pl->setContentSize(Size(450, 250));
	//auto sizeLayer = baseLayer->getContentSize();

	pl->setAnchorPoint(Vec2(.5,.5));
	pl->setPosition(popPos.x,popPos.y);
	pl->setTitle(sTitle,Color3B(106,72,36));
	pl->setContentText(sContent.c_str(),Color3B(106,72,36), 24, 20, 150);
	
    pl->setCallbackFunc(this, callfuncND_selector(CollageScene::popupCallback),NULL);
    // 添加按钮，设置图片，文字，tag 信息
	pl->addButton("button_normal.png", "button_dis.png", "RETURN",Color3B(255,255,255), 1);
    // 添加到指定层
	baseLayer->addChild(pl);
}

void CollageScene::popupCallback(cocos2d::Node *pNode,void *data)
{
//log("button call back. tag: %d", pNode->getTag());
	auto scene = ChooseLayer::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

bool CollageScene::CreateFolder(const char *path)

{

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);

    if (ret != 0 && (errno != EEXIST))

    {

        return false;

    }

    return true;

#else

    BOOL ret = CreateDirectoryA(path, NULL);

	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())

	{

		return false;

	}

    return true;

#endif

}