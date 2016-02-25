#include "TurntableSystem.h"
#include "../ZhuanPanSystem/Zp_ImgPath.h"
#include "../ZhuanPanSystem/Zp_BoxData.h"
#include "../ZhuanPanSystem/SpinLuckyLayer.h"
#include "../ZhuanPanSystem/Gzi.h"
#include "../ZhuanPanSystem/GlobalInfo.h"
#include "../MemberCenter/RegisterView.h"
#include "../ChooseLayer.h"
#include "../utilits/DailyCheckIn.h"
#include "../utilits/DailyCheckInLayer.h"
#include "../utilits/pugixml.hpp"

Vector<DailyCheckIn *> vecUserCheckInDays;
int fnum =0;
Vector<Zp_BoxData*> boxgezi;
int iPrizeID;
float nexttime=0.0;


TurntableSystem::TurntableSystem(){};

bool TurntableSystem::init()
{
	//getHttpRequest();//get checkin xml
	nexttime = 0;
	LoadTurntableSystem();

	return true;
}
void TurntableSystem::LoadTurntableSystem(){
	Sprite* allbd = Sprite::create(IMG_ZP_BD);
	allbd->setPosition(VisibleRect::center());

	int allbd_w = 960;//allbd->getContentSize().width; //720
	int allbd_h = 640;//allbd->getContentSize().height; //500

	Sprite* cbd = Sprite::create(IMG_ZP_CENTERBD);
	cbd->setPosition(Vec2(allbd_w / 2 - 120, allbd_h / 2 - 35));

	//格子s	 
	Sprite* gezi_bd = Sprite::create(IMG_ZP_KUANGBD);
	int gezi_w = 114;//gezi_bd->getContentSize().width;  //94
	int gezi_h = 114;//gezi_bd->getContentSize().height; //94

	//创建一个矩阵格子阵
	//boxgezi =new  Vector<Zp_BoxData>;
	int bid = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j <= 5; j++)
		{

			Zp_BoxData* thisbox = new Zp_BoxData();
			thisbox->set_boxid(bid);
			thisbox->set_boxReward(getReward(bid));
			if (bid == 4)
			{
				thisbox->set_xuanzhong(true);
			}
			else
			{
				thisbox->set_xuanzhong(false);
			}
			thisbox->set_tag(bid);//not used
			switch (i)
			{
			case 0:
				thisbox->set_point(Vec2(218 + gezi_w*j, allbd_h - 179));
				boxgezi.pushBack(thisbox);
				break;
			case 1:
				if (j <= 2)
				{
					thisbox->set_point(Vec2(218 + gezi_w * 5, allbd_h - 179 - gezi_h - gezi_h*j));
					boxgezi.pushBack(thisbox);
				}
				break;
			case 2:
				thisbox->set_point(Vec2(103 + gezi_w * 5 - gezi_w*j, 119));
				boxgezi.pushBack(thisbox);
				break;
			case 3:
				if (j <= 2)
				{
					thisbox->set_point(Vec2(103, 119 + gezi_h + gezi_h*j));
					boxgezi.pushBack(thisbox);
				}
				break;
			default:
				break;
			}
			thisbox->release();
			bid++;
		}
	}
	vector<int> maua;

	for (int i = 0; i < boxgezi.size(); i++)
	{
		Zp_BoxData* bdata = (Zp_BoxData*)boxgezi.at(i);
		Gzi* gz = new Gzi(allbd, bdata);
		maua.push_back(bdata->get_boxid());
		gz->release();
	}
	GlobalInfo::getInstance()->set_gizilist(maua);

	//show Lucky 
	CCMenuItemImage *Spin_Lucky = CCMenuItemImage::create(IMG_ZP_LUCKY, IMG_ZP_LUCKY0, this, menu_selector(TurntableSystem::ShowSpinLucky));
	Spin_Lucky->setPosition(Vec2(cbd->getContentSize().width / 2 + 250, cbd->getContentSize().height / 2));
	CCMenu *menu_lucky = CCMenu::create(Spin_Lucky, NULL);
	menu_lucky->setPosition(Vec2(0, 0));
	cbd->addChild(menu_lucky, 0, TAG_ZP_LUCKY);

	AppDelegate* app = (AppDelegate*)Application::getInstance();
	NetworkType ntType = app->GetNetWorkStatus();
	if (ntType == NetworkTypeNone)
	{
		Spin_Lucky->setEnabled(false);
	}

	//Start Spining
	CCMenuItemImage *item1 = CCMenuItemImage::create(IMG_ZP_START, IMG_ZP_START0, this, menu_selector(TurntableSystem::runPlay));
	item1->setPosition(Vec2(cbd->getContentSize().width / 2, cbd->getContentSize().height / 2));
	CCMenu *menu = CCMenu::create(item1, NULL);
	menu->setPosition(Vec2(0, 0));
	cbd->addChild(menu, 0, TAG_ZP_PLAY);
	allbd->addChild(cbd, 2, TAG_ZP_ZHONGJIAN);
	this->addChild(allbd, 3, TAG_ZP_BD);

	Sprite* imgBackground = Sprite::create(TUENTABLE_BACKGROUND);
	imgBackground->setAnchorPoint(Vec2(0, 0));
	imgBackground->setPosition(Vec2(0, 0));
	this->addChild(imgBackground, -1, 222);

}
TurntableSystem::~TurntableSystem(void)
{
	boxgezi.clear();
}

Node* TurntableSystem::getChildByTagRecursively(int nodeTag,cocos2d::Node* parent)
{
	 Node* nodeFound = parent->getChildByTag(nodeTag);
    // auto nodeFound = dynamic_cast<T>(aNode);
    if (!nodeFound) {
        auto children = parent->getChildren();
        for (auto child : children)
        {
            nodeFound = getChildByTagRecursively(nodeTag, child);
            if (nodeFound) break;
        }
    }
    return nodeFound;
}

void TurntableSystem::ShowSpinLucky(Ref* pSender)
{
	/*AppDelegate *app = (AppDelegate*)Application::getInstance();
	app->PlayEffect(SOUND_BUTTON_CLICK);
	auto scene = SpinLuckyLayer::createScene();
	if (scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}*/
	Application::getInstance()->openURL("https://www.tweebaa.com/SpinLucky.aspx");
}

void TurntableSystem::runPlay(CCObject* pSender)
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_BUTTON_CLICK);
	}
	CCMenu *menu =(CCMenu *)this->getChildByTagRecursively(TAG_ZP_PLAY,this);
	if(menu)
		menu->setEnabled(false);
	runnum = -1; 
	srand(time(NULL));
	//create a random number

	srand((unsigned)time(0));//
	int random = rand();

	fnum = 4;//start from tweebot
	
	int nRand = rand() % 20000;
	if (nRand == 19999)//Tweebot
	{
		//StopNum = 36;
		StopNum = 22;//100 points
	}
	else if (nRand==19998)//Wonder Cap
	{
		//StopNum = 25;
		StopNum = 22;//100 points
	}
	else if (nRand >= 19996)//10TweeBucks
	{
		StopNum = 31;
	}
	else if (nRand >= 19990)//5TweeBuck
	{
		StopNum = 27;
	}
	else if (nRand >= 19960)//100 points
	{
		StopNum = 22;
	}
	else if (nRand == 19930)//100 points
	{
		StopNum = 29;
	}
	else if (nRand >= 19700)//50points
	{
		StopNum = 17;
	}
	else if (nRand >= 19500)//50points
	{
		StopNum = 24;
	}
	else if (nRand >= 18000)//10points
	{
		StopNum = 16;
	}
	else if (nRand >= 16500)//10points
	{
		StopNum = 19;
	}
	else if (nRand >= 14500)//1points
	{
		StopNum = 15;
	}
	else if (nRand >= 12500)//1points
	{
		StopNum = 26;
	}
	else if (nRand >= 10500)//1Tips
	{
		StopNum = 32;
	}
	else if (nRand >= 8500)//1Tips
	{
		StopNum = 30;
	}
	else if (nRand >= 6500)//1Tips
	{
		StopNum = 21;
	}
	else if (nRand >= 4500)//1Tips
	{
		StopNum = 23;
	}
	else if (nRand >= 2500)//None
	{
		StopNum = 20;
	}
	else//None
	{
		StopNum = 28;
	}
	float timerun = 0.05f;
	runTurntableGet(timerun);
	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_DAILYSPIN_LONG);
	}
}

void TurntableSystem::runTurntableGet(float time)
{
	this->schedule(schedule_selector(TurntableSystem::runAct), time); 
}

void TurntableSystem::changeBox(int boxid, bool change)
{

	CCPoint thisboxpiont = this->getChildByTag(TAG_ZP_BD)->getChildByTag(boxid)->getPosition();
	this->getChildByTag(TAG_ZP_BD)->removeChildByTag(boxid);
	Zp_BoxData* thisbox = new Zp_BoxData();
	thisbox->set_boxid(boxid);
	thisbox->set_xuanzhong(change);
	thisbox->set_boxReward(getReward(boxid));
	thisbox->set_point(thisboxpiont);
	Gzi* gz = new Gzi((Sprite*)this->getChildByTag(TAG_ZP_BD), thisbox); 
	thisbox->release();
	gz->release();	
	if (nexttime > 0.3)
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			SimpleAudioEngine::sharedEngine()->stopAllEffects();
			app->PlayEffect(SOUND_DAILYSPIN);
		}
	}
}

void TurntableSystem::runAct(float time)
{
	vector<int> gezi_l = GlobalInfo::getInstance()->get_gizilist();
	//Do spin
	if(gezi_l.size()>0)
	{
		if(fnum>gezi_l.size()-1)
		{
			fnum = 0;
		}
		int bid = gezi_l.at(fnum);
		changeBox(bid,true);
		int lastnum = fnum-1;
		if(lastnum<0)
		{
			lastnum=gezi_l.size()-1;
		}
		int lastid = gezi_l.at(lastnum);
		changeBox(lastid,false);
		fnum++;
	}
	runnum++;
	this->unschedule(schedule_selector(TurntableSystem::runAct)); 
	log("------%f----times=%d-",time,runnum);
	if(runnum<StopNum)
	{
		if (runnum>StopNum-12)
		{
			nexttime = time +  0.05f;


		}
		else nexttime = time;
		if(nexttime>=0.75f)
		{
			nexttime=0.75f;
		}
		this->schedule(schedule_selector(TurntableSystem::runAct),nexttime);
	}
	else
	{
		///unload this layer?
		//get the reward ??
		AppDelegate *app = (AppDelegate*)Application::getInstance();


		m_iUserTotalScore = app->GetUserTotalScore();
		int lastnum = fnum-1;
		if(lastnum<0)
		{
			lastnum=gezi_l.size()-1;
		}
		int lastid = gezi_l.at(lastnum);

		MsgBoxType m_type = TP_INFO;
		std::string title = "\0";
		std::string msg = "\0";
		std::string info = "\0";
		HttpRequest* request = new HttpRequest();
		string sUrl = GameAPIURL;
		char url[100];

		if (lastid >= 0 && lastid <21)
		{
			if (lastid >= 12) lastid -= 3;//3spare place in the middle
			Zp_BoxData* bdata = (Zp_BoxData*)boxgezi.at(lastid);// >objectAtIndex(i);
				//thisbox->set_boxid(bid);
			int iReward=bdata->get_boxReward();// set_boxReward(getReward(bid%6));
			log("box id=%d, reward=%d", lastid, iReward);
			int iTools = 0;
			int iUserScore = 0;

			switch (iReward)
			{
			case 0://1 tool
				title = "CONGRATULATIONS";
				msg = "You just won a Tool!";
				info = "You can use it during game play for a hint. Come back tomorrow for another spin!";
				iTools = app->getToolsNumberInfo();
				iTools++;
				app->setToolsNumberInfo(iTools);
				break;
			case 1://1 Point
				title = "CONGRATULATIONS";
				msg = "You just won 1 Tweebaa Point!";
				info = "Your reward has been added to your account. Come back tomorrow for another spin!";
				app->UpdateUserScore(1,1);
				break;
			case 2://10 Points
				title = "CONGRATULATIONS";
				msg = "You just won 10 Tweebaa Point!";
				info = "Your reward has been added to your account. Come back tomorrow for another spin!";
				app->UpdateUserScore(1, 10);
				break;
			case 3://50 Point
				title = "CONGRATULATIONS";
				msg = "You just won 50 Tweebaa Point!";
				info = "Your reward has been added to your account. Come back tomorrow for another spin!";
				app->UpdateUserScore(1, 50);
				break;
			case 4://100 Point
				title = "CONGRATULATIONS";
				msg = "You just won 100 Tweebaa Point!";
				info = "Your reward has been added to your account. Come back tomorrow for another spin!";
				app->UpdateUserScore(1, 100);
				break;
			case 5://5 Bucks
			case 6://10 Bucks
			case 7://Wonder cup
			case 8://Tweebot
				iPrizeID = iReward;
				sUrl = sUrl + "?action=Save_Spin_Request";
				strcpy(url, sUrl.c_str());
				request->setUrl(url);
				request->setRequestType(HttpRequest::Type::GET);
				request->setResponseCallback(CC_CALLBACK_2(TurntableSystem::onHttpRequestCompleted, this));
				request->setTag("Save_Spin_Request");
				cocos2d::network::HttpClient::getInstance()->send(request);
				request->release();
				return;
				break;
			default:
				title = "SORRY";
				msg = "Sorry! Come back tomorrow for another spin!";
			}

			request->release();
			MsgBox::setMsgBox(m_type, &title, &msg, &info);
			auto msgbox = MsgBox::create();

			msgbox->setTag(9000);
			this->addChild(msgbox, 900);
			//Add Event to process
			auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
			btnOk->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::CallBack, this));
			if (iReward >= 0)
			{
				btnOk->setTitleText("GREAT!");
			}
        }

		//this->schedule(schedule_selector(TurntableSystem::ShowPriceAndBack2WinScene),4.0f);

	}
}



void TurntableSystem::CallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		auto director = Director::getInstance();
		auto scene = ChooseLayer::createScene();
		director->replaceScene(scene);
		this->removeFromParentAndCleanup(true);

	}
}

void TurntableSystem::CallRegister(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		auto scene = RegisterView::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}
}

void TurntableSystem::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		SaveReward2Local(OFF_LINE);
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	// _labelStatusCode->setString(statusString);
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		SaveReward2Local(OFF_LINE);
		return;
	}
	if (statusCode != 200){
		log("response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		SaveReward2Local(OFF_LINE);
		return;
	}
	// dump data
	std::vector<char> *buffer = response->getResponseData();

	//get the return Random string
	std::string sRandom(buffer->begin(), buffer->end());
    string ss="29";
    ss=ss+sRandom.substr(2, 6);
    ss=ss+"34";
	string sTemp = Convert( ss );

	string sMd5 = CCMD5::md5(sTemp.c_str());
	string sMd5Up = Convert(sMd5);

	AppDelegate* app = (AppDelegate*)Application::getInstance();

	HttpRequest* request = new HttpRequest();
	std::string sUrl = GameAPIURL;
	sUrl = sUrl + "?action=Save_Spin";
	char url[100];
	strcpy(url, sUrl.c_str());
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(TurntableSystem::onHttpSaveRequestCompleted, this));

	// write the post data
	char pp[100];
	string sUser = app->GetUserGuid();
	int prize = iPrizeID + 1;
	sprintf(pp, "UserID=%s&PrizeID=%d&Md5=%s", sUser.c_str(), prize, sMd5.c_str());
	std::string sPost(pp);
	const char* postData = sPost.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Login POST");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

}
std::string TurntableSystem::Convert(std::string& str)
{
	std::string converted;

	for (short i = 0; i < str.size(); ++i)
		converted += toupper(str[i]);

	return converted;
}

