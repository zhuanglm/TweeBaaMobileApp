#include "PictureGameLayer.h"
#include "jigSawScene.h"
#include "global_const.h"
#include "GameWinScene.h"
#include "TweebaaMobileLayer.h"
#include "AppDelegate.h"
#include "WelcomeScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameFailedScene.h"
#include "../utilits/VideoPlayer.h"
#include "../utilits/Tutorial.h"
#include "../Msgbox/Dialogbox.h"
#include "../Msgbox/MsgBox.h"
#include "cocostudio/CocoStudio.h"//for animation
#include "theme/ThemeLayer.h"

using namespace ui;
using namespace cocostudio;

//extern char jigSawFile[100];
float TILE_SQUARE_SIZE = 0.0f;

int NUM_ROWS = 3;
int NUM_COLUMNS = 3;
int num_max_title=9;
int nRand = 0;
char jSawFile[100] = "\0";
extern int iTotalTime;
PictureGameLayer::PictureGameLayer ()
{
	BlockMoved=0;
};
PictureGameLayer::~PictureGameLayer ()
{
};	
void PictureGameLayer::UpdateBlockMoveFlag(float dt)
{
	BlockMoved=0;
}
void PictureGameLayer::onTouchesMoved(const vector<Touch*>& touches, Event* event)
{
	//move to touch end
	for (auto& touch : touches)
	{
		// Touch* touch=touches.back();
		if (touch && BlockMoved==0 )
		{
			BlockMoved=1;
			scheduleOnce(schedule_selector(PictureGameLayer::UpdateBlockMoveFlag), 0.5f );
			Rect* spritePos ;
			Vec2 p = touch->getLocation();

			//prevBlockMoved=p;
			log("touch comes x=%f,y=%f",p.x,p.y);
			CCNode* tilesNode = (CCNode*) this->getChildByTag(TILE_NODE_TAG) ;
			//We loop through each of the tiles and get its cordinates
			for (int i = 1 ; i < (NUM_ROWS * NUM_COLUMNS); i++)
			{
				CCNodeExt* eachNode = (CCNodeExt*) tilesNode->getChildByTag(i) ;
				//we construct a rectangle covering the current tiles cordinates
				spritePos=new Rect();
				spritePos ->setRect(
						eachNode->getPosition().x - (eachNode->getContentSize().width*generalscalefactor/2.0f),
						eachNode->getPosition().y - (eachNode->getContentSize().height*generalscalefactor/2.0f),
						eachNode->getContentSize().width*generalscalefactor   ,
						eachNode->getContentSize().height*generalscalefactor   );
				//Check if the user's touch falls inside the current tiles cordinates
				if(spritePos->containsPoint(p))
				{
					//ccMacros.CCLOG("Began Touched Node", "Began touched : " + eachNode.getNodeText());
					log("Began Touched Node");
					slideCallback(eachNode); // if yes, we pass the tile for sliding.
					break;
				}
				spritePos=NULL;
				delete spritePos;
			}
		}
	}
}

