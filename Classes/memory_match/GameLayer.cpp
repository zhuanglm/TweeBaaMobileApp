#include "GameLayer.h"
#include "cocostudio/CocoStudio.h"//for animation
#include "theme/ThemeLayer.h"

using namespace cocostudio;


int iFirstCard = 0;
int iSecondCard = 0;
int iWaiting = 0;
int iMaxCard = 8;
int iRows = 2; //Total Row
int iCols = 4; //Total Column
extern int iTotalTime = 30;
int flag = 1;
char sFile[4][100] = { "\0", "\0", "\0", "\0" };
Vector<Sprite*> spCard;

TouchSprite* firstCard;
TouchSprite* secondCard;



GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::randomArray(int a[], int n)
{
	int index, tmp, i;
	srand(time(NULL));
	for (i = 0; i <n; i++)
	{
		index = rand() % (n - i) + i;
		if (index != i)
		{
			tmp = a[i];
			a[i] = a[index];
			a[index] = tmp;
		}
	}
}

bool GameLayer::init()
{
	iTotalTime = 30;
	spCard.clear();//clear it first
	if (Layer::init())
	{

		//-----------------------------------Setup basic settings-----------------------------------------
		//get the origin point of the X-Y axis, and the visiable size of the screen
		Size visiableSize = Director::getInstance()->getVisibleSize();
		Point origin = Director::getInstance()->getVisibleOrigin();

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		std::string sCSD = app->GetLayoutString();
		_rootNode = CSLoader::createNode("res/memory_game/flip" + sCSD + ".csb");
		addChild(_rootNode, 1);

		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Flip_Level.plist");
		thetime = 0;
		iFirstCard = -1;
		iSecondCard = -1;

		//--------------------------------------Setup common gadgets-------------------------------------
		_nTlimit = 3; // total tools can be used in each game
		_nTlcount = app->getToolsNumberInfo(); // total tools user has
		_nLvl = app->GetUserLevel(); //totol tips can be used
		_fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
		_lTtlScore = app->GetUserTotalScore();

		auto btnTips = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTips"));
		if (btnTips){
			btnTips->setTag(2001);
			btnTips->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchButton, this));
		}
		auto button_Main = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("button_Main"));
		if (button_Main){
			button_Main->setTag(2002);
			button_Main->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchButton, this));
		}
		auto btnTipsTools = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTipsTools"));
		if (btnTipsTools){
			btnTipsTools->setTag(2003);
			btnTipsTools->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchButton, this));
		}

		if (app->iGameMode == MobileGameMode::Theme_Mode){
			//button_Main->setVisible(false);
			//btnTips->setVisible(false);
		}
		auto timerLabel = dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblTimer"));

		auto lvlbar = dynamic_cast<Sprite*> (_rootNode->getChildByName("lv_bar_4"));
		lvlbar->setAnchorPoint(Vec2(0, 0.5));
		lvlbar->setScaleX(_fPerToNLvl);

		if (app->getBGMstatus() == BGMusic_flag::ON)
		{
			app->StartBGMusic(BGM_GAME);
		}

		//----------------------------------create a random array------------------------------------
		int iDiv = iMaxCard / 2;
		int* array1 = new int[iMaxCard / 2];
		int* array2 = new int[iMaxCard / 2];
		int* unique_array = new int[iMaxCard];
		int* array4 = new int[iMaxCard];
		int i = 0;
		for (i = 0; i<iDiv; i++)
		{
			array1[i] = i;
			array2[i] = i;
		}
		randomArray(array1, iMaxCard / 2);
		randomArray(array2, iMaxCard / 2);
		for (i = 0; i<iMaxCard; i++)
		{
			unique_array[i] = i;
		}
		for (i = 0; i<iDiv; i++)
		{
			array4[i] = array1[i];
		}
		for (i = iDiv; i<iMaxCard; i++)
		{
			array4[i] = array2[i - iDiv];
		}
		randomArray(unique_array, iMaxCard);
		for (i = 0; i<iMaxCard; i++)
		{
			unique_array[i] = array4[unique_array[i]];
			log("i=%d,%d", i, array4[unique_array[i]]);
		}
		//setup brick content
		srand((unsigned)time(0));//set rand() with real
		int random = rand();
		sprintf(sFile[0], "%s", sGame[random%MUST_HAVE]);
		sprintf(sFile[1], "%s", sGame[MUST_HAVE + int(random % (IMAGE_COUNT - MUST_HAVE) / 3)]);
		sprintf(sFile[2], "%s", sGame[MUST_HAVE + int((IMAGE_COUNT - MUST_HAVE) / 3) + int(random % (IMAGE_COUNT - MUST_HAVE) / 3)]);
		sprintf(sFile[3], "%s", sGame[MUST_HAVE + int((IMAGE_COUNT - MUST_HAVE) / 3) * 2 + int(random % (IMAGE_COUNT - MUST_HAVE) / 3)]);

		auto memory_bg_2 = dynamic_cast<Sprite*>(_rootNode->getChildByName("memory_bg_2"));
		if (memory_bg_2){
			memory_bg_2->setZOrder(-100);

		}

		//-----------------------------------------Touch Event Create----------------------------------------------------
		auto touchListener1 = EventListenerTouchOneByOne::create();
		touchListener1->onTouchBegan = [this](Touch* touch, Event* event) -> bool//start lambda,[]内为捕获列表
		{
			auto bounds = event->getCurrentTarget()->getBoundingBox();

			if (bounds.containsPoint(touch->getLocation()))
			{
				if (iWaiting == 1){
					return true;
				}
				else if (iWaiting == 2)/*already open 2 cards,turn over,reset*/
				{
					firstCard->setScale(1);
					firstCard->setTexture("res/memory_game/memory_square.png");
					//firstCard->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("memory_square.png"));//roll back 
					firstCard->cancel_rollback_timer();
					secondCard->setScale(1);
					secondCard->setTexture("res/memory_game/memory_square.png");
					//secondCard->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("memory_square.png"));//roll back
					secondCard->cancel_rollback_timer();
					iFirstCard = -1;
					iSecondCard = -1;
					iWaiting = 0;
					return true;
				}

				auto target = static_cast<TouchSprite*>(event->getCurrentTarget());
				int i = target->getTag() - 1000;
				char file[100];
				if (i<1 || i>8){
					log("invalide target tag=%d", i);
					return true;
				}
				sprintf(file, "%s", sFile[i - 1]);

				bool bFound = false;
				log("enter here 1=%d,2=%d", iFirstCard, iSecondCard);
				if (iFirstCard == -1 && iSecondCard == -1)
				{
					iFirstCard = target->getZOrder() - 20;
				}
				else if (iFirstCard >= 0 && iSecondCard == -1)
				{
					iWaiting = 1; //waiting for 
					iSecondCard = target->getZOrder() - 20;
					if (iFirstCard == iSecondCard)
					{
						//The same card,
						bFound = false;
						iSecondCard = -1;
						iWaiting = 0;
						return 0;
					}
					else
					{
						firstCard = (TouchSprite*)spCard.at(iFirstCard);
						secondCard = (TouchSprite*)spCard.at(iSecondCard);
						log("tag1=%d,tag2=%d", firstCard->getTag(), secondCard->getTag());
						if (firstCard->getTag() == secondCard->getTag() && firstCard->getTag()>1000)
						{
							bFound = true;
							firstCard->setTag(999);
							secondCard->setTag(999);
							iFirstCard = -1;
							iSecondCard = -1;
							iWaiting = 0;
							CheckWin();
						}
						else
						{
							iFirstCard = -1;
							iSecondCard = -1;
							log("not found!");
							iWaiting = 2;//next touch will reset
							if (iTotalTime > 1)
							{
								iTotalTime --;
							}
							else
							{
								iTotalTime = 0;
							}
							this->showDeductPointEffect("lblTimer");
						}
					}
				}
				if (!bFound)
				{
					log("file=%s,should setup rollback timer,tag=%d", file, target->getTag());
					target->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));
					if (!target)
					{
						return 1;
					}
					//target->setScale(0.95);//?
					target->setScaleX(0.9);
					target->setScaleY(0.97);

					if (iFirstCard >= 0 && iSecondCard == -1)//first card
					{
						firstCard = target;
						target->setup_rollback_timer(4);//scheduleOnce(schedule_selector(GameLayer::RollBackClickMe), 2.0f );
					}
					else
					{
						iFirstCard = -1;
						iSecondCard = -1;
						firstCard->cancel_rollback_timer();
						firstCard->setup_rollback_timer(2);//should be rollback with the first card simultaneously
						target->setup_rollback_timer(2);//should be rollback with the first card simultaneously
					}
				}
				else//bFound==true
				{
					log("found 1=%d,2=%d", iFirstCard, iSecondCard);
					//reset it
					firstCard->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));
					//firstCard->setScale(0.95);
					firstCard->setScaleX(0.9);
					firstCard->setScaleY(0.97);
					secondCard->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));
					//secondCard->setScale(0.95);
					secondCard->setScaleX(0.9);
					secondCard->setScaleY(0.97);
					log("found it tag=%d,tag2=%d", firstCard->getTag(), secondCard->getTag());

				}
				//delete[]file;
			}
			return true;
		};

		for (int col = 0; col < iCols; col++)
		{
			float x_position = 0.0;
			if (col <= iCols / 2)
			{
				x_position = Director::getInstance()->getVisibleSize().width / 2 - (iCols / 2 - col) * 190 + 80;
			}
			else
			{
				x_position = Director::getInstance()->getVisibleSize().width / 2 + (col - iCols / 2) * 190 + 80;
			}
			for (int row = 0; row < iRows; row++)
			{
				float y_position = 0.0;
				if (row <= iRows / 2)
				{
					y_position = Director::getInstance()->getVisibleSize().height / 2 + (iRows / 2 - row) * 190 - 80;
				}
				else
				{
					y_position = Director::getInstance()->getVisibleSize().height / 2 - (row - iRows / 2) * 190 - 80;
				}
				auto card1 = Sprite::create("res/memory_game/memory_square.png");

				/*
				char file[100];
				sprintf(file, "%s", sFile[0]);
				card1->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));*/
				card1->setPosition(Vec2(x_position + 50, y_position));

				if (row == 0 && col == 0){
					Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener1, card1);
				}
				else{
					Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener1->clone(), card1);
				}
				int index = row + col*iRows;
				this->addChild(card1, 20 + index, 1001 + unique_array[index]);
				log("index=%d,z-index=%d,x=%f,y=%f", index, unique_array[index], x_position, y_position);
				spCard.pushBack(card1);

				auto card2 = Sprite::create("res/memory_game/kuang.png");
				card2->setPosition(Vec2(x_position + 50, y_position));
				this->addChild(card2, 100 + index, 1001 + unique_array[index]);
				log("index=%d,z-index=%d,x=%f,y=%f", index, unique_array[index], x_position, y_position);

			}
		}

		iFirstCard = -1;
		iSecondCard = -1;

		app->LogAccess("MemoryMatch");
		m_iCurrentGameLevel = app->getCurrentGameLevel();

		schedule(schedule_selector(GameLayer::updateLabels), 0.01f);
		schedule(schedule_selector(GameLayer::updateTimeLabel), 1.0f);

		delete[] array1;
		delete[] array2;
		delete[] unique_array;
		delete[] array4;
		//Director::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&GameLayer::loadingCallBack), this, .5f, false);


		//-----------------------------------add animation----------------------------------------------------------

		auto mastco = dynamic_cast<Sprite *> (_rootNode->getChildByName("Sprite_10"));
		//int mastcoZOrder=-1;
		if (mastco)
		{
			//mastcoZOrder = mastco->getZOrder();
			mastco->setVisible(false);
		}

		ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/1.3/1.3.ExportJson");
		Armature *armature = Armature::create("1.3");
		armature->setAnchorPoint(Vec2(0.5, 0.5));
		armature->setRotation(270);
		armature -> setPosition(Vec2(150,400));
		armature->setVisible(true);
		armature->setTag(888);
		schedule(schedule_selector(GameLayer::PlayAnimation), 5.0f);
		_rootNode->addChild(armature);
		nAniFlag = 1;
		//-------------------------------------------------------------------------------------

		_bTlEnabled = true;

		return true;
	}
	else
	{
		return false;
	}
}