void TurntableSystem::onHttpSaveRequestCompleted(HttpClient *sender, HttpResponse *response)
{

	if (!response)
	{
		SaveReward2Local(OFF_LINE);
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	// _labelStatusCode->setString(statusString);
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		SaveReward2Local(OFF_LINE);
		return;
	}
	if (statusCode != 200){
		log("response code failed");
		log("error buffer: %s", response->getErrorBuffer());
		SaveReward2Local(OFF_LINE);
		return;
	}

	if (m_UserID>0)//already login
	{
		ShowMessage(LOG_IN);
	}
	else
	{
		SaveReward2Local(NOT_LOG_IN);
	}
}

int TurntableSystem::getReward(int id)
{
	switch (id)
	{
	case 0:
	case 7:
	case 12:
	case 19:
		return(0);//Tips
		break;
	case 1:
	case 15:
		return(1);//Points_1
		break;
	case 2:
	case 5:
		return(2);//Points_10
		break;
	case 3:
	case 13:
		return(3);//Points_50
		break;
	case 4:
		return(8);//TweeBox
		break;
	case 6:
	case 17:
	case 9:
	case 10:
	case 11:
		return(-1);//Try Again;
		break;
	case 8:
	case 18:
		return(4);//Points_100
		break;
	case 14:
		return(7);//WonderCup
		break;
	case 16:
		return(5);//TweeBucks_5
		break;
	case 20:
		return(6);//TweeBucks_10
		break;
	default:
		return (-1);
	}
}

void TurntableSystem::ShowMessage(int iMode)
{
	MsgBoxType m_type;
	std::string title="\0";
	std::string msg = "\0";
	std::string info = "\0";
	title = "CONGRATULATIONS";
	if (iMode == LOG_IN)
	{
		m_type = TP_INFO;
		if (iPrizeID >= 5 && iPrizeID <= 6)//TweeBucks
		{
			msg = "You just won ";
			msg += BoxReward[iPrizeID];
			msg += "!";
			info = "Your reward has been added to your account. Come back tomorrow for another spin!";
		}
		else if (iPrizeID >= 7 && iPrizeID <= 8)//TweeBot | Wonder cap
		{
			msg = "You just won ";
			msg += BoxReward[iPrizeID];
			msg += "!";
			info = "Please check your email for instructions on how to claim your reward! Come back tomorrow tfor another spin!";
		}
		MsgBox::setMsgBox(m_type, &title, &msg, &info);
		auto msgbox = MsgBox::create();
		auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
		btnOk->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::CallBack, this));
		btnOk->setTitleText("GREAT!");
		msgbox->setTag(9000);
		this->addChild(msgbox, 900);
	}
	else if (iMode == OFF_LINE)
	{
		m_type = TP_INFO;
		if (iPrizeID >= 5 && iPrizeID <= 6)//TweeBucks
		{
			msg = "You just won ";
			msg += BoxReward[iPrizeID];
			msg += "!";
			info = "You are currently offline. Don't worry,you will recieve your rewards when you successfully log in next time.";
		}
		else if (iPrizeID >= 7 && iPrizeID <= 8)//TweeBot | Wonder cap
		{
			msg = "You just won ";
			msg += BoxReward[iPrizeID];
			msg += "!";
			info = "You are currently offline, but we’ll send you further instructions once you connect to the internet.";
		}
		MsgBox::setMsgBox(m_type, &title, &msg, &info);
		auto msgbox = MsgBox::create();
		auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
		btnOk->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::CallBack, this));
		btnOk->setTitleText("GREAT!");
		msgbox->setTag(9000);
		this->addChild(msgbox, 900);
	}
	else if(iMode == NOT_LOG_IN)
	{
		m_type = TP_Q_YESNO;
		if (iPrizeID >= 5 && iPrizeID <= 6)//TweeBucks
		{
			msg = "You just won ";
			msg += BoxReward[iPrizeID];
			msg += "!";
			info = "Please create a Tweebaa account, then your reward will be added to your account.";
		}
		else if (iPrizeID >= 7 && iPrizeID <= 8)//TweeBot | Wonder cap
		{
			msg = "You just won ";
			msg += BoxReward[iPrizeID];
			msg += "!";
			info = "Please create a Tweebaa account, then we will email instructions to you on how to claim your reward !";
		}
		MsgBox::setMsgBox(m_type, &title, &msg, &info);
		auto msgbox = MsgBox::create();
		auto btnYES = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
		btnYES->setTitleFontSize(36);
		btnYES->setTitleText("CREATE");
		btnYES->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::CallRegister, this));
		auto btnNO = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
		btnNO->setTitleText("NOT NOW");
		btnNO->setTitleFontSize(36);
		btnNO->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::CallBack, this));
		msgbox->setTag(9000);
		this->addChild(msgbox, 900);
	}

}