bool PictureGameLayer::init()
{
	iTotalTime = 360;

	if(!Layer::init())
	{
		return false;
	}
	thetime=0;moves=0;

	//only 3 x3 in stage 1
	NUM_ROWS = 3; NUM_COLUMNS = 3; num_max_title = 9; 
	
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(PictureGameLayer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(PictureGameLayer::onTouchesMoved, this);

	auto dispatcher = this->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	screenSize = Director::getInstance()->getVisibleSize();
	
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	 std::string sCSD=app->GetLayoutString();
	_rootNode = CSLoader::createNode("res/pixel_shuffle/JigsawLayer"+sCSD+".csb");

	addChild(_rootNode,1);
	m_iCurrentGameLevel = app->getCurrentGameLevel();

	auto puzzle_bg_12 = dynamic_cast<Sprite*>(_rootNode->getChildByName("puzzle_bg_12"));

	auto btnTips = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTips"));
	if (btnTips){
		 btnTips->setTag(2001);
		 btnTips->addTouchEventListener(CC_CALLBACK_2(PictureGameLayer::touchButton, this));
	}


	auto button_Main = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("button_Main"));
	if (button_Main){
		 button_Main->setTag(2002);
		 button_Main->addTouchEventListener(CC_CALLBACK_2(PictureGameLayer::touchButton, this));
	}
	auto btnTipsTools = dynamic_cast<::ui::Button*>(_rootNode->getChildByName("btnTipsTools"));
	if (btnTipsTools){
		 btnTipsTools->setTag(2003);
		 btnTipsTools->addTouchEventListener(CC_CALLBACK_2(PictureGameLayer::touchButton, this));
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
	auto lvlbar_bg= dynamic_cast<Sprite*> (_rootNode->getChildByName("lv_bg_3"));
	lvlbar->setAnchorPoint(Vec2(0, 0.5));
	lvlbar->setScaleX(_fPerToNLvl);
	
	ImageView* ivSample = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivSample")); 

	srand((unsigned)time(0));//
	int random = rand();
	nRand = rand() % MAX_LEVEL_JIGSAW;
	sprintf(jSawFile, IMAGE_PATH);
	strcat(jSawFile, sJigsaw[nRand]);


	auto card1 = Sprite::create(jSawFile);
	card1->setScale(0.095f);

	if (ivSample)
	{
		card1->setPosition(Vec2(ivSample->getContentSize().width / 2, ivSample->getContentSize().height / 2));
		ivSample->addChild(card1, 1, 9001);
	}
	
	schedule(schedule_selector(PictureGameLayer::updateLabels), 0.01f);
	schedule(schedule_selector(PictureGameLayer::updateTimeLabel), 1.0f);
	generateTiles();

	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->StartBGMusic(BGM_GAME);
	}

	app->LogAccess("JigSaw");

	//set _bTlEnabled to true when init the layer
	//-----------------------------------add animation----------------------------------------------------------

	auto mastco = dynamic_cast<Sprite *> (_rootNode->getChildByName("mastco"));
	//int mastcoZOrder=-1;
	if (mastco)
	{
		//mastcoZOrder = mastco->getZOrder();
		mastco->setVisible(false);
	}
	auto Board = dynamic_cast<Sprite *> (_rootNode->getChildByName("Sprite_2"));
	auto Img = dynamic_cast<ImageView*> (_rootNode->getChildByName("ivSample"));
	Board->setZOrder(1);
	Img->setZOrder(2);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("res/animations/1.1/1.1.ExportJson");
	Armature *armature = Armature::create("1.1");
	armature->setAnchorPoint(Vec2(0.5, 0.5));
	//armature->setPosition(Vec2(825, 453));
	//armature->setScale(1.2);
	//armature->setZOrder(mastcoZOrder);
	armature->setTag(888);

	nAniFlag = 1;
	schedule(schedule_selector(PictureGameLayer::PlayAnimation), 5.0f);
	_rootNode->addChild(armature);

	_bTlEnabled = true;
	return true;
}
void PictureGameLayer::PlayAnimation(float dt)
{
	Armature *armature = dynamic_cast<Armature *>(_rootNode->getChildByTag(888));
	if (nAniFlag == 1)
	{
		armature->setPosition(Vec2(825, 420));
		armature->setRotation(0);
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
void PictureGameLayer::loadFirstTutorial(float dt)
{
	unschedule(schedule_selector(PictureGameLayer::loadFirstTutorial));
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		Director::getInstance()->pushScene(scene);
	}
}

void PictureGameLayer::touchButton(Object* obj,::ui::Widget::TouchEventType eventType)  
{  
    auto button = dynamic_cast<::ui::Button*>(obj);  
    int tag = button->getTag();  
	if (eventType == ::ui::Widget::TouchEventType::ENDED)
    {  
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		if(tag==2001)
		{ //Tutorial
			auto scene = TutorialScene::create();
			if (scene != nullptr)
			{
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->pushScene(transition);
			}
		}
		if(tag==2002)
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
			btnYes->addTouchEventListener(CC_CALLBACK_2(PictureGameLayer::yesCallback, this));
			btnNo->addTouchEventListener(CC_CALLBACK_2(PictureGameLayer::noCallback, this));
		}
		if(tag==2003)
		{//Tips
			if (_bTlEnabled == false) return;
			_bTlEnabled = false;
			if (_nTlcount > 0)
			{
				_nTlcount--;
				AppDelegate *app = (AppDelegate*)Application::getInstance();
				app->setToolsNumberInfo(_nTlcount);
			}
			else
			{
				showDeductPointEffect("txtScore");
				if (_lTtlScore >= 10){
					_lTtlScore = _lTtlScore - 10;
					AppDelegate *app = (AppDelegate*)Application::getInstance();
					app->UpdateUserScore(0, 10);
				}
				else{
					return;
				}
			}
			if (app->getBGMstatus() == BGMusic_flag::ON){
				app->PlayEffect(SOUND_TOOL_CLICK);
				SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			}
			//this->scheduleOnce(schedule_selector(PictureGameLayer::enableTools), 2.0f);

            this->unschedule(schedule_selector(PictureGameLayer::updateTimeLabel));           
			auto scene = VideoPlayer::createScene(VIDEO_KITTYLITTY, 1);
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);

			//AppDelegate *app = (AppDelegate*)Application::getInstance();
		}
	}
}
void PictureGameLayer::updateTimeLabel(float dt) 
{
	iTotalTime -= 1;
	char lblTimer[100];
	sprintf(lblTimer, "%d:%02d", (int)(iTotalTime / 60), (int)iTotalTime % 60);
	string string1 = lblTimer;
	auto timerLabel=dynamic_cast<::ui::Text*>(_rootNode->getChildByName("lblTimer"));
	timerLabel->setString(string1);
	if (iTotalTime == 270)
	{
		ImageView* ivSample = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivSample")); 
		ivSample->removeFromParentAndCleanup(true);
	}
	if (iTotalTime>0 && iTotalTime < 10) 
	{
		timerLabel->setColor(ccc3(255, 0, 0));
	}
	if (iTotalTime <= 0)
	{
		//game over
		this->unschedule(schedule_selector(PictureGameLayer::updateTimeLabel));

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->moveToNextGameLevel();

		auto scene = GameFailed::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void PictureGameLayer::generateTiles()
{
	//We create a Node element to hold all our tiles
	
	CCNode* tilesNode = CCNode::create();
	tilesNode->setTag(TILE_NODE_TAG);
	
	this->addChild(tilesNode,15); 	
	//this->addChild(tilesNode,15); 	
	float scalefactor ;   // a value we compute to help scale our tiles
	int useableheight  ;	
	int tileIndex = 0 ;

	//We attempt to calculate the right size for the tiles given the screen size and 
	//space left after adding the status label at the top
	int nextval ;

	int nRand = rand() % 16;//16 sets of solvable arranges
	int* tileNumbers=new int[num_max_title];
	for (int i = 0; i < num_max_title; i++)
	{
		if (num_max_title == 9)
		{
			tileNumbers[i] = _szSolvable9[nRand][i];
		}
	}

	if(screenSize.width < 960)
	{
		generalscalefactor=0.6f;
	}
	else
	{
		generalscalefactor=1.034f;
	}
	//if(num_max_title>9) generalscalefactor=generalscalefactor*1.3;
	int useablewidth = (int) (404*generalscalefactor ) ;
	useableheight =  (int) (404*generalscalefactor) ;

	TILE_SQUARE_SIZE = (int) min((useableheight/NUM_ROWS) , (useablewidth/NUM_COLUMNS)) ;
	
	//ccMacros.CCLOG("Began", " Scale Fact " + generalscalefactor + " :  "  );

	
	toppoint = (int) (useableheight  - (TILE_SQUARE_SIZE / 2) +106* generalscalefactor);
	if(num_max_title==9)
	{
		scalefactor = TILE_SQUARE_SIZE / (150.0f*generalscalefactor) ;
	}
	else if(num_max_title==16)
	{
		scalefactor = TILE_SQUARE_SIZE / (114.0f*generalscalefactor) ;
	}
	else if(num_max_title==25)
	{
		scalefactor = TILE_SQUARE_SIZE / (92.0f*generalscalefactor) ;
	}
	if(screenSize.width < 960)
	{
		topleft = (int) ((TILE_SQUARE_SIZE / 2*scalefactor) +304*generalscalefactor) ;
	}
	else
	{
		topleft = (int) ((TILE_SQUARE_SIZE / 2*scalefactor) +154*generalscalefactor) ;
	}
	Sprite* tile = Sprite::create("tile.png");
	//Sprite tilebox = Sprite.sprite("tilebox.png");
	int Space = TILE_SQUARE_SIZE + _XBlank; 
	int ySpace = TILE_SQUARE_SIZE + _YBlank;
	for (int j = toppoint; j > toppoint - (TILE_SQUARE_SIZE * NUM_ROWS); j -= ySpace)
	{
		for (int i = topleft; i < (topleft - 5 * generalscalefactor) + (TILE_SQUARE_SIZE * NUM_COLUMNS); i += Space)
		{
			if (tileIndex >= (NUM_ROWS * NUM_COLUMNS)) 
			{
				break ;
			}
			nextval = tileNumbers[tileIndex];
			CCNodeExt* eachNode =  new  CCNodeExt(); 
			eachNode->setContentSize(tile->getContentSize());
			eachNode->setScale(scalefactor);
			//
			//Layout Node based on calculated postion
			eachNode->setPosition(i, j);
			char nText[10];
			sprintf(nText, "%d", nextval);
			nText[1]='\0';
			//eachNode->setNodeText(nText);

			if( nextval != 0)
			{
				tilesNode->addChild(eachNode,1,nextval);
			}
			else 
			{
				emptyPosition = Vec2(i, j);
			}
			tileIndex++;
		}
	} 
	
	//Add Picture Sprites as tile background 
	
	CCNode* tileNode = (CCNode*) this->getChildByTag(TILE_NODE_TAG);
	int nodeindex = 1 ;
	CCTexture2D* metexture = new CCTexture2D();
	CCImage*  mybit = new CCImage();

	
	mybit->initWithImageFile(jSawFile);

	metexture->initWithImage(mybit);

	for (float j = 0 ; j < NUM_ROWS ; j++)
	{
		for (float i = 0 ; i <NUM_COLUMNS; i++)
		{

			//Calculate the size of each tile by dividing the height and width of our image
			// and returning the min value of both calculations
			float theblock = min( mybit->getHeight()/NUM_ROWS, mybit->getWidth()/NUM_COLUMNS) ;

			//Create a new sprite using this dimension above and from a given portion of the image
			Rect rcBlock=Rect(i*theblock, j*theblock, theblock, theblock);
			SpriteFrame* myframe = SpriteFrame::createWithTexture(metexture, rcBlock);
			tile = Sprite::createWithSpriteFrame(myframe);
			tile->setScale((TILE_SQUARE_SIZE/theblock) * (1.2f*scalefactor));

			CCNode* nodeChild=(CCNode*)tileNode->getChildByTag(nodeindex);
			nodeChild->addChild(tile,-1,1); 
			tileNode->setContentSize(tile->getContentSize());

			if(nodeindex == (NUM_ROWS * NUM_COLUMNS) - 1)
			{
				break ;
			} 
			nodeindex++ ;
		}
	}	
	delete []tileNumbers;
}
	
void PictureGameLayer::slideCallback(CCNodeExt* thenode) 
{
	Vec2 nodePosition = thenode->getPosition();

	//Determine the position to slide the tile to .. ofcourse only if theres an empty space beside it
	int xSpace = TILE_SQUARE_SIZE + _XBlank;
	int ySpace = TILE_SQUARE_SIZE + _YBlank;

	if ((nodePosition.x - xSpace) == emptyPosition.x && nodePosition.y == emptyPosition.y)
	{
		slideTile(1, thenode,true); //"Left"
	}
	else if ((nodePosition.x + xSpace) == emptyPosition.x && nodePosition.y == emptyPosition.y)
	{
		slideTile(3, thenode,true); //"Right"
	}
	else if ((nodePosition.x) == emptyPosition.x && nodePosition.y == (emptyPosition.y + ySpace))
	{
		slideTile(4, thenode,true); //"Down"
	}
	else if ((nodePosition.x) == emptyPosition.x && nodePosition.y == (emptyPosition.y - ySpace))
	{
		slideTile(2, thenode,true); //"Up"
	}
	else
	{
		slideTile(0, thenode,false); //"Unmovable"
	}
	
}

void PictureGameLayer::slideTile(int direction, CCNodeExt* thenode, bool move)
{ 
	if(move && !gameover)
	{ 
		//  Increment the moves label and animate the tile
		moves ++ ;
		
		Vec2 nodePosition = thenode->getPosition();
		Vec2 tempPosition = emptyPosition ;

		ImageView* ivBoard = dynamic_cast<ImageView*>(_rootNode->getChildByName("ivBoard")); 
		float x1= ivBoard->getPositionX()-ivBoard->getContentSize().width/2;
		float x2 = ivBoard->getPositionX()+ivBoard->getContentSize().width/2;
		float y1= ivBoard->getPositionY()-ivBoard->getContentSize().height/2;
		float y2=ivBoard->getPositionY()+ivBoard->getContentSize().height/2;

		//根据方向来判断坐标是否正确
		int m_iIsValid=0;
		float x3=nodePosition.x-tempPosition.x;
		float y3=nodePosition.y-tempPosition.y;

		if(direction==1)
		{ //"Left"
			//向左移,Y不变，X+= TILE_SQUARE_SIZE
			if(y3==0 && x3==TILE_SQUARE_SIZE) m_iIsValid=1;
		}
		else if(direction==3)
		{//"Right"
			//向右移,Y不变，X-= TILE_SQUARE_SIZE
			if(y3==0 && x3 + TILE_SQUARE_SIZE==0) m_iIsValid=1;
		}
		else if(direction==4)
		{ //"Down"
			//向下移,X不变，Y-= TILE_SQUARE_SIZE
			if(x3==0 && y3==TILE_SQUARE_SIZE) m_iIsValid=1;
		}
		else if(direction==2)
		{  //"Up"
			//向上移,X不变，Y+= TILE_SQUARE_SIZE
			if(x3==0 && y3+TILE_SQUARE_SIZE==0) m_iIsValid=1;
		}

		log("direction=%d,move to %f,%f,original point %f,%f,x1=%f,y1=%f",direction,tempPosition.x,tempPosition.y,
		nodePosition.x,nodePosition.y,x3,y3);

		thenode->setPosition(tempPosition);
		emptyPosition = nodePosition ;
	}
    //handleWin(thenode);
	checkCorrect();
}

//This method checks if the puzzle has been correctly solved.
bool PictureGameLayer::checkCorrect()
{
	bool result = false; 
	CCNode* tileNode = (CCNode*)this->getChildByTag(TILE_NODE_TAG);
	int nodeindex = 1 ;

	for (float j = toppoint ; j > toppoint - ((TILE_SQUARE_SIZE) * NUM_ROWS); j-= TILE_SQUARE_SIZE+_YBlank)
	{
		for (float i = topleft ; i < (topleft - 5) + (TILE_SQUARE_SIZE * NUM_COLUMNS); i+= TILE_SQUARE_SIZE+_XBlank)
		{
			//Vec2 node_point = ;
			if(tileNode->getChildByTag(nodeindex)->getPosition().x == i && tileNode->getChildByTag(nodeindex)->getPosition().y == j )
			{
				result = true ; 
			}
			else
			{ 
				return false ;
			}
			nodeindex++ ;
			if(nodeindex == (NUM_ROWS * NUM_COLUMNS))
			{
				if (result)
				{
					scheduleOnce(schedule_selector(PictureGameLayer::GameWinJump), 1.0f);
				}
				return result ;
			}
		}
	}
	return result ;
}

void PictureGameLayer::GameWinJump(float dt)
{
	int score = 0;
	int ttlseconds = iTotalTime ;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if(app->iGameMode==MobileGameMode::Theme_Mode)
	{
		app->GameRunRoundNext();
	}
	else
	{
		//save game data
		app->setPromoteProduct("");
		app->updateGameWinScore(ttlseconds);
		app->SaveGamePassLevel();//Add by Long to Save Level
		app->moveToNextGameLevel();

		auto scene = GameWin::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}
void PictureGameLayer::updateLabels(float dt)
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

void PictureGameLayer::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		app->moveToNextGameLevel();

		this->unschedule(schedule_selector(PictureGameLayer::updateLabels));
		this->unschedule(schedule_selector(PictureGameLayer::updateTimeLabel));

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

void PictureGameLayer::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->PlayEffect(SOUND_BUTTON_CLICK);
		//Don't quit the game
		this->removeChildByTag(9000);
		//enable the touch event
	}
}

void PictureGameLayer::GameFailed()
{
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	app->moveToNextGameLevel();

	this->unschedule(schedule_selector(PictureGameLayer::updateLabels));
	this->unschedule(schedule_selector(PictureGameLayer::updateTimeLabel));

	auto scene = GameFailed::createScene();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void PictureGameLayer::showDeductPointEffect(std::string sLabel){

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

void PictureGameLayer::enableTools(float dt){
	_bTlEnabled = true;
}