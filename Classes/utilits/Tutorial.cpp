#include "Tutorial.h"
#include "../HelloWorldScene.h"
#include "ChooseLayer.h"
#include "../Collage/CollageScene.h"
#include "../utilits/NetworkUtil.h"
#include "AppDelegate.h"
#include "global_const.h"

#include "memory_match/GameScene.h"
#include "jigsaw/jigSawScene.h"
#include "MultiPriceMatch/MultiPriceMatchScene.h"
#include "PriceMatch/PriceMatchScene.h"
#include "find_diff/FindDiffScene.h"
#include "zoom/ZoomGameScene.h"

//#include "befool/SimpleVideoEngine.h"
int TutorialScene::_nIsIngame = 0;

TutorialScene::TutorialScene()
{

};

TutorialScene::~TutorialScene(){};

bool TutorialScene::init(){
	bool bRet = false;
	do{
		//_ttl_time = 0;
		CC_BREAK_IF(!Scene::init());
		 //befool::SimpleVideoEngine::getShared()->playVideo("renai-circulation.mp4");
		UserDefault *load = UserDefault::getInstance();
		_nPLayPre = load->getIntegerForKey("Tutorial_PlayPreStep", 0);

		//load->setIntegerForKey("LevelInfoFlag", 1);

		//if (!_UINode) {
		//	bRet = false;
		//	break;
		//}
		//this->addChild(_UINode);
		//this->schedule(schedule_selector(TutorialScene::update), 0.01f);

	_nCurrentStep = 1;
	_nSteps = 6;	

	Size visiblesize = Director::getInstance()->getVisibleSize();
	char szName[100]={0};
	std::string pList;
	std::string pImage;
	char szName2[100]={0};
	std::string pList2;
	std::string pImage2;

	log("***********TutorialScene::init*********** m_iCurrentGameID = %d ", m_iCurrentGameID);
	AppDelegate* app = (AppDelegate*)Application::getInstance();

	if (m_iCurrentGameID == -2)
	{
		_nSteps = 2;
		sprintf(szName, THEME_FRAME, _nCurrentStep);
	}
	if (m_iCurrentGameID == -1){

		_nSteps = 12;
		//pList = COLLAGE_TUTORIAL_PLIST;
		//pImage = COLLAGE_TUTORIAL_IMAGE;
		sprintf(szName, COLLAGE_FRAME, _nCurrentStep);

	}
	if (m_iCurrentGameID == 0){

		_nSteps = 6;
		//pList = SPOT_TUTORIAL_PLIST;
		//pImage = SPOT_TUTORIAL_IMAGE;
		sprintf(szName, TIPS2WIN_FRAME, _nCurrentStep);
	}
	if (m_iCurrentGameID == 1){

		_nSteps = 5;
		//pList = SPOT_TUTORIAL_PLIST;
		//pImage = SPOT_TUTORIAL_IMAGE;
		sprintf(szName,SPOT_FRAME,_nCurrentStep);
	}
	if (m_iCurrentGameID == 2){
		_nSteps = 4;	
		//pList = ZOOM_TUTORIAL_PLIST;
		//pImage = ZOOM_TUTORIAL_IMAGE;
		sprintf(szName,ZOOM_FRAME,_nCurrentStep);
	}
	if (m_iCurrentGameID == 3){
		_nSteps = 4;	
		//pList = SHUFFLE_TUTORIAL_PLIST;
		//pImage = SHUFFLE_TUTORIAL_IMAGE;
		sprintf(szName,SHUFFLE_FRAME,_nCurrentStep);
	}
	if (m_iCurrentGameID == 4){
		_nSteps = 3;	
		//pList = FLIP_TUTORIAL_PLIST;
		//pImage = FLIP_TUTORIAL_IMAGE;
		sprintf(szName,FLIP_FRAME,_nCurrentStep);
	}
	if (m_iCurrentGameID == 5){
		_nSteps = 3;	
		//pList = MATCH_TUTORIAL_PLIST;
		//pImage = MATCH_TUTORIAL_IMAGE;
		sprintf(szName,MATCH_FRAME,_nCurrentStep);
	}
	if (m_iCurrentGameID == 6){
		_nSteps = 4;	
		//pList = LINK_TUTORIAL_PLIST;
		//pImage = LINK_TUTORIAL_IMAGE;
		sprintf(szName,LINK_FRAME,_nCurrentStep);
	}

	//log("***********TutorialScene::init***********  pList path = %s\n",pList.c_str());

	//log("***********TutorialScene::init***********  pList fullPathForFilename = %s\n",	FileUtils::getInstance()->fullPathForFilename(pList.c_str()).c_str());
	//CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(pList));

	//log("***********TutorialScene::init***********  pImage path = %s\n",pImage.c_str());
	//CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(pImage));


	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pList);
	//log("***********TutorialScene::init*********** FileExist");
	//auto testBatchNode=SpriteBatchNode::create(pImage);
	//testBatchNode->setPosition(visiblesize.width/2, visiblesize.height/2);
	//testBatchNode->setTag(2001);
	//this->addChild(testBatchNode);

	//auto testSprite=Sprite::createWithSpriteFrameName(szName);
	//testSprite->setTag(2002);
	////testSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ZOOM_step3.jpg"));
	//testBatchNode->addChild(testSprite);

	auto pic = Sprite::create(szName);
	pic->setTag(2001);
	pic->setZOrder(9);
	pic->setAnchorPoint(Vec2(0.5,0.5));
	pic->setPosition(visiblesize.width/2, visiblesize.height/2);
	this->addChild(pic);

	//auto animation=Animation::create();
	//for(int i=1;i<=6;i++)
	//{
	//	char szName[100]={0};
	//	sprintf(szName,"ZOOM_step%d.jpg",i);
	//	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(szName));
	//}
	//animation->setDelayPerUnit(1.0f); 
	////testSprite->runAction(RepeatForever::create(Animate::create(animation))); 
	//testSprite->runAction(RepeatForever::create(Animate::create(animation))); 
	//testBatchNode->addChild(testSprite);

	/*
	auto btnContinue = Button::create();
	btnContinue->setTouchEnabled(true);
	btnContinue->setTitleText("NEXT");
	btnContinue->setTitleFontSize(BUTTON_FONTSIZE);
	btnContinue->setTitleFontName("res/FONT/HUM521B.TTF");
	btnContinue->setTitleColor(ccc3(224,224,224));
	btnContinue->setPosition(Vec2(visiblesize.width-80, 50));
	btnContinue->addTouchEventListener(CC_CALLBACK_2(TutorialScene::btnContinue, this));
	btnContinue->setTag(2003);
	btnContinue->setZOrder(99);
	this->addChild(btnContinue);*/

	auto btnSkip = Button::create();
	btnSkip->setTouchEnabled(true);
	btnSkip->setTitleText("SKIP");
	btnSkip->setTitleFontSize(BUTTON_FONTSIZE);
	btnSkip->setTitleFontName("res/FONT/HUM521B.TTF");
	btnSkip->setTitleColor(ccc3(224,224,224));
	btnSkip->setPosition(Vec2(visiblesize.width-80, visiblesize.height-50));
	btnSkip->addTouchEventListener(CC_CALLBACK_2(TutorialScene::btnQuit, this));
	btnSkip->setTag(2005);
	btnSkip->setZOrder(100);
	this->addChild(btnSkip);
	auto btnContinue = Button::create();
	//btnContinue->setTouchEnabled(true);
	btnContinue->setEnabled(false);
	btnContinue->setTitleText("");
	btnContinue->loadTextures("res/public/bg.png", "res/public/bg.png", "res/public/bg.png");
	btnContinue->setAnchorPoint(Vec2(0.5, 0.5));
	btnContinue->setPosition(Vec2(visiblesize.width / 2, visiblesize.height / 2));
	btnContinue->setScale(20);
	btnContinue->setTag(2003);
	btnContinue->setZOrder(99);
	btnContinue->addTouchEventListener(CC_CALLBACK_2(TutorialScene::btnContinue, this));
	this->addChild(btnContinue);
	scheduleOnce(schedule_selector(TutorialScene::enableBtn), 0.5f);


	bRet = true;

	}while(0);

	if (bRet == false){
		
	}
	return bRet;
}
void TutorialScene::enableBtn(float dt)
{
	auto btnContinue = dynamic_cast<Button*>(this->getChildByTag(2003));
	btnContinue->setEnabled(true);
}
void TutorialScene::btnQuit(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate* app = (AppDelegate*)Application::getInstance();
		if (m_iCurrentGameID == -2)
		{
			string sIsFirst = DataAccess::getParamValue(TutorialStory[app->iThemeID-1], "\0");
			if (sIsFirst.length() == 0)
			{
				DataAccess::setParamValue(TutorialStory[app->iThemeID - 1], "1");
				auto scene = ThemeLayer::createScene();
				TransitionScene *transition = TransitionFade::create(1, scene);
				Director::getInstance()->replaceScene(transition);
			}
			else
			{
				Director::getInstance()->popScene();
			}
		}
		else if(m_iCurrentGameID == -1){
			HttpConnection::bTutorialQuitFlag=true;
			Director::getInstance()->popScene();
		}else if(m_iCurrentGameID == 0){
			Director::getInstance()->popScene();
		}else {

		//UserDefault *load = UserDefault::getInstance();
		//string sIsFirst = load->getStringForKey(TutorialIsFirstTime[m_iCurrentGameID],"0");

		string sIsFirst = DataAccess::getParamValue(TutorialIsFirstTime[m_iCurrentGameID], "0");

		if (atoi(sIsFirst.c_str()) == 0){
			//load->setStringForKey(TutorialIsFirstTime[m_iCurrentGameID],"1");
			DataAccess::setParamValue(TutorialIsFirstTime[m_iCurrentGameID], "1");

			if (m_iCurrentGameID == 1){
				auto scene = FindDiffScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}			
			}
			if (m_iCurrentGameID == 2){
				auto scene = ZoomGameScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}			
			}
			if (m_iCurrentGameID == 3){
				auto scene = jigSawScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}			
			}
			if (m_iCurrentGameID == 4){
				auto scene = GameScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}			
			}
			if (m_iCurrentGameID == 5){
				auto scene = PriceMatchScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}			
			}
			if (m_iCurrentGameID == 6){
				auto scene = MultiPriceMatchScene::create();
				if(scene == nullptr){ 
					MessageBox(MSG_ERROR_GEN, "Tweebaa");
				}else{
					TransitionScene *transition = TransitionFade::create(1, scene);
					Director::getInstance()->replaceScene(transition);
				}
			}

		}else{
			//HttpConnection::bTutorialQuitFlag=true;
			Director::getInstance()->popScene();		
		}
	}
	}
	return;
}

void TutorialScene::btnContinue(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType != cocos2d::ui::Widget::TouchEventType::ENDED) return;

	do{
		auto btnContinue = dynamic_cast<::ui::Button*>(this->getChildByTag(2003));
		btnContinue->setEnabled(false);
		scheduleOnce(schedule_selector(TutorialScene::enableBtn), 0.5f);

		//MessageBox("This is button","OK");
		if (_nCurrentStep < _nSteps) {
			_nCurrentStep++;
		}

		Size visiblesize = Director::getInstance()->getVisibleSize();
		//auto sp = (Sprite*) this->getChildByTag(2001);
		//auto displaySp = (Sprite*) sp->getChildByTag(2002);
		//auto testSprite=Sprite::createWithSpriteFrameName("ZOOM_step4.jpg");
		char szName[100]={0};
		//if (m_iCurrentGameID == 0){
		//	sprintf(szName,PRESTEP_FRAME,_nCurrentStep);
		//	//sprintf(szName,ZOOM_FRAME,_nCurrentStep);
		//}
		AppDelegate* app = (AppDelegate*)Application::getInstance();
		if (m_iCurrentGameID == -2)
		{
			sprintf(szName, THEME_FRAME, _nCurrentStep);
		}
		if (m_iCurrentGameID == -1){
			sprintf(szName, COLLAGE_FRAME, _nCurrentStep);
		}
		if (m_iCurrentGameID == 0){
			sprintf(szName, TIPS2WIN_FRAME, _nCurrentStep);
		}
		if (m_iCurrentGameID == 1){

			sprintf(szName,SPOT_FRAME,_nCurrentStep);
		}
		if (m_iCurrentGameID == 2){
			sprintf(szName,ZOOM_FRAME,_nCurrentStep);
		}
		if (m_iCurrentGameID == 3){
			sprintf(szName,SHUFFLE_FRAME,_nCurrentStep);
		}
		if (m_iCurrentGameID == 4){
			sprintf(szName,FLIP_FRAME,_nCurrentStep);
		}
		if (m_iCurrentGameID == 5){
			sprintf(szName,MATCH_FRAME,_nCurrentStep);
		}
		if (m_iCurrentGameID == 6){
			sprintf(szName,LINK_FRAME,_nCurrentStep);
		}

		this->removeChildByTag(2001);
		auto pic = Sprite::create(szName);
		pic->setTag(2001);
		pic->setPosition(visiblesize.width/2, visiblesize.height/2);
		pic->setZOrder(9);
		this->addChild(pic);
		this->getChildByTag(2003)->setZOrder(99);
		//this->getChildByTag(2005)->setZOrder(99);

		//sprintf(szName,"ZOOM_step%d.jpg",_nCurrentStep);
		//displaySp->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName));
		//sp->addChild(testSprite);	

		if (_nCurrentStep == _nSteps)
		{
			/*if (m_iCurrentGameID == 1)
			{
				if (_nPLayPre == 0)
				{
						_nPLayPre = 1;
						_nSteps = 7;
						_nCurrentStep = 1;

						char szName[100]={0};
						sprintf(szName,SPOT_FRAME,_nCurrentStep);

						this->removeChildByTag(2001);
						auto pic = Sprite::create(szName);
						pic->setTag(2001);
						pic->setPosition(visiblesize.width/2, visiblesize.height/2);
						pic->setZOrder(9);
						this->addChild(pic);
						this->getChildByTag(2003)->setZOrder(99);
						//this->getChildByTag(2005)->setZOrder(99);

						UserDefault *load = UserDefault::getInstance();
						load->setIntegerForKey("Tutorial_PlayPreStep", 1);
				}else{
					Size visiblesize = Director::getInstance()->getVisibleSize();
					this->getChildByTag(2003)->setVisible(false);
					//this->getChildByTag(2005)->setVisible(false);
					auto btnQuit = Button::create();
					btnQuit->setTouchEnabled(true);
					btnQuit->setTitleText("QUIT");
					btnQuit->setTitleFontSize(BUTTON_FONTSIZE);
					btnQuit->setTitleFontName("res/FONT/HUM521B.TTF");
					btnQuit->setTitleColor(ccc3(224,224,224));
					btnQuit->setZOrder(99);
					//btnQuit->loadTextures("res/public/quit_btn.png", "res/public/quit_btn_press.png", "");
					btnQuit->setPosition(this->getChildByTag(2003)->getPosition());
					btnQuit->addTouchEventListener(CC_CALLBACK_2(TutorialScene::btnQuit, this));
					this->addChild(btnQuit);				
				}
			}
			else{

				Size visiblesize = Director::getInstance()->getVisibleSize();
				this->getChildByTag(2003)->setVisible(false);
				this->getChildByTag(2005)->setVisible(false);
				auto btnQuit = Button::create();
				btnQuit->setTouchEnabled(true);
				btnQuit->setTitleText("QUIT");
				btnQuit->setTitleFontSize(BUTTON_FONTSIZE);
				btnQuit->setTitleFontName("res/FONT/HUM521B.TTF");
				btnQuit->setTitleColor(ccc3(224,224,224));
				btnQuit->setZOrder(99);
				//btnQuit->loadTextures("res/public/quit_btn.png", "res/public/quit_btn_press.png", "");
				btnQuit->setPosition(this->getChildByTag(2003)->getPosition());
				btnQuit->addTouchEventListener(CC_CALLBACK_2(TutorialScene::btnQuit, this));
				this->addChild(btnQuit);
			
			}*/
				auto btnContinue = dynamic_cast<Button*>(this->getChildByTag(2003));
				btnContinue->addTouchEventListener(CC_CALLBACK_2(TutorialScene::btnQuit, this));
		}
	}while(0);

	return;


	//auto director = Director::getInstance();
	//auto scene = CollageScene::createScene();
	//director->replaceScene(scene);
	//getEventDispatcher()->removeCustomEventListeners(APP_CE_OK);
}
void TutorialScene::loadingCallBack(Texture2D *texture){
	//AtlasLoader::getInstance()->loadAtlas("zoom_atlas.txt", texture);

}
void TutorialScene::update(float dt) {

		//auto director = Director::getInstance();
		//auto scene = ChooseLayer::createScene();
		//director->replaceScene(scene);
		//this->unschedule(schedule_selector(TutorialScene::update));

	DataAccess::getParamValue("","");
	//}
}