void TurntableSystem::SaveReward2Local(int iMode)
{
	
	string sLocalReward;
	string sReward;
	switch (iPrizeID)
	{
	case 5://5 tweebucks
		sLocalReward = DataAccess::getParamValue("LocalRW01", "\0");
		sReward = UpdateReward(sLocalReward);
		if (sReward.length()>0)
			{
				DataAccess::setParamValue("LocalRW01", sReward);
		}
		break;
	case 6://10 tweebucks
		sLocalReward = DataAccess::getParamValue("LocalRW01", "\0");
		sReward = UpdateReward(sLocalReward);
		if (sReward.length()>0)
		{
			DataAccess::setParamValue("LocalRW01", sReward);
		}
		break;
	case 7://wonder cap
		sLocalReward = DataAccess::getParamValue("LocalRW02", "\0");
		sReward = UpdateReward(sLocalReward);
		if (sReward.length()>0)
		{
			DataAccess::setParamValue("LocalRW02", sReward);
		}
		break;
	case 8://tweebot
		sLocalReward = DataAccess::getParamValue("LocalRW03", "\0");
		sReward = UpdateReward(sLocalReward);
		if (sReward.length()>0)
		{
			DataAccess::setParamValue("LocalRW03", sReward);
		}
		break;
	}
	ShowMessage(iMode);
}


std::string TurntableSystem::UpdateReward(std::string sLocalReward)
{

	AppDelegate *app = (AppDelegate*)Application::getInstance();

	string::size_type pos;
	string sReward = "\0";
	string sSub = "\0";
	string sDecrypt = "\0";
	string sEncrypt = "\0";
	char sTemp[100] = "\0";
	int iReward = 0;

	char *pszCurrTime = (char*)malloc(sizeof(char)* 20);
	memset(pszCurrTime, 0, sizeof(char)* 20);
	time_t now;
	time(&now);
	strftime(pszCurrTime, 20, "%Y/%m/%d %H:%M:%S", localtime(&now));


	if (sLocalReward.length() != 0)//Record exists
	{
		sDecrypt = app->base64_xxtea_decode(sLocalReward);
		pos = sDecrypt.find("|");
		if (pos != string::npos)//found
		{
			sSub = sDecrypt.substr(0, pos);
			sDecrypt = sDecrypt.substr(pos + 1, sDecrypt.size() - 1);
			iReward = atoi(sSub.c_str());
		}
		else
		{
			return sReward;//Structure not match,return blank;
		}
	}

	switch (iPrizeID)
	{
	case 5://5 tweebucks
		if (iReward >= 0 || iReward < 20)
		{
			iReward += 5;
		}
		else return sReward;
		break;
	case 6://10 tweebucks
		if (iReward >= 0 || iReward < 20)
		{
			iReward += 10;
		}
		else return sReward;
		break;
	case 7://wonder cap
		if (iReward ==0)
		{
			iReward ++;
		}
		else return sReward;
		break;
	case 8://tweebot
		if (iReward == 0)
		{
			iReward++;
		}
		else return sReward;
		break;
	}

	sprintf(sTemp, "%d|", iReward);
	strcat(sTemp, pszCurrTime);
	sReward = app->base64_xxtea_encode(sTemp);
	return sReward;

}


void TurntableSystem::ShowMsgBox(std::string title, std::string msg)
{
	MsgBoxType m_type = TP_INFO;
	MsgBox::setMsgBox(m_type, &title, &msg);
	auto msgbox = MsgBox::create();
	msgbox->setTag(MSGBOX_TAG);
	this->addChild(msgbox, 999);

	//Add Event to process
	auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
	btnOk->addTouchEventListener(CC_CALLBACK_2(TurntableSystem::CallBack, this));

}

void TurntableSystem::onHttpRequestGetDailyCheckInCompleted(HttpClient *sender, HttpResponse *response)
{
	std::string title = "\0";
	std::string msg = "\0";
	if (!response)
	{
		title = "Network Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsgBox(title, msg);
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	// _labelStatusCode->setString(statusString);
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		title = "Response Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsgBox(title, msg);
		return;
	}
	if (statusCode != 200){
		title = "Response Status Error\0";
		msg = "Ooops, something wrong with the network. Please try again later.\0";
		ShowMsgBox(title, msg);
		return;
	}
	// dump data
	std::vector<char> *buffer = response->getResponseData();

	char sXml[10000];

	//printf("Http Test, dump data: ");
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		sXml[i] = (*buffer)[i];
	}
	sXml[buffer->size()] = '\0';

	vecUserCheckInDays.clear();

	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	XMLError errorId = pDoc->Parse((const char*)sXml);
	if (errorId == 0){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		//app->rootRound = pDoc->RootElement();
		XMLElement *rootEle = pDoc->RootElement();
		XMLElement *idResult = rootEle->FirstChildElement();


		int e1 = atoi(idResult->GetText());
		if (e1>0){
			XMLElement *elResults = idResult->NextSiblingElement();

			XMLElement *elResult = elResults->FirstChildElement();

			log("aa=%s", elResult->GetText());
			while (elResult) {
				XMLElement *el_createtime = elResult;//gameEle->FirstChildElement(); //<round_id>1</round_id>
				XMLElement *el_SameWeek = el_createtime->NextSiblingElement(); //<round_step>1</round_step>
				XMLElement *el_WeekDay = el_SameWeek->NextSiblingElement();

				DailyCheckIn* p = new DailyCheckIn();
				if (el_createtime && el_createtime->GetText()){
					string s = el_createtime->GetText();
					p->checkin_date = s;

				}
				if (el_SameWeek && el_SameWeek->GetText()){
					string s = el_SameWeek->GetText();
					p->isSameWeek = atoi(s.c_str());

				}
				if (el_WeekDay && el_WeekDay->GetText()){
					string s = el_WeekDay->GetText();
					p->sWeekDay = s;

				}
				vecUserCheckInDays.pushBack(p);
				// next one
				elResult = el_WeekDay->NextSiblingElement();
			}

		}
	}
	delete pDoc;

	//LayerColor *layer1 = LayerColor::create(Color4B(0, 0, 0, 0));
	//Layer* layer1 = Layer::create();
	//auto scene = SpinLuckyLayer::create();
	//TransitionScene *transition = TransitionFade::create(1, scene);
	//Director::getInstance()->pushScene(scene);



	/*
	LayerColor *layer1 = LayerColor::create(Color4B(0, 0, 0, 180));
	DailyCheckInLayer* checkin = new DailyCheckInLayer(layer1);
	layer1->addChild(checkin);
	layer1->setTag(999);
	addChild(layer1, 999);*/
}
