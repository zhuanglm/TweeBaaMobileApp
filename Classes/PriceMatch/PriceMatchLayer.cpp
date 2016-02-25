#include "PriceMatchLayer.h"
//#include "GameScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	

#include "jigSaw/jigSawScene.h"
#include "utilits/CCSVParse.h"

#else
#include "jigSawScene.h"
#include "CCSVParse.h"
#endif

#include "global_const.h"
#include "GameWinScene.h"
#include "GameFailedScene.h"
#include "WelcomeScene.h"

#include "TweebaaMobileLayer.h"
#include "AppDelegate.h"


#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../utilits/Tutorial.h"
#include "../Msgbox/Dialogbox.h"
#include "../Msgbox/MsgBox.h"
#include "cocostudio/CocoStudio.h"//for animation
#include "theme/ThemeLayer.h"

extern int iTotalTime ;
using namespace ui;
using namespace cocostudio;

PriceMatchLayer::PriceMatchLayer(){};

PriceMatchLayer::~PriceMatchLayer(){
	vecQuestion.clear();
};

bool PriceMatchLayer::init(){

	iTotalTime = 90;
	if(!Layer::init()){
		return false;
	}

	thetime=0;
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	 std::string sCSD=app->GetLayoutString();
	_rootNode = CSLoader::createNode("res/price_right/PriceMatchLayer"+sCSD+".csb");

	addChild(_rootNode,1);

	//////////////////////////Load CSV File
	CCSVParse* csvFile = new CCSVParse();
    csvFile->openFile("price_match.csv");
	m_iTotalQuestion=csvFile->getRows();
	log("total question =%d",m_iTotalQuestion);
    for (int i=0; i<m_iTotalQuestion; ++i)
    {
        string strLine = "";
		PriceMatchQuestion* question=new PriceMatchQuestion();
		int iCols=csvFile->getCols();
		if(iCols>10) iCols=10;
		log("total col=%d",iCols);
        for(int j=0; j<iCols; ++j)
        {
			if(j==0){
				//ID 
				strLine = csvFile->getData(i,j);
				char p[100];
				strcpy(p,strLine.c_str());
				question->m_iIndex=atoi(p);
			}
			
			if(j==1){
				//image name,if image name length less then 6,then this is a text question
				strLine = csvFile->getData(i,j);
				question->m_strImgName=strLine;
			}
			if(j==2){
				//Product name
				strLine = csvFile->getData(i,j);
				question->m_strPrdName=strLine;
			}
			
            if(j==3){
				//Option A
				strLine = csvFile->getData(i,j);
				question->m_strOptionA=strLine;
			}
			if(j==4){
				//Option B
				strLine = csvFile->getData(i,j);
				question->m_strOptionB=strLine;
			}
			if(j==5){
				//Option C
				strLine = csvFile->getData(i,j);
				question->m_strOptionC=strLine;
			}
			if(j==6){
				//Option D
				strLine = csvFile->getData(i,j);
				question->m_strOptionD=strLine;
			}
			if(j==7){
				//Answer
				strLine = csvFile->getData(i,j);
				strLine = strLine.substr(0, 1);
				if (strLine.compare("a") == 0) question->m_iAnswer = 1;
				if (strLine.compare("b") == 0) question->m_iAnswer = 2;
				if (strLine.compare("c") == 0) question->m_iAnswer = 3;
				if (strLine.compare("d") == 0) question->m_iAnswer = 4;
			}
        }
		vecQuestion.pushBack(question);
    }

	log("now init GUI");
	////////////////////////////////////
	m_iCurrentIndex = app->getCurrentGameLevel();  //当前关卡
	Current_Question = vecQuestion.at(int(rand() %vecQuestion.size()));


	ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivProductImg")); 
	if (Current_Question->m_strImgName.length() > 6)
	{
		auto card1 = Sprite::create(Current_Question->m_strImgName);

		card1->setAnchorPoint(Vec2(0.5, 0.5));
		card1->setPosition(Vec2(imgPromotion->getContentSize().width / 2, imgPromotion->getContentSize().height / 2));
		float width = imgPromotion->getContentSize().width;
		card1->setScale(0.6f);

		imgPromotion->addChild(card1, 2, 2000);

	}

	///////////////////////
	//ＵＩfor Image Questions

	auto lblQuestion = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblQuestion"));

	if (lblQuestion)
		lblQuestion->setString(Current_Question->m_strPrdName);

	auto btnOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionA"));
	btnOptionA->setTag(1001);
	btnOptionA->setTitleText(Current_Question->m_strOptionA);
	btnOptionA->setTitleFontSize(20.0f);
	btnOptionA->setTitleColor(ccc3(72, 37, 0));
	btnOptionA->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	auto btnOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionB"));
	btnOptionB->setTag(1002);
	btnOptionB->setTitleText(Current_Question->m_strOptionB);
	btnOptionB->setTitleFontSize(20.0f);
	btnOptionB->setTitleColor(ccc3(72, 37, 0));
	btnOptionB->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	auto btnOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionC"));
	btnOptionC->setTag(1003);
	btnOptionC->setTitleText(Current_Question->m_strOptionC);
	btnOptionC->setTitleFontSize(20.0f);
	btnOptionC->setTitleColor(ccc3(72, 37, 0));
	btnOptionC->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	auto btnOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionD"));
	btnOptionD->setTag(1004);
	btnOptionD->setTitleText(Current_Question->m_strOptionD);
	btnOptionD->setTitleFontSize(20.0f);
	btnOptionD->setTitleColor(ccc3(72, 37, 0));
	btnOptionD->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	/////////////////////////////////////////////////////////////////
	///UI for Questions
	auto price_r_wood_13_0 = dynamic_cast<Sprite*>(_rootNode->getChildByName("price_r_wood_13_0"));
	auto lblQuestion_0 = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblQuestion_0"));

	auto btnTextOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionA"));
	btnTextOptionA->setTag(3001);
	btnTextOptionA->setTitleFontSize(20.0f);
	btnTextOptionA->setTitleColor(ccc3(72, 37, 0));
	btnTextOptionA->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	auto btnTextOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionB"));
	btnTextOptionB->setTag(3002);
	btnTextOptionB->setTitleFontSize(20.0f);
	btnTextOptionB->setTitleColor(ccc3(72, 37, 0));
	btnTextOptionB->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	auto btnTextOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionC"));
	btnTextOptionC->setTag(3003);
	btnTextOptionC->setTitleFontSize(20.0f);
	btnTextOptionC->setTitleColor(ccc3(72, 37, 0));
	btnTextOptionC->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));

	auto btnTextOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionD"));
	btnTextOptionD->setTag(3004);
	btnTextOptionD->setTitleFontSize(20.0f);
	btnTextOptionD->setTitleColor(ccc3(72, 37, 0));
	btnTextOptionD->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));


	auto TrueA = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueA"));
	auto TrueB = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueB"));
	auto TrueC = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueC"));
	auto TrueD = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueD"));

	auto FalseA = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseA"));
	auto FalseB = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseB"));
	auto FalseC = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseC"));
	auto FalseD = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseD"));

	TrueA->setZOrder(1000);
	TrueB->setZOrder(1000);
	TrueC->setZOrder(1000);
	TrueD->setZOrder(1000);
	FalseA->setZOrder(1000);
	FalseB->setZOrder(1000);
	FalseC->setZOrder(1000);
	FalseD->setZOrder(1000);
	/////////////////////////////////////////
	m_iCurrentGameLevel = app->getCurrentGameLevel();
	//-----------------------------------add animation----------------------------------------------------------

	auto mastco = dynamic_cast<Sprite *> (_rootNode->getChildByName("Sprite_10"));
	//int mastcoZOrder=-1;
	if (mastco)
	{
		//mastcoZOrder = mastco->getZOrder();
		mastco->setVisible(false);
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/1.4/1.4.ExportJson");
	Armature *armature = Armature::create("1.4");
	armature->setAnchorPoint(Vec2(0.5, 0.5));
	armature->setVisible(true);
	armature->setTag(888);
	schedule(schedule_selector(PriceMatchLayer::PlayAnimation), 5.0f);
	_rootNode->addChild(armature);
	//-------------------------------------------------------------------------------------
	switchUI();
	
	auto btnTips = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTips"));
	if (btnTips){
		btnTips->setTag(2001);
		btnTips->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));
	}
	auto button_Main = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("button_Main"));
	if (button_Main){
		button_Main->setTag(2002);
		button_Main->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));
	}
	auto btnTipsTools = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTipsTools"));
	if (btnTipsTools){
		btnTipsTools->setTag(2003);
		btnTipsTools->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::touchButton, this));
	}

	if (app->iGameMode == MobileGameMode::Theme_Mode){
		//button_Main->setVisible(false);
		//btnTips->setVisible(false);
	}
	_nTlimit = 3; // total tools can be used in each game
	_nTlcount = app->getToolsNumberInfo(); // total tools user has
	_nLvl = app->GetUserLevel(); //totol tips can be used
	_fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
	_lTtlScore = app->GetUserTotalScore();
	
	auto timerLabel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblTimer"));

	auto lvlbar = dynamic_cast<Sprite*> (_rootNode->getChildByName("lv_bar_4"));
	lvlbar->setAnchorPoint(Vec2(0, 0.5));
	//lvlbar->setPositionX(lvlbar->getPositionX() - lvlbar->getContentSize().width / 2);
	lvlbar->setScaleX(_fPerToNLvl);
	

	schedule(schedule_selector(PriceMatchLayer::updateLabels), 0.01f);
	schedule(schedule_selector(PriceMatchLayer::updateTimeLabel), 1.0f);

	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->StartBGMusic(BGM_GAME);
	}

	app->LogAccess("PriceMatch");
	delete csvFile;


	_bTlEnabled = true;
	return true;
}

void PriceMatchLayer::PlayAnimation(float dt)
{
	Armature *armature = dynamic_cast<Armature *>(_rootNode->getChildByTag(888));
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	AppDelegate* app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_ANIMATION);
	}
}
void PriceMatchLayer::loadFirstTutorial(float dt)
{
	unschedule(schedule_selector(PriceMatchLayer::loadFirstTutorial));
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		Director::getInstance()->pushScene(scene);
	}
}
void PriceMatchLayer::switchUI(){

	auto lblQuestion = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblQuestion"));
	auto btnOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionA"));
	auto btnOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionB"));
	auto btnOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionC"));
	auto btnOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionD"));

	auto price_r_wood_13_0 = dynamic_cast<Sprite*>(_rootNode->getChildByName("price_r_wood_13_0"));
	auto lblQuestion_0 = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblQuestion_0"));

	auto btnTextOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionA"));
	auto btnTextOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionB"));
	auto btnTextOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionC"));
	auto btnTextOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionD"));


	auto Text_1 = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("Text_1"));
	
	auto price_r_wood_13 = dynamic_cast<Sprite*>(_rootNode->getChildByName("price_r_wood_13"));
	auto Sprite_4 = dynamic_cast<Sprite*>(_rootNode->getChildByName("Sprite_4"));
	auto Image_1 = dynamic_cast<ImageView*>(_rootNode->getChildByName("Image_1"));
	ImageView* imgPromotion = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivProductImg"));

	Armature *armature = dynamic_cast<Armature *>(_rootNode->getChildByTag(888));

	if (Current_Question->m_strImgName.length() < 6)//questions
	{
		btnOptionA->setVisible(false);
		btnOptionB->setVisible(false);
		btnOptionC->setVisible(false);
		btnOptionD->setVisible(false);
		price_r_wood_13->setVisible(false);
		if (imgPromotion) imgPromotion->setVisible(false);

		Text_1->setVisible(true);
		Image_1->setVisible(true);
		price_r_wood_13_0->setVisible(true);
		lblQuestion_0->setVisible(true);
		lblQuestion_0->setString(Current_Question->m_strPrdName);
		btnTextOptionA->setVisible(true);
		btnTextOptionA->setTitleFontSize(30);
		btnTextOptionA->setTitleText(Current_Question->m_strOptionA);
		btnTextOptionB->setVisible(true);
		btnTextOptionB->setTitleFontSize(30);
		btnTextOptionB->setTitleText(Current_Question->m_strOptionB);
		btnTextOptionC->setVisible(true);
		btnTextOptionC->setTitleFontSize(30);
		btnTextOptionC->setTitleText(Current_Question->m_strOptionC);
		btnTextOptionD->setVisible(true);
		btnTextOptionD->setTitleFontSize(30);
		btnTextOptionD->setTitleText(Current_Question->m_strOptionD);

		armature->setPosition(Vec2(155,450));
		armature->setRotation(270);
		price_r_wood_13_0->setZOrder(1);
		Image_1->setZOrder(2);
		Text_1->setZOrder(2);
		btnTextOptionA->setZOrder(2);
		btnTextOptionB->setZOrder(2);
		btnTextOptionC->setZOrder(2);
		btnTextOptionD->setZOrder(2);
		lblQuestion_0->setZOrder(3);
		btnTextOptionA->setZOrder(2);
	}
	else
	{
		Text_1->setVisible(false);
		Image_1->setVisible(false);
		price_r_wood_13->setVisible(true);

		price_r_wood_13_0->setVisible(false);
		lblQuestion_0->setVisible(false);
		btnTextOptionA->setVisible(false);
		btnTextOptionB->setVisible(false);
		btnTextOptionC->setVisible(false);
		btnTextOptionD->setVisible(false);

		btnOptionA->setVisible(true);
		btnOptionB->setVisible(true);
		btnOptionC->setVisible(true);
		btnOptionD->setVisible(true);

		armature->setPosition(Vec2(825, 410));
		armature->setRotation(0);
		Sprite_4->setZOrder(1);
		btnOptionA->setZOrder(2);
		btnOptionB->setZOrder(2);
		btnOptionC->setZOrder(2);
		btnOptionD->setZOrder(2);

		if (imgPromotion)
		{
			imgPromotion->setVisible(true);
			imgPromotion->setZOrder(1000);
		}
		Sprite* card1 = (Sprite*)imgPromotion->getChildByTag(2000);
		if (card1)//image question
		{
			Image* image = nullptr;

			image = new Image();
			image->initWithImageFile(Current_Question->m_strImgName);
			Texture2D* texture = new Texture2D();
			texture->initWithImage(image);
			card1->setTexture(texture);

			image->release();
			texture->release();
		}
		else
		{
			card1 = Sprite::create(Current_Question->m_strImgName);

			card1->setAnchorPoint(Vec2(0.5, 0.5));
			card1->setPosition(Vec2(imgPromotion->getContentSize().width / 2, imgPromotion->getContentSize().height / 2));
			card1->setScale(2.5f);

			imgPromotion->addChild(card1, 2, 2000);
		}

		btnOptionA->setTitleText(Current_Question->m_strOptionA);
		btnOptionB->setTitleText(Current_Question->m_strOptionB);
		btnOptionC->setTitleText(Current_Question->m_strOptionC);
		btnOptionD->setTitleText(Current_Question->m_strOptionD);



	}

}
void PriceMatchLayer::updateTimeLabel(float dt) {
	iTotalTime -= 1;
	char lblTimer[100];
	sprintf(lblTimer, "%d:%02d", (int)(iTotalTime / 60), (int)iTotalTime % 60);
	string string1 = lblTimer;
	auto timerLabel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblTimer"));
	timerLabel->setString(string1);
	if (iTotalTime>0 && iTotalTime < 10) {
		timerLabel->setColor(ccc3(255, 0, 0));
	}
	if (iTotalTime <= 0){
		//game over
		this->unschedule(schedule_selector(PriceMatchLayer::updateLabels));
		this->unschedule(schedule_selector(PriceMatchLayer::updateTimeLabel));
		schedule(schedule_selector(PriceMatchLayer::GameFailed),1.0f);
	}
}

void PriceMatchLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  

	switch (eventType)
	{
	case ::ui::Widget::TouchEventType::ENDED:
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		if (tag == 2001){ //Tutorial
			auto scene = TutorialScene::create();
			if (scene != nullptr){
				TransitionScene *transition = TransitionFade::create(0, scene);
				Director::getInstance()->pushScene(transition);
			}
		}
		if (tag == 2002){ //Home

			MsgBoxType m_type = TP_Q_YESNO;
			std::string title = MSG_QUIT_TITLE;
			std::string msg = MSG_QUIT_MSG;

			MsgBox::setMsgBox(m_type, &title, &msg);
			auto msgbox = MsgBox::create();
			msgbox->setTag(9000);
			this->addChild(msgbox, 999);

			//Add Event to process
			auto btnYes = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
			auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
			btnYes->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::yesCallback, this));
			btnNo->addTouchEventListener(CC_CALLBACK_2(PriceMatchLayer::noCallback, this));

		}
		if (tag == 2003)
		{  //AI tips
			if (_bTlEnabled == false) return;
			_bTlEnabled = false;
			if (_nTlimit > 0)
			{
				AppDelegate *app = (AppDelegate*)Application::getInstance();
				if (_nTlcount > 0)
				{
					_nTlcount--;
					app->setToolsNumberInfo(_nTlcount);
				}
				else
				{
					if (_lTtlScore >= 10){
						showDeductPointEffect("txtScore");
						_lTtlScore = _lTtlScore - 10;
						app->UpdateUserScore(0, 10);
					}
					else{
						return;
					}

				}
				_nTlimit--;
				if (_nTlimit == 0){
					auto spTips = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTipsTools"));
					spTips->setColor(ccc3(84, 84, 84));//not disabled button, so use color instead
				}
				app->PlayEffect(SOUND_TOOL_CLICK);
				UseTips(2-_nTlimit);
				this->scheduleOnce(schedule_selector(PriceMatchLayer::enableTools), 2.0f);
			}
			else {
				return;
			}
		}
		if ((tag >= 1001 && tag <= 1004) || (tag >= 3001 && tag <= 3004))
		{
			if (Current_Question->m_iAnswer == 1)
			{
				auto TrueA = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueA"));
				TrueA->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueA->setPositionY(385);
				}
			}
			if (Current_Question->m_iAnswer == 2)
			{
				auto TrueB = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueB"));
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueB->setPositionY(290);
				}
				TrueB->setVisible(true);
			}
			if (Current_Question->m_iAnswer == 3)
			{
				auto TrueC = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueC"));
				TrueC->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueC->setPositionY(195);
				}
			}
			if (Current_Question->m_iAnswer == 4)
			{
				auto TrueD = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueD"));
				TrueD->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueD->setPositionY(100);
				}
			}
		}

		if (tag == 1001 || tag == 3001){
			//Answer A
			if (Current_Question->m_iAnswer == 1){
				auto TrueA = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueA"));
				TrueA->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueA->setPositionY(385);
				}
				schedule(schedule_selector(PriceMatchLayer::GameWin), 1.0f);
			}
			else{
				auto FalseA = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseA"));
				FalseA->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					FalseA->setPositionY(385);
				}
				schedule(schedule_selector(PriceMatchLayer::GameFailed), 1.0f);
			}
		}
		if (tag == 1002 || tag == 3002){
			//Answer B
			if (Current_Question->m_iAnswer == 2){
				auto TrueB = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueB"));
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueB->setPositionY(290);
				}
				TrueB->setVisible(true);
				schedule(schedule_selector(PriceMatchLayer::GameWin), 1.0f);
			}
			else{
				auto FalseB = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseB"));
				if (Current_Question->m_strImgName.length() > 6)
				{
					FalseB->setPositionY(290);
				}
				FalseB->setVisible(true);
				schedule(schedule_selector(PriceMatchLayer::GameFailed), 1.0f);
			}
		}
		if (tag == 1003 || tag == 3003){
			//Answer C
			if (Current_Question->m_iAnswer == 3){
				auto TrueC = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueC"));
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueC->setPositionY(195);
				}
				TrueC->setVisible(true);
				schedule(schedule_selector(PriceMatchLayer::GameWin), 1.0f);
			}
			else{
				auto FalseC = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseC"));
				FalseC->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					FalseC->setPositionY(195);
				}
				schedule(schedule_selector(PriceMatchLayer::GameFailed), 1.0f);
			}
		}
		if (tag == 1004 || tag == 3004){
			//Answer D
			if (Current_Question->m_iAnswer == 4){
				auto TrueD = dynamic_cast<Sprite*>(_rootNode->getChildByName("TrueD"));
				TrueD->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					TrueD->setPositionY(100);
				}
				schedule(schedule_selector(PriceMatchLayer::GameWin), 1.0f);
			}
			else{
				auto FalseD = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseD"));
				FalseD->setVisible(true);
				if (Current_Question->m_strImgName.length() > 6)
				{
					FalseD->setPositionY(100);
				}
				schedule(schedule_selector(PriceMatchLayer::GameFailed), 1.0f);
			}
		}


		if (tag >= 1001 && tag <= 1004)
		{
			auto btnOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionA"));
			auto btnOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionB"));
			auto btnOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionC"));
			auto btnOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionD"));
			btnOptionA->setEnabled(false);
			btnOptionB->setEnabled(false);
			btnOptionC->setEnabled(false);
			btnOptionD->setEnabled(false);
		}

		if (tag >= 3001 && tag <= 3004)
		{

			auto btnTextOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionA"));
			auto btnTextOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionB"));
			auto btnTextOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionC"));
			auto btnTextOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionD"));
			btnTextOptionA->setEnabled(false);
			btnTextOptionB->setEnabled(false);
			btnTextOptionC->setEnabled(false);
			btnTextOptionD->setEnabled(false);
		}
		break;
    }  
}  