void GameLayer::PlayAnimation(float dt)
{
	Armature *armature = dynamic_cast<Armature *>(_rootNode->getChildByTag(888));
	if (nAniFlag == 1)
	{
		armature->setPosition(Vec2(200, 400));
		armature->setRotation(270);
		nAniFlag = 2;
	}
	else
	{
		armature->setPosition(Vec2(800, 200));
		armature->setRotation(180);
		nAniFlag = 1;
	}
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	AppDelegate* app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_ANIMATION);
	}
}
void GameLayer::loadFirstTutorial(float dt)
{
	unschedule(schedule_selector(GameLayer::loadFirstTutorial));
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		Director::getInstance()->pushScene(scene);
	}
}

void GameLayer::CheckWin()
{
	int iWin = 0;
	TouchSprite* Card;
	for (int k = 0; k< spCard.size(); k++)
	{
		Card = (TouchSprite*)spCard.at(k);
		if (Card->getTag() == 999){
			iWin++;
		}
	}
	if (iWin == spCard.size())
	{
		//well , you win
		//Card->GameWinJump();
		Card->unschedule(schedule_selector(GameLayer::updateTimeLabel));
		Card->scheduleOnce(schedule_selector(TouchSprite::GameWinJump), 1.0f);
	}
}

void GameLayer::touchButton(Object* obj, ::ui::Widget::TouchEventType eventType)
{
	auto button = dynamic_cast<::ui::Button*>(obj);
	int tag = button->getTag();
	if (eventType == ::ui::Widget::TouchEventType::ENDED)
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		if (tag == 2001)
		{ //Tutorial
			auto scene = TutorialScene::create();
			if (scene != nullptr){
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->pushScene(transition);
			}
		}
		if (tag == 2002)
		{ //Home

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
			btnYes->addTouchEventListener(CC_CALLBACK_2(GameLayer::yesCallback, this));
			btnNo->addTouchEventListener(CC_CALLBACK_2(GameLayer::noCallback, this));

		}
		if (tag == 2003)
		{  //AI tips
			if (_bTlEnabled == false) return;
			_bTlEnabled = false;



			if (_nTlimit > 0)
			{
				if (_nTlcount > 0)
				{
					_nTlcount--;
					//AppDelegate *app = (AppDelegate*)Application::getInstance();
					app->setToolsNumberInfo(_nTlcount);
				}
				else
				{
					if (_lTtlScore >= 10)
					{
						showDeductPointEffect("txtScore");
						_lTtlScore = _lTtlScore - 10;
						//AppDelegate *app = (AppDelegate*)Application::getInstance();
						app->UpdateUserScore(0, 10);
					}
					else return;
				}
				_nTlimit--;
				if (_nTlimit == 0)
				{
					auto spTips = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTipsTools"));
					spTips->setColor(ccc3(84, 84, 84));//not disabled button, so use color instead
				}
				if (app->getBGMstatus() == BGMusic_flag::ON){
					app->PlayEffect(SOUND_TOOL_CLICK);
				}
				UseTips();
				this->scheduleOnce(schedule_selector(GameLayer::enableTools), 2.0f);
			}
			else return;
		}
	}
}

void GameLayer::UseTips()
{
	//Find a set of matched cards, Game them
	int iCardNo = 0;
	int iCardMatch = 0;

	TouchSprite* Card;
	TouchSprite* MatchCard;
	for (int i = 0; i < iMaxCard; i++)
	{
		Card = (TouchSprite*)spCard.at(i);
		if (Card->getTag() != 999)
		{
			iCardNo = i;
			break;
		}
	}
	for (int i = iCardNo + 1; i < iMaxCard; i++)
	{
		MatchCard = (TouchSprite*)spCard.at(i);
		if (Card->getTag() == MatchCard->getTag() || Card->getTag() - 1000 == MatchCard->getTag() || Card->getTag() + 1000 == MatchCard->getTag())
		{
			iCardMatch = i;
			break;
		}
	}
	//iCardMatch=FindMatch(iCardNo);
	if (iCardMatch>iCardNo)//found
	{
		int i = Card->getTag() % 1000;
		char file[100];
		//sprintf(file, "match_game_img_%d.jpg", i);
		//file[20] = '\0';
		sprintf(file, "%s", sFile[i - 1]);
		//Card = (TouchSprite*)spCard.at(iCardNo);
		//MatchCard = (TouchSprite*)spCard.at(iCardMatch);
		Card->setTag(999);
		MatchCard->setTag(999);
		Card->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));
		Card->setScaleX(0.9);
		Card->setScaleY(0.97);
		MatchCard->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(file));
		MatchCard->setScaleX(0.9);
		MatchCard->setScaleY(0.97);
	}
	CheckWin();
}

void GameLayer::updateTimeLabel(float dt)
{
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
		this->unschedule(schedule_selector(GameLayer::updateLabels));
		this->unschedule(schedule_selector(GameLayer::updateTimeLabel));
		GameFailed();
	}
}

void GameLayer::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)//quit game button
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		app->moveToNextGameLevel();

		this->unschedule(schedule_selector(GameLayer::updateLabels));
		this->unschedule(schedule_selector(GameLayer::updateTimeLabel));


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

void GameLayer::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		this->removeChildByTag(9000);
	}
}

void GameLayer::updateLabels(float dt)
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


void GameLayer::GameFailed(){
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	app->moveToNextGameLevel();

	this->unschedule(schedule_selector(GameLayer::updateLabels));
	this->unschedule(schedule_selector(GameLayer::updateTimeLabel));

	auto scene = GameFailed::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void GameLayer::showDeductPointEffect(std::string sLabel){

	auto scoreLabel = dynamic_cast<Text*> (_rootNode->getChildByName(sLabel.c_str()));
	if (scoreLabel == nullptr) return;

	//show score effect
	if (flag)
	{
		flag = 0;
		auto blink2 = Blink::create(1.5, 3);
		auto tintto = TintTo::create(0.01f, 255.0f, 0.0f, 0.0f);
		auto tintback = TintTo::create(0.01f, 255.0f, 255.0f, 255.0f);
		auto seq1 = Sequence::create(tintto, blink2, tintback, nullptr);
		scoreLabel->runAction(seq1);
		flag = 1;
	}
	

	return;
}


void GameLayer::enableTools(float dt){
	_bTlEnabled = true;
}