void PriceMatchLayer::UseTips(int iSequence)
{
	if (Current_Question->m_strImgName.length() > 6)//image questions
	{
		if (Current_Question->m_iAnswer == 1)//A
		{
			DisableImg(SequenceA[iSequence]);
		}
		else if(Current_Question->m_iAnswer == 2)//B
		{
			DisableImg(SequenceB[iSequence]);
		}
		else if (Current_Question->m_iAnswer == 3)//C
		{
			DisableImg(SequenceC[iSequence]);
		}
		else //D
		{
			DisableImg(SequenceD[iSequence]);
		}
	}
	else
	{
		if (Current_Question->m_iAnswer == 1)//A
		{
			DisableButton(SequenceA[iSequence]);
		}
		else if (Current_Question->m_iAnswer == 2)//B
		{
			DisableButton(SequenceB[iSequence]);
		}
		else if (Current_Question->m_iAnswer == 3)//C
		{
			DisableButton(SequenceC[iSequence]);
		}
		else //D
		{
			DisableButton(SequenceD[iSequence]);
		}

	}
}

void PriceMatchLayer::DisableImg(int iButton)
{
	if (iButton == 1)//
	{
		auto btnOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionA"));
		btnOptionA->setEnabled(false);
		btnOptionA->setTitleColor(ccc3(84, 84, 84));
		auto FalseA = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseA"));
		FalseA->setVisible(true);
		FalseA->setPositionY(385);
	}
	else if (iButton == 2)
	{
		auto btnOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionB"));
		btnOptionB->setEnabled(false);
		btnOptionB->setTitleColor(ccc3(84, 84, 84));
		auto FalseB = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseB"));
		FalseB->setVisible(true);
		FalseB->setPositionY(290);
	}
	else if (iButton == 3)
	{
		auto btnOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionC"));
		btnOptionC->setEnabled(false);
		btnOptionC->setTitleColor(ccc3(84, 84, 84));
		auto FalseC = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseC"));
		FalseC->setVisible(true);
		FalseC->setPositionY(195);
	}
	else
	{
		auto btnOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnOptionD"));
		btnOptionD->setEnabled(false);
		btnOptionD->setTitleColor(ccc3(84, 84, 84));
		auto FalseD = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseD"));
		FalseD->setVisible(true);
		FalseD->setPositionY(100);
	}
}

void PriceMatchLayer::DisableButton(int iButton)
{
	if (iButton == 1)//
	{
		auto btnTextOptionA = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionA"));
		btnTextOptionA->setEnabled(false);
		btnTextOptionA->setTitleColor(ccc3(84, 84, 84));
		auto FalseA = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseA"));
		FalseA->setVisible(true);
	}
	else if (iButton == 2)
	{
		auto btnTextOptionB = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionB"));
		btnTextOptionB->setEnabled(false);
		btnTextOptionB->setTitleColor(ccc3(84, 84, 84));
		auto FalseB = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseB"));
		FalseB->setVisible(true);
	}
	else if (iButton == 3)
	{
		auto btnTextOptionC = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionC"));
		btnTextOptionC->setEnabled(false);
		btnTextOptionC->setTitleColor(ccc3(84, 84, 84));
		auto FalseC = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseC"));
		FalseC->setVisible(true);
	}
	else
	{
		auto btnTextOptionD = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTextOptionD"));
		btnTextOptionD->setEnabled(false);
		btnTextOptionD->setTitleColor(ccc3(84, 84, 84));
		auto FalseD = dynamic_cast<Sprite*>(_rootNode->getChildByName("FalseD"));
		FalseD->setVisible(true);
	}
}


void PriceMatchLayer::GameWinJump(){
	int score = 0;
	int ttlseconds = iTotalTime;
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->iGameMode == MobileGameMode::Theme_Mode){
		app->GameRunRoundNext();
	}
	else{
		/*
		if (app->getGameLevelPassedInfo() == 0){
			if (ttlseconds > 60 && ttlseconds <= 90){
				score = WIN_POINT_HIG;
			}
			else if (ttlseconds > 30 && ttlseconds <= 60){
				score = WIN_POINT_MED;
			}
			else{
				score = WIN_POINT_LOW;
			}
		}
		else{
			score = WIN_POINT_LOW;
		}
		app->UpdateUserScore(1, score);
		app->SaveGamePassLevel();//Add by Long to Save Level
		app->saveGameLevelPassedInfo();
		app->moveToNextGameLevel();
		app->setReward(score);*/
		app->setPromoteProduct("");
		app->updateGameWinScore(ttlseconds);
		app->SaveGamePassLevel();//Add by Long to Save Level
		app->moveToNextGameLevel();

		auto scene = GameWin::createScene();
		if (scene != nullptr){
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
	}
}
void PriceMatchLayer::GameWin(float dt){
	

	this->unschedule(schedule_selector(PriceMatchLayer::updateLabels));
	this->unschedule(schedule_selector(PriceMatchLayer::updateTimeLabel));
	this->unschedule(schedule_selector(PriceMatchLayer::GameWin));

	GameWinJump();
	
}

void PriceMatchLayer::GameFailed(float dt){
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	app->moveToNextGameLevel();

	this->unschedule(schedule_selector(PriceMatchLayer::updateLabels));
	this->unschedule(schedule_selector(PriceMatchLayer::updateTimeLabel));
	this->unschedule(schedule_selector(PriceMatchLayer::GameFailed));

	auto scene = GameFailed::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void PriceMatchLayer::menuCallbackOptionA(Ref *sender) {

	//CCDirector.sharedDirector().replaceScene(SlidingMenuLayer.scene());
	auto scene = GameWin::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void PriceMatchLayer::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		//app->SaveGamePassLevel();
		app->moveToNextGameLevel();
		

		this->unschedule(schedule_selector(PriceMatchLayer::updateLabels));
		this->unschedule(schedule_selector(PriceMatchLayer::updateTimeLabel));

		if (app->iGameMode == MobileGameMode::Theme_Mode)
		{
			auto scene = ThemeLayer::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}
		else
		{
			auto scene = WelcomeScene::create();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}

		this->removeChildByTag(9000);
		CCLOG("touch ended.");
	}
}

void PriceMatchLayer::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {
		//Don't quit the game
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		this->removeChildByTag(9000);
		//enable the touch event
	}
}

void PriceMatchLayer::updateLabels(float dt)
{
	char lblTips[10];
	sprintf(lblTips, "%d", _nTlcount);
	string string2 = lblTips;
	auto tipsLabel = dynamic_cast<Text*> (_rootNode->getChildByName("txtTips"));
	tipsLabel->setString(string2);

	char lblLvl[10];
	sprintf(lblLvl, "%d", _nLvl);
	string string3 = lblLvl;
	auto lvlLabel = dynamic_cast<Text*> (_rootNode->getChildByName("txtLvl"));
	lvlLabel->setString(string3);


	char lblScore[20];
	sprintf(lblScore, "%lu", _lTtlScore);
	string string4 = lblScore;
	auto scoreLabel = dynamic_cast<Text*> (_rootNode->getChildByName("txtScore"));
	scoreLabel->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	scoreLabel->setString(string4);
}
void PriceMatchLayer::showDeductPointEffect(std::string sLabel){

	auto scoreLabel = dynamic_cast<Text*> (_rootNode->getChildByName(sLabel.c_str()));
	if (scoreLabel == nullptr) return;

	//show score effect
	auto blink2 = Blink::create(1.5, 3);
	auto tintto = TintTo::create(0.01f, 255.0f, 0.0f, 0.0f);
	auto tintback = TintTo::create(0.01f, 255.0f, 255.0f, 255.0f);
	auto seq1 = Sequence::create(tintto, blink2, tintback, nullptr);
	scoreLabel->runAction(seq1);

	return;
}


void PriceMatchLayer::enableTools(float dt){
	_bTlEnabled = true;
}