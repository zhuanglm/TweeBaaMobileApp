#include "FindDiffLayer.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "AppDelegate.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "../MultiPriceMatch/MultiPriceMatchScene.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "../Msgbox/Dialogbox.h"
#include "../Msgbox/MsgBox.h"
#include "../LevelSelect/LevelSelectLayer.h"
#include "../utilits/Tutorial.h"
#include "cocostudio/CocoStudio.h"//for animation
#include "theme/ThemeLayer.h"

using namespace ui;
using namespace cocostudio;

FindDiffLayer::FindDiffLayer(){};

FindDiffLayer::~FindDiffLayer(){};

bool FindDiffLayer::init(){
	/*
	auto director = Director::getInstance();
	director->getOpenGLView()->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);
*/	
	bool bRet = false;

	do{
		if(!Layer::init()){
			bRet = false;
			break;
			//return false;
		}

		//if (_gamestatus != GAME_START) {
		//	_gamestatus = GAME_START;
		//}
		
		std::string id;
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		CC_BREAK_IF(!app);
		
		_zoomin = true;
		_gametime = GAME_TIME_EASY;
		_UINode = nullptr;
		_bTouchable = true;
		_nTlimit = 3; // total tools can be used in each game
		_nTlcount = app->getToolsNumberInfo(); // total tools user has
		_nLvl = app->GetUserLevel(); //totol tips can be used
		_fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
		_lTtlScore = app->GetUserTotalScore();		
		_bTlEnabled = true;

		//load game level info
		CC_BREAK_IF(!loadLevelInfo());
		//load UI from csb file
		CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(SPOT_UI_CSB));
		_UINode = CSLoader::createNode(SPOT_UI_CSB);
		this->addChild(_UINode);

		auto lblTimer = dynamic_cast<Text*>(_UINode->getChildByName(LABEL_TIMER));
		auto _spError = dynamic_cast<Sprite*>(_UINode->getChildByName(SPOT_ERROR_ICON));
		auto btnMagnifier = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_MAGNIFIER));// 
		auto btnHome = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_HOME));// 
		auto btnTips= dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_TIPS));// 

		CC_BREAK_IF(lblTimer == nullptr);
		CC_BREAK_IF(btnHome == nullptr);
		CC_BREAK_IF(btnTips == nullptr);
		CC_BREAK_IF(_spError == nullptr);
		CC_BREAK_IF(btnMagnifier == nullptr);		

		auto lvlbar = dynamic_cast<Sprite*> (_UINode->getChildByName(SPOT_UI_LEVELBAR));
		CC_BREAK_IF(lvlbar == nullptr);
		lvlbar->setAnchorPoint(Vec2(0,0.5));
		//lvlbar->setPositionX(lvlbar->getPositionX()-lvlbar->getContentSize().width/2);
		lvlbar->setScaleX(_fPerToNLvl);

		auto btnTools = dynamic_cast<Button*> (_UINode->getChildByName(SPOT_UI_BTN_TIPS));
		CC_BREAK_IF(btnTools == nullptr);
		btnTools->addTouchEventListener(CC_CALLBACK_2(FindDiffLayer::ShowHintCallback, this));

		this->schedule(schedule_selector(FindDiffLayer::updateLabels), 0.01f);
		_spError->setVisible(false);
		_spError->setGlobalZOrder(99); //diplay error over the image

		btnHome->addTouchEventListener(CC_CALLBACK_2(FindDiffLayer::btnClick, this));
		btnTips->addTouchEventListener(CC_CALLBACK_2(FindDiffLayer::btnClick, this));
		btnMagnifier->addTouchEventListener(CC_CALLBACK_2(FindDiffLayer::onTouchZoom, this,FINDDIFF_ZOOMIN));

		//Add by Long for theme mode
		if (app->iGameMode == MobileGameMode::Theme_Mode){
			//btnMain->setVisible(false);
			//btnSetting->setVisible(false);
			//btnVoice->setVisible(false);
			//btnQuit->setVisible(false);
			//btnHome->setVisible(false);
			//btnTips->setVisible(false);
		}
		//Add by Long EOF

		//Add touch event
		_df_found = 0;
		_df_total = 5;
		_isMoving = false;
		_isResizing = false;
		_startPt = CCPoint::ZERO;
		_endPt = CCPoint::ZERO;
		this->schedule(schedule_selector(FindDiffLayer::updateDragMove), 0.01f);

		CC_BREAK_IF(!loadimages());

		//Add listener to layer
		//auto touchListener = EventListenerTouchOneByOne::create();
		//touchListener->onTouchBegan = CC_CALLBACK_2(FindDiffLayer::onTouchBegan, this);
		//touchListener->onTouchEnded = CC_CALLBACK_2(FindDiffLayer::onTouchEnded, this);
		//touchListener->onTouchMoved = CC_CALLBACK_2(FindDiffLayer::onTouchMoved, this);
		//touchListener->onTouchCancelled = CC_CALLBACK_2(FindDiffLayer::onTouchCancelled, this);
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		auto listener = EventListenerTouchAllAtOnce::create();
		CC_BREAK_IF(listener == nullptr);
		listener->onTouchesBegan = CC_CALLBACK_2(FindDiffLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(FindDiffLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(FindDiffLayer::onTouchesEnded, this);
		listener->onTouchesCancelled = CC_CALLBACK_2(FindDiffLayer::onTouchesCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,	this);
		
		//Add sound Effect
		auto audio = SimpleAudioEngine::getInstance();
		CC_BREAK_IF(audio == nullptr);
		audio->preloadEffect(SOUND_ERROR);

		if ( app->getBGMstatus() == BGMusic_flag::ON){
			//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.1f);
			app->StartBGMusic(BGM_GAME);

		}

		//UserDefault *load = UserDefault::getInstance();
		//string sFirstFlip = load->getStringForKey("FIRSTTIME_SPOT","0");
		//{
		//	if (atoi(sFirstFlip.c_str()) == 0)
		//	{
		//		this->scheduleOnce(schedule_selector(FindDiffLayer::loadFirstTutorial), 1.1f);
		//		load->setStringForKey("FIRSTTIME_SPOT", "1");
		//	}
		//}

		//-----------------------------------add animation----------------------------------------------------------

		auto mastco = dynamic_cast<ImageView *> (_UINode->getChildByName(MASCOTT));
		if (mastco)
		{
			mastco->setVisible(false);
		}

		ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMATION_FILE);
		Armature *armature = Armature::create(ARMATURE);
		CC_BREAK_IF(armature == nullptr);
		armature->setAnchorPoint(Vec2(0.5, 0.5));
		armature->setVisible(true); 
		armature->setPosition(Vec2(155, 450));
		armature->setTag(888);
		auto imgBackFrame = dynamic_cast<ImageView*> (_UINode->getChildByName(BACK_FRAME));
		CC_BREAK_IF(imgBackFrame == nullptr);
		imgBackFrame->setZOrder(1);
		schedule(schedule_selector(FindDiffLayer::PlayAnimation), 5.0f);
		_UINode->addChild(armature);


		//-------------------------------------------------------------------------------------
		bRet = true;

	}while(0);

	return bRet;
}


void FindDiffLayer::PlayAnimation(float dt)
{
	Armature *armature = dynamic_cast<Armature *>(_UINode->getChildByTag(888));
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	armature->getAnimation()->setSpeedScale(0.8);
	AppDelegate* app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_ANIMATION);
	}
}
void FindDiffLayer::loadFirstTutorial(float dt)
{
	//unschedule(schedule_selector(FindDiffLayer::loadFirstTutorial));
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		Director::getInstance()->pushScene(scene);
	}
}
bool FindDiffLayer::loadimages(){

	bool bRet = false;

	do{
		//get the origin point of the X-Y axis, and the visiable size of the screen
		Size visiblesize = Director::getInstance()->getVisibleSize();
		//Point origin = Director::getInstance()->getVisibleOrigin();

		//Load 2 images
		//Sprite *photo_original  = Sprite::create("find_diff_sample.jpg",Rect(0,0,300,300));
		//Sprite *photo_copy  = Sprite::create("find_diff_sample.jpg",Rect(300,0,300,300));

		//_imgSize = Size(300,300);
		//Sprite *photo_original  = Sprite::create("games_lance/find_diff/tweebot.jpg");
		//Sprite *photo_copy  = Sprite::create("games_lance/find_diff/tweebot_wrong.jpg");
		if (!FileUtils::getInstance()->isFileExist(_levelinfofd.image1)){
			char msg[256];
			std::string::size_type pos = _levelinfofd.image1.find_last_of('/'); // position of "live" in str
			std::string str = _levelinfofd.image1.substr (pos+1); 			
			//std::string strFilename = _levelinfofd.image1.find_last_of()
			sprintf(msg,"file %s doesn't exist", str.c_str());
			MessageBox(msg,"Tweebaa");
			break;
		}
		//else{
		//	char msg[256];
		//	std::string::size_type pos = _levelinfofd.image1.find_last_of('/'); // position of "live" in str
		//	std::string str = _levelinfofd.image1.substr (pos+1); 			
		//	//std::string strFilename = _levelinfofd.image1.find_last_of()
		//	sprintf(msg,"file %s exists", str.c_str());
		//	MessageBox(msg,"Tweebaa");	
		//}

		if (!FileUtils::getInstance()->isFileExist(_levelinfofd.image2)){
			char msg[256];
			std::string::size_type pos = _levelinfofd.image2.find_last_of('/'); // position of "live" in str
			std::string str = _levelinfofd.image2.substr (pos+1); 			
			//std::string strFilename = _levelinfofd.image1.find_last_of()
			sprintf(msg,"file %s doesn't exist", str.c_str());
			MessageBox(msg,"Tweebaa");
			break;
		}
		//else{
		//	char msg[256];
		//	std::string::size_type pos = _levelinfofd.image2.find_last_of('/'); // position of "live" in str
		//	std::string str = _levelinfofd.image2.substr (pos+1); 			
		//	//std::string strFilename = _levelinfofd.image1.find_last_of()
		//	sprintf(msg,"file %s exists", str.c_str());
		//	MessageBox(msg,"Tweebaa");
		//}


		Sprite *photo_original  = Sprite::create(_levelinfofd.image1);
		Sprite *photo_copy  = Sprite::create(_levelinfofd.image2);



		CC_BREAK_IF(photo_original == nullptr);
		CC_BREAK_IF(photo_copy == nullptr);

		_imgSize = getImageSize(_levelinfofd.image1.c_str());

		auto img1 = dynamic_cast <Sprite *> (_UINode->getChildByName(BG_IMG1));
		auto img2 = dynamic_cast <Sprite *> (_UINode->getChildByName(BG_IMG2));
		CC_BREAK_IF(img1 == nullptr);
		CC_BREAK_IF(img2 == nullptr);

		img1->setVisible(false);
		img2->setVisible(false);
	
		_scale = img1->getBoundingBox().size.width / _imgSize.width;
		_zmLevel = 1;

		//Add 1st Image Frame
		//auto stencil = Sprite::create("games_lance/zoom_dummy_300x300.png");
		auto stencil = Sprite::create(_levelinfofd.image1);
		CC_BREAK_IF(stencil == nullptr);
		stencil->setScale(_scale);

		auto clipper = ClippingNode::create();
		CC_BREAK_IF(clipper == nullptr);

		clipper->setStencil(stencil);//
		clipper->setInverted(false);//set template can be seen
		clipper->setAlphaThreshold(0);//Set template Alpha value to 0
		clipper->setTag(FINDDIFF_CLIPPER_1);
		this->addChild(clipper);//4
		clipper->setPosition(img1->getPosition());

		photo_original->setTag(FINDDIFF_IMG_1);
		photo_original->setAnchorPoint(Vec2(0.5,0.5));
		photo_original->setScale(_scale);
		clipper->addChild(photo_original);

		//get image size
		_image_width = photo_original->getBoundingBox().size.width;
		_image_height = photo_original->getBoundingBox().size.height;

		//Add differences in 1st image
		//*******************************BEGIN*******************************

		std::vector<Vec2>::const_iterator v;
		for(v =_diffPositions.begin(); v!=_diffPositions.end(); ++v){
		  cocos2d::log("%f %f",(*v).x,(*v).y);
		}

		//char light[20] = "btnLight_X\0";
		string strlight = BUTTON_LIGHT;

		//v =_diffPositions.begin();

		for (int i = 0; i < _levelinfofd.diffNum; i++) {
			auto sp = Sprite::create(SPOT_DIFF_CIRCLE);
			CC_BREAK_IF(sp == nullptr);
			//sp->setScale(50 / sp->getContentSize().width);
			sp->setTag(FINDDIFF_IMG1_CIR1 + i);
			sp->setAnchorPoint(Vec2(0.5,0.5));
			sp->setVisible(false);
			photo_original->addChild(sp,99);
			sp->setPosition(_levelinfofd.diffList[i]);
			_vec_circle.pushBack(sp);
			strlight[9] = LETTER_NUM_1 + i;
			_arrlight[i] = dynamic_cast<Button*>(_UINode->getChildByName(strlight));
			CC_BREAK_IF(_arrlight[i] == nullptr);
			_arrlight[i]->setBright(true);
			_arrlight[i]->setZOrder(2);
		}

		//Add 1st Image Frame
		auto stencil2 = Sprite::create(_levelinfofd.image1);
		CC_BREAK_IF(stencil2 == nullptr);

		stencil2->setScale(_scale);
		auto clipper2 = ClippingNode::create();
		CC_BREAK_IF(clipper2 == nullptr);

		clipper2->setStencil(stencil2);//
		clipper2->setInverted(false);//set template can be seen
		clipper2->setAlphaThreshold(0);//Set template Alpha value to 0
		clipper2->setTag(FINDDIFF_CLIPPER_2);
		this->addChild(clipper2);//4
		clipper2->setPosition(img2->getPosition());

		//Load 2nd image
		photo_copy->setTag(FINDDIFF_IMG_2);
		photo_copy->setAnchorPoint(Vec2(0.5,0.5));
		photo_copy->setScale(_scale);
		clipper2->addChild(photo_copy);

		//Add differences in 1st image
		//v =_diffPositions.begin();
		for (int i = 0; i < _levelinfofd.diffNum; i++) {
			auto sp = Sprite::create(SPOT_DIFF_CIRCLE);
			CC_BREAK_IF(sp == nullptr);
			//sp->setScale(50 / sp->getContentSize().width);
			sp->setTag(FINDDIFF_IMG2_CIR1 + i);
			sp->setAnchorPoint(Vec2(0.5,0.5));
			sp->setVisible(false);
			photo_copy->addChild(sp,99);
			sp->setPosition(_levelinfofd.diffList[i]);
			_vec_circle2.pushBack(sp);
		}


		_rectFrame1 = img1->getBoundingBox();
		_rectFrame2 = img2->getBoundingBox();
		//_rectFrame1 = photo_original->getBoundingBox();
		//_rectFrame2 = photo_copy->getBoundingBox();
		_rectImg1 = photo_original->getBoundingBox();
		_rectImg2 = photo_copy->getBoundingBox();

		//log("_rectFrame1 origin.x = %f, origin.y = %f, width = %f, height = %f", _rectFrame1.origin.x,_rectFrame1 .origin.y,_rectFrame1 .size.width,_rectFrame1 .size.height );
		//log("_rectImg1 origin.x = %f, origin.y = %f, width = %f, height = %f", _rectImg1.origin.x,_rectImg1.origin.y,_rectImg1.size.width,_rectImg1.size.height );
		_new_x = img1->getPositionX();
		_new_y = img1->getPositionY();
	
		auto world_cord = clipper->convertToWorldSpace(photo_original->getPosition());
		auto world_cord_AR = clipper->convertToWorldSpaceAR(photo_original->getPosition());
		//auto world_cord_AR = clipper->convertToWindowSpace(photo_original->getPosition());

		_new_x2 = img2->getPositionX();
		_new_y2 = img2->getPositionY();

		bRet = true;
	} while(0);

	return bRet;
}

void FindDiffLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event){
	cocos2d::log("onTouchesBegan");

}

void FindDiffLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event){


	cocos2d::log("onTouchesMoved: start");
	int sel = 0;

	if (touches.size() > 1){
		//Two touches should be in the same frame
		if (_rectFrame1.containsPoint(touches[0]->getLocation()) && _rectFrame1.containsPoint(touches[1]->getLocation())){
			cocos2d::log("Touche in frame 1");
			sel = 1;
		} else if (_rectFrame2.containsPoint(touches[0]->getLocation()) && _rectFrame2.containsPoint(touches[1]->getLocation())){
			cocos2d::log("Touche in frame 2");
			sel = 2;
		}else {
			cocos2d::log("touches are not in any frame");
			return;
		}

		Sprite* img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1);
		Sprite* img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2);

		if (sel == 1) {

		}
		else if (sel == 2){
			cocos2d::log("Swap two images img1's tag : %d, img2's tag : %d", img1->getTag(), img2->getTag());
			Sprite* sp = img1;
			img1 = img2;
			img2 = sp;
			cocos2d::log("Swap two images img1's tag : %d, img2's tag : %d", img1->getTag(), img2->getTag());
		} else {
			return;
		}
	
		auto location1 = touches.at(0)->getLocation();
		auto location2 = touches.at(1)->getLocation();
		auto curDistance = location1.distance(location2);

		auto prev_location1 = touches.at(0)->getPreviousLocation();
		auto prev_location2 = touches.at(1)->getPreviousLocation();
		auto prevDistance = prev_location1.distance(prev_location2);

        float deltax = (prev_location1.x + prev_location2.x)/2 - img1->getPositionX();
        float deltay = (prev_location1.y + prev_location2.y)/2 - img1->getPositionY();  
		_zmLevel = img1->getScale()*(curDistance/prevDistance);
		_zmLevel = MIN(2*_scale,MAX(1*_scale, _zmLevel));
		img1->setScale(_zmLevel);
		img2->setScale(_zmLevel);

		float x = (location2.x+location1.x)/2 - deltax;
		float y = (location2.y+location1.y)/2 - deltay;  
		_new_x = x;
		_new_y = y;

		if (_new_x < (-(img1->getBoundingBox().size.width - _rectFrame1.size.width) / 2)) _new_x = -(img1->getBoundingBox().size.width - _rectFrame1.size.width) / 2;
		if (_new_x > (img1->getBoundingBox().size.width - _rectFrame1.size.width) / 2) _new_x = (img1->getBoundingBox().size.width - _rectFrame1.size.width) / 2;
		if (_new_y < (-(img1->getBoundingBox().size.height - _rectFrame1.size.height) /2 )) _new_y = -(img1->getBoundingBox().size.height - _rectFrame1.size.height) /2;
		if (_new_y > (img1->getBoundingBox().size.height - _rectFrame1.size.height) / 2) _new_y = (img1->getBoundingBox().size.height - _rectFrame1.size.height) / 2;
		
		float x_1, y_1;
		x_1 = img1->getPositionX();
		y_1 = img1->getPositionY();
		img1->setPosition(Point(_new_x,_new_y));                        
       
		//deltax = (location1.x+ location2.x)/2 - img1->getPositionX();       //计算新的偏移量  
		//deltay = (location2.y + location1.y)/2 - img1->getPositionY();  
		float diff_x = img1->getPositionX() - x_1;
		float diff_y = img1->getPositionY() - y_1;
		
		//following the 1st image move
		float x_2, y_2;
		img2->setPositionX(img2->getPositionX() + diff_x);
		img2->setPositionY(img2->getPositionY() + diff_y);

		return;

	} else if (touches.size() == 1)	{
			//cocos2d::log("touch's size is 1, move");
			_isMoving = true;

			//Two touches should be in the same frame
			if (_rectFrame1.containsPoint(touches[0]->getLocation())){
				cocos2d::log("Touche in frame 1");
				sel = 1;
			} else if (_rectFrame2.containsPoint(touches[0]->getLocation())){
				cocos2d::log("Touche in frame 2");
				sel = 2;
			}else {
				cocos2d::log("touches are not in any frame");
				return;
			}

			Sprite* img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1);
			Sprite* img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2);

			if (sel == 1) {

			}
			else if (sel == 2){
				cocos2d::log("Swap two images img1's tag : %d, img2's tag : %d", img1->getTag(), img2->getTag());
				Sprite* sp = img1;
				img1 = img2;
				img2 = sp;
				cocos2d::log("Swap two images img1's tag : %d, img2's tag : %d", img1->getTag(), img2->getTag());
			} else {
				return;
			}

			//Sprite * container1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1);
			//Sprite * container2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2);
	
			Rect img1Rect = img1->getBoundingBox();
			Rect img2Rect = img2->getBoundingBox();

			//_rectImg1 = img1->getBoundingBox();
			//_rectImg2 = img2->getBoundingBox();

			//Point newptImg1,newptImg2;
			float deltaX = touches[0]->getLocation().x - touches[0]->getPreviousLocation().x;
			float deltaY = touches[0]->getLocation().y - touches[0]->getPreviousLocation().y;
			float x1,y1,x2,y2;

			_new_x = img1->getPositionX() + deltaX;
			_new_y = img1->getPositionY() + deltaY;

			x1 = img1->getPositionX() + deltaX;
			y1 = img1->getPositionY() + deltaY;

			//cocos2d::log("move to position x:%f, y:%f",_new_x, _new_y);
			if (x1 < (-(img1Rect.size.width - _rectFrame1.size.width) / 2)) x1 = -(img1Rect.size.width - _rectFrame1.size.width) / 2;
			if (x1 > (img1Rect.size.width - _rectFrame1.size.width) / 2) x1 = (img1Rect.size.width - _rectFrame1.size.width) / 2;
			if (y1 < (-(img1Rect.size.height - _rectFrame1.size.height) /2 )) y1 = -(img1Rect.size.height - _rectFrame1.size.height) /2;
			if (y1 > (img1Rect.size.height - _rectFrame1.size.height) / 2) y1 = (img1Rect.size.height - _rectFrame1.size.height) / 2;

			deltaX = x1 - img1->getPositionX(); //get distance in x
			deltaY = y1 - img1->getPositionY(); //get distance in y
			
			x2 = img2->getPositionX() + deltaX;
			y2 = img2->getPositionY() + deltaY;
			cocos2d::log("move to position x:%f, y:%f",x1,y1);

			img1->setPosition(x1,y1);
			img2->setPosition(x2,y2);
		}
 
	return;
}

void FindDiffLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event){
	cocos2d::log("onTouchesEnded");
	if (touches.size() == 1) {
		onTouchEnded(touches.at(0),unused_event);
	}
	_isResizing = false;
}

void FindDiffLayer::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event){
	cocos2d::log("onTouchesCancelled");
	if (touches.size() == 1) {
		onTouchCancelled(touches.at(0),unused_event);
	}
	_isResizing = false;
}

void FindDiffLayer::menuStartCallback(Object *sender){
}

void FindDiffLayer::menuKeyCallback(Object *sender){

}

void FindDiffLayer::btnClick(Ref* pSender, ui::Widget::TouchEventType eEventType){


	//if (eEventType != ui::Widget::TouchEventType::BEGAN)
	Sprite* sp = (Sprite*) pSender;
	int tag = sp->getTag();
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}

		if (sp->getName().compare(BUTTON_HOME) == 0){
			FindDiffLayer::QuitGame(); //quit the game
		}else{
			//MessageBox("Another botton", "Tweebaa App");
			showTutorial();
		}	
	}
	return;

}

void FindDiffLayer::resize(float fScale){

	return;
}
void FindDiffLayer::btnZoomInCallback(Object *sender){

	if (_zoomin == true) {
		if (_zmLevel < 2) {
			_zmLevel = _zmLevel + 0.5; 	
			//ClippingNode* clipper = (ClippingNode*) this->getChildByTag(FINDDIFF_CLIPPER_1);
	
			Sprite* img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1);
			img1->setScale(_zmLevel * _scale);
			//cocos2d::log("after: img 1 size.x is %d and size.y is %d",_image_width, _image_height);

			Sprite* img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2);
			img2->setScale(_zmLevel * _scale);

			_image_width = img1->getBoundingBox().size.width;
			_image_height = img1->getBoundingBox().size.height;
			
			if (_zmLevel == 2) {
				_zoomin = false;
			}
		}
	}
	else {
		if (_zmLevel > 1) {
			_zmLevel = _zmLevel - 0.5; 	
			//ClippingNode* clipper = (ClippingNode*) this->getChildByTag(FINDDIFF_CLIPPER_1);
	
			Sprite* img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1);
			img1->setScale(_zmLevel * _scale);
			cocos2d::log("after: img 1 size.x is %d and size.y is %d",_image_width, _image_height);

			Sprite* img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2);
			img2->setScale(_zmLevel * _scale);

			_image_width = img1->getBoundingBox().size.width;
			_image_height = img1->getBoundingBox().size.height;

			auto bound = img1->getBoundingBox();
			float left = bound.origin.x; 
			float right = bound.origin.x + bound.size.width;
			float bottom = bound.origin.y;
			float top = bound.origin.y + bound.size.height;

			Vec2 pt = img1->convertToNodeSpace(this->getChildByTag(FINDDIFF_CLIPPER_1)->getPosition());

			//if (img1->getBoundingBox().size.width  <= _frame1_x) img1->setPosition(0,0);
			//if (img2->getBoundingBox().size.width <= _frame2_x) img2->setPosition(0,0);
			img1->setPosition(0,0);
			img2->setPosition(0,0);

			if (_zmLevel == 1) {
				_zoomin = true;
			}
		}
	}
	return;
	}

void FindDiffLayer::btnZoomOutCallback(Object *sender){
	
	return;
	}

bool FindDiffLayer::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch Began");
	_isMoving = false;
	return true;
}

void FindDiffLayer::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");
	if (!_bTouchable) {
		return;
	}

	auto location1 = touch->getLocation();
	auto location2 = touch->getStartLocation();
	auto distance = location1.distance(location2);

	if (distance > 5) {
		return;
	}

	do{
		//if (_gamestatus != GAME_START) {
		//	break;
		//}

		//_endPt = touch->getLocation();

		_isMoving = false;
		_isResizing = false;
		AppDelegate *app = (AppDelegate*)Application::getInstance();

		int prevfound = _df_found;
		Vec2 posError;

		//auto bound_img1 = this->getChildByTag(FINDDIFF_IMG_1)->getBoundingBox();
		Sprite * img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1); //get 1st Image sprite
		Sprite * img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2); //get 1st Image sprite
		CC_BREAK_IF(img1 == nullptr);
		CC_BREAK_IF(img2 == nullptr);

		auto bound_img1 = this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1)->getBoundingBox(); // get 1st image's boundingbox
		auto bound_img2 = this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2)->getBoundingBox(); //get 2nd image's boundingbox
		Vec2 touchinImg1 = this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1)->convertTouchToNodeSpace(touch);
		Vec2 touchinImg2 = this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2)->convertTouchToNodeSpace(touch);

		auto hide_img1 = dynamic_cast <Sprite *> (_UINode->getChildByName(BG_IMG1));
		auto hide_img2 = dynamic_cast <Sprite *> (_UINode->getChildByName(BG_IMG2));
		CC_BREAK_IF(hide_img1 == nullptr);
		CC_BREAK_IF(hide_img2 == nullptr);

		//auto touchinClipper1 = hide_img1->convertTouchToNodeSpace(touch);
		auto touchinClipper1 = touch->getLocation();	
		auto bound1 = hide_img1->getBoundingBox();

		//auto touchinClipper2 = hide_img2->convertTouchToNodeSpace(touch);
		auto touchinClipper2 = touch->getLocation();
		auto bound2 = hide_img2->getBoundingBox();

		//auto touchtip = touch->getLocation();
		//auto spTips = dynamic_cast<Sprite*>(_UINode->getChildByName(SPOT_UI_BTN_TIPS));
		//auto bound3 = spTips->getBoundingBox();


	//	if (bound_img1.containsPoint(touch->getLocation())) { // if touch point within 1st image's boundingbox
		if (bound1.containsPoint(touchinClipper1)) { // if touch point within 1st image's boundingbox		
			for(auto sp : _vec_circle) { //Check touch position matches differences
				//log("sprite tag = %d", sp->getTag());
			
				if (sp->isVisible() == false) {
					auto bounds = sp->getBoundingBox();	//get difference boundingbox
					//cocos2d::log("bounds's origin.x:%f; origin.y:%f; width:%f; height:%f",bounds.origin.x,bounds.origin.y,bounds.size.width,bounds.size.height);
					if (bounds.containsPoint(touchinImg1)){		
					//if (bounds.containsPoint( this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1)->convertTouchToNodeSpace(touch))){	//if touch point on touch difference
						sp->setVisible(true);	//set touched difference visiable
						auto sp2 = img2->getChildByTag(sp->getTag()+5);	//get touched difference sprite in 2nd image
						sp2->setVisible(true);	//set touched difference visiable in 2nd image
						_df_found++;  //update found differences count
						break;
					}
				}
			}
			posError = touchinClipper1;
		} 
		else if (bound2.containsPoint(touchinClipper2)) { // if touch point within 1st image's boundingbox		
			for(auto sp : _vec_circle2) { //Check touch position matches differences
				//log("sprite tag = %d", sp->getTag());
				if (sp->isVisible() == false) {
					auto bounds = sp->getBoundingBox();	//get difference boundingbox
					//cocos2d::log("bounds's origin.x:%f; origin.y:%f; width:%f; height:%f",bounds.origin.x,bounds.origin.y,bounds.size.width,bounds.size.height);
					if (bounds.containsPoint(touchinImg2)){		
					//if (bounds.containsPoint( this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1)->convertTouchToNodeSpace(touch))){	//if touch point on touch difference
						sp->setVisible(true);	//set touched difference visiable
						auto sp2 = img1->getChildByTag(sp->getTag()-5);	//get touched difference sprite in 2nd image
						sp2->setVisible(true);	//set touched difference visiable in 2nd image
						_df_found++;  //update found differences count
						break;
					}
				}
			}
			posError = touchinClipper2;
		} 
		else {
			//touch outsite of image1 and image2
			//if (bound3.containsPoint(touchtip)){ // Touch in tools
			//	FindDiffLayer::ShowHint();
			//	log("this is tips");
			//}
			return;
		}

		for (int j = 0; j < _df_found; j++) {
			if (j < 5){ 
			_arrlight[j]->setBright(false);
			}
		}

		if (prevfound == _df_found) {
			_bTouchable = false;
			ShowWrongPos(posError);
			//show cross

			
			//Timer Label effect
			//auto timerLabel = dynamic_cast<Text*> (_UINode->getChildByName(LABEL_TIMER));
			//CC_BREAK_IF(timerLabel == nullptr);
			//auto scaleup = ScaleBy::create(0.01f,1.2f,1.2f);
			//auto blink2 = Blink::create(2,3);
			//auto tintto = TintTo::create(0.01f, 255.0f, 0.0f, 0.0f);
			//auto scaledown = ScaleBy::create(0.1f,0.83f,0.83f);
			//auto tintback = TintTo::create(0.01f, 255.0f, 255.0f, 255.0f);
			//auto seq2 = Sequence::create(scaleup,tintto,blink2,scaledown,tintback, nullptr);

			//app->PlayEffect(SOUND_ERROR);
			//timerLabel->runAction(seq2);
		}

	} while(0);

	return;

}

void FindDiffLayer::ShowWrongPos(Vec2 ErrPos){

	auto sperror = dynamic_cast <Sprite *> (_UINode->getChildByName(SPOT_ERROR_ICON));
	if (sperror == nullptr) return;

	auto timerLabel = dynamic_cast<Text*> (_UINode->getChildByName(LABEL_TIMER));
	if(timerLabel == nullptr) return;

	//sperror->setVisible(true);
	sperror->setPosition(ErrPos);

	//_errtimer = ERROR_TIME;
	auto show = Show::create();
	auto action = Blink::create(2,3);
	auto hide = Hide::create();
	auto seq = Sequence::create(show, action, hide,nullptr);
	sperror->runAction(seq);

	//TimerLabel effect
	_gametime = (_gametime - 600) > 0 ? _gametime - 600 : 0; 
	auto blink2 = Blink::create(1.5f,3);
	auto tintto = TintTo::create(0.01f, 255.0f, 0.0f, 0.0f);
	auto tintback = TintTo::create(0.01f, 255.0f, 255.0f, 255.0f);
	auto seq2 = Sequence::create(tintto,blink2,tintback, nullptr);
	timerLabel->runAction(seq2);

	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(FindDiffLayer::resetTouchEvent, this));
	this->runAction(Sequence::create(DelayTime::create(2.0f), runCallback, nullptr));

}


void FindDiffLayer::onTouchMoved(Touch* touch, Event* event)
{
   cocos2d::log("touch moved");
	do{
		_isMoving = true;

		Sprite * container1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1);
		Sprite * img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1);
		Sprite * container2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2);
		Sprite * img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2);
		CC_BREAK_IF(container1 == nullptr);	
		CC_BREAK_IF(img1 == nullptr);
		CC_BREAK_IF(container2 == nullptr);
		CC_BREAK_IF(img2 == nullptr);

		Rect img1Rect = img1->getBoundingBox();
		Rect img2Rect = img2->getBoundingBox();

		_rectImg1 = img1->getBoundingBox();
		_rectImg2 = img2->getBoundingBox();
	

		Point newptImg1,newptImg2;
		float x = touch->getLocation().x - touch->getPreviousLocation().x;
		float y = touch->getLocation().y - touch->getPreviousLocation().y;

		if (_rectFrame1.containsPoint(touch->getLocation())) {
			_new_x = img1->getPositionX() + x;
			_new_y = img1->getPositionY() + y;
			//cocos2d::log("move to position x:%f, y:%f",_new_x, _new_y);

			if (_new_x < (-(img1Rect.size.width - _rectFrame1.size.width) / 2)) _new_x = -(img1Rect.size.width - _rectFrame1.size.width) / 2;
			if (_new_x > (img1Rect.size.width - _rectFrame1.size.width) / 2) _new_x = (img1Rect.size.width - _rectFrame1.size.width) / 2;
			if (_new_y < (-(img1Rect.size.height - _rectFrame1.size.height) /2 )) _new_y = -(img1Rect.size.height - _rectFrame1.size.height) /2;
			if (_new_y > (img1Rect.size.height - _rectFrame1.size.height) / 2) _new_y = (img1Rect.size.height - _rectFrame1.size.height) / 2;

			x = _new_x - img1->getPositionX(); //get distance in x
			y = _new_y - img1->getPositionY(); //get distance in y
			_new_x2 = img2->getPositionX() + x;
			_new_y2 = img2->getPositionY() + y;
			cocos2d::log("move to position x:%f, y:%f",_new_y, _new_y);

		} else if (_rectFrame2.containsPoint(touch->getLocation())) {
			_new_x2 = img2->getPositionX() + x;
			_new_y2 = img2->getPositionY() + y;
			//cocos2d::log("move to position x:%f, y:%f",_new_x, _new_y);

			if (_new_x2 < (-(img1Rect.size.width - _rectFrame1.size.width) / 2)) _new_x2 = -(img1Rect.size.width - _rectFrame1.size.width) / 2;
			if (_new_x2 > (img1Rect.size.width - _rectFrame1.size.width) / 2) _new_x2 = (img1Rect.size.width - _rectFrame1.size.width) / 2;
			if (_new_y2 < (-(img1Rect.size.height - _rectFrame1.size.height) /2 )) _new_y2 = -(img1Rect.size.height - _rectFrame1.size.height) /2;
			if (_new_y2 > (img1Rect.size.height - _rectFrame1.size.height) / 2) _new_y2 = (img1Rect.size.height - _rectFrame1.size.height) / 2;

			x = _new_x2 - img2->getPositionX();
			y = _new_y2 - img2->getPositionY();
			_new_x = img1->getPositionX() + x;
			_new_y = img1->getPositionY() + y;

		} 

		//Sprite * img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1);
		img1->setPosition(_new_x,_new_y);
		//Sprite * img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2);
		img2->setPosition(_new_x2,_new_y2);
		
	}while(0);

	return;

}

void FindDiffLayer::onTouchCancelled(Touch* touch, Event* event)
{
   cocos2d::log("touch cancelled");
	onTouchEnded(touch, event);
}

void FindDiffLayer::updateLabels(float dt) {
	do{
		_gametime -= 1;
		int ttlseconds = _gametime / 60;
		int seconds = ttlseconds % 60;
		int min = ttlseconds / 60;
		char lblTimer[100];
		sprintf(lblTimer,"%d:%02d", min, seconds);
		string string1 = lblTimer;
		auto timerLabel = dynamic_cast<Text*> (_UINode->getChildByName(LABEL_TIMER));
		CC_BREAK_IF(timerLabel == nullptr);
		timerLabel->setString(string1);

		if (_gametime < 10 * 60) {
				timerLabel->setColor(ccc3(255,0,0));
		}

		char lblTips[10];
		sprintf(lblTips,"%d", _nTlcount);
		string string2 = lblTips;
		auto tipsLabel = dynamic_cast<Text*> (_UINode->getChildByName("txtTips"));
		CC_BREAK_IF(tipsLabel == nullptr);
		tipsLabel->setString(string2);

		char lblLvl[10];
		sprintf(lblLvl,"%d", _nLvl);
		string string3 = lblLvl;
		auto lvlLabel = dynamic_cast<Text*> (_UINode->getChildByName("txtLvl"));
		CC_BREAK_IF(lvlLabel == nullptr);
		lvlLabel->setString(string3);


		char lblScore[20];
		sprintf(lblScore,"%lu", _lTtlScore);
		string string4 = lblScore;
		auto scoreLabel = dynamic_cast<Text*> (_UINode->getChildByName("txtScore"));
		CC_BREAK_IF(scoreLabel == nullptr);
		scoreLabel->setTextHorizontalAlignment(TextHAlignment::RIGHT);
		scoreLabel->setString(string4);

		updateprogress();
	}while(0);

	return;
}

void FindDiffLayer::updateprogress(){
	if (_gametime > 0) {
		if (_df_found == _df_total) {
			_df_found = 0;
			gamewin();
			AppDelegate *app = (AppDelegate*)Application::getInstance();
			app->LogAccess("Find Difference: Game Win");
		}
	}
	else {
		gamelose();
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->LogAccess("Find Difference: Game Lose");

	}


}

void FindDiffLayer::updateDragMove(float dt) 
{
	// check if the screen is being touched
	if (_isMoving == true)
	{
	}

	if (_isResizing == true){
		resize(_zmLevel);
	}
}

void FindDiffLayer::onTouchZoom(Object* pSender, ui::Widget::TouchEventType eEventType, int nSender)
{
	Sprite* sp = (Sprite*) pSender;
	if (eEventType != ui::Widget::TouchEventType::BEGAN)
	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			test();
			break;
		case ui::Widget::TouchEventType::MOVED:
			cocos2d::log("touch Cancel");
			break;
		case ui::Widget::TouchEventType::CANCELED:
			cocos2d::log("touch Cancel");
			break;
		default:
			cocos2d::log("touch Default");
			break;
	}
	return;

}



void FindDiffLayer::QuitGame(){

	//_gamestatus = GAME_OVER;

	//start to call a Msgbox
	_bTouchable = false;
	MsgBoxType m_type = TP_Q_YESNO;
	std::string title = MSG_QUIT_TITLE;
	std::string msg = MSG_QUIT_MSG;

	MsgBox::setMsgBox(m_type,&title,&msg);
	auto msgbox = MsgBox::create();
	msgbox->setTag(9000);
	this->addChild(msgbox,999);
	
	//Add Event to process
	auto btnYes = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
	auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
	btnYes->addTouchEventListener(CC_CALLBACK_2(FindDiffLayer::yesCallback, this));
	btnNo->addTouchEventListener(CC_CALLBACK_2(FindDiffLayer::noCallback, this));
	
}
void FindDiffLayer::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);

		}
		_vec_circle.clear();
		_vec_circle2.clear();
		this->unschedule(schedule_selector(FindDiffLayer::updateLabels));
		this->unschedule(schedule_selector(FindDiffLayer::updateDragMove));

		//AppDelegate *app = (AppDelegate*)Application::getInstance();
		//app->SaveGamePassLevel();
		app->moveToNextGameLevel();
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

void FindDiffLayer::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {
			
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);

		}		
		//Don't quit the game
		this->removeChildByTag(9000);
		//enable the touch event
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(FindDiffLayer::resetTouchEvent, this));
		this->runAction(Sequence::create(DelayTime::create(1.0f), runCallback, nullptr));
        CCLOG("touch ended.");
    }
}

void FindDiffLayer::resetTouchEvent(){
	_bTouchable = true;
}

void FindDiffLayer::gamestart(){
	//_gamestatus = GAME_START;
}

void FindDiffLayer::gamepause(){

}

void FindDiffLayer::gamewin(){

	//_gamestatus = GAME_OVER;
	_vec_circle.clear();
	_vec_circle2.clear();
	
	this->unschedule(schedule_selector(FindDiffLayer::updateLabels));
	this->unschedule(schedule_selector(FindDiffLayer::updateDragMove));


	int score = 0;
	int ttlseconds = _gametime / 60;


	//update score
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if(app->iGameMode == MobileGameMode::Theme_Mode){
		app->GameRunRoundNext();
	}else{

		//if(app->getGameLevelPassedInfo() == 0){
		//	if (ttlseconds > 60 && ttlseconds <= 90){
		//		score = WIN_POINT_HIG;
		//	}else if(ttlseconds > 30 && ttlseconds <= 60){
		//		score = WIN_POINT_MED;	
		//	}else{
		//		score = WIN_POINT_LOW;
		//	}
		//}else{
		//	score = WIN_POINT_LOW;		
		//}

		//app->UpdateUserScore(1,score);
		//app->setReward(score);
		app->updateGameWinScore(ttlseconds);
		app->SaveGamePassLevel();//Add by Long to Save Level
		//app->saveGameLevelPassedInfo();
		app->moveToNextGameLevel();


		this->scheduleOnce(schedule_selector(FindDiffLayer::delayWinQuit),1.0f);
		//auto scene = GameWin::createScene();
		//if(scene != nullptr){
		//	TransitionScene *transition = TransitionFade::create(1, scene);
		//	Director::getInstance()->replaceScene(transition);
		//}
	}
}

void FindDiffLayer::gamelose(){
	_vec_circle.clear();
	_vec_circle2.clear();
	//_gamestatus = GAME_OVER;

	this->unschedule(schedule_selector(FindDiffLayer::updateLabels));
	this->unschedule(schedule_selector(FindDiffLayer::updateDragMove));

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	//app->SaveGamePassLevel();//Add by Long to Save Level
	app->moveToNextGameLevel();

	this->scheduleOnce(schedule_selector(FindDiffLayer::delayFailQuit),1.0f);
	//auto scene = GameFailed::createScene();
	//if(scene != nullptr){
	//	TransitionScene *transition = TransitionFade::create(1, scene);
	//	Director::getInstance()->replaceScene(transition);
	//}
}

bool FindDiffLayer::loadLevelInfo(){
	bool bRet = false;

	
	std::string str = SPOT_LEVEL_INFO;
	char lvl[10];
	int nCurrentLevel = 0;

	AppDelegate *app = (AppDelegate*) Application::getInstance();
	//int isPassed = app->getGameLevelPassedInfo();
	//log("%d", isPassed);
	
	//get current level

	nCurrentLevel = app->getCurrentGameLevel();
	//nCurrentLevel = 10;
	UserDefault *save = UserDefault::getInstance();
	//g_passLevelCount = save->getIntegerForKey(PlayerPassLevelCountKey[m_iCurrentGameID], 0);
	//m_iCurrentGameLevel = g_passLevelCount + 1;

	//if (m_iCurrentGameLevel > SPOT_MAX_LEVEL) { 		//For testing purpose, set 3 levels only
	//	m_iCurrentGameLevel = 1;
	//}

	snprintf(lvl, 10, "%d", nCurrentLevel);
	bRet = readFile(str.c_str(),lvl);

	return bRet;


}
void FindDiffLayer::saveLevelInfo(){

}

Size FindDiffLayer::getImageSize(const char* filename) {
	Size ret;
	do {
		CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(filename));
		auto sprite = Sprite::create(filename);
		return ret = Size(sprite->boundingBox().size.width, sprite->boundingBox().size.height);
	}while(0);
	ret = Size::ZERO;
	return ret;
}

void FindDiffLayer::showTutorial(){
	auto scene = TutorialScene::create();
	TutorialScene::_nIsIngame = 1;
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}
	return;
}
void FindDiffLayer::test() {

	auto scene = TutorialScene::create();
	if(scene != nullptr){
		log("***********SPOT GAME*********** scene != nullptr");
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
	}

	
	//Size visiblesize = Director::getInstance()->getVisibleSize();

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SPOT_TUTORIAL_PLIST);
	//auto testBatchNode=SpriteBatchNode::create(SPOT_TUTORIAL_IMAGE);

	//testBatchNode->setPosition(visiblesize.width/2, visiblesize.height/2);
	//this->addChild(testBatchNode);

	//auto testSprite=Sprite::createWithSpriteFrameName("ZOOM_step1.jpg");
	//testSprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ZOOM_step3.jpg"));
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


	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_atlas.plist");
	//mySprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("frame_name"))
	
}

void FindDiffLayer::test2(){
		log("********FindDiffLayer::test2()********");
	auto scene = GameWin::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}
void FindDiffLayer::test4(float d){

}

bool FindDiffLayer::readFile(const char* fileName,const char* levelID) {

	bool bRet = false;
	do{
		string id = levelID;

		//Add by Long to check the file existings
		if(FileUtils::getInstance()->isFileExist(fileName)){
			ValueMap root = FileUtils::getInstance()->getValueMapFromFile(fileName);
			CC_BREAK_IF(root.size() < 1);
			ValueMap levels = root["SPOT"].asValueMap();
			CC_BREAK_IF(levels.size() < 1);

			//	CC_BREAK_IF(levels.size() != SPOT_MAX_LEVEL);
			//log("%s", FileUtils::getInstance()->fullPathForFilename(fileName).c_str());
			if(levels.size()>=1){ //Add by Long to verify
				ValueMap levelinfo = levels[levelID].asValueMap();
				//_levelinfofd.sku = levelinfo["SKU"].asString();
				_levelinfofd.name = levelinfo["NAME"].asString();
				_levelinfofd.image1 = levelinfo["IMAGE1"].asString();
				_levelinfofd.image2 = levelinfo["IMAGE2"].asString();
				_levelinfofd.diffNum = levelinfo["DIFFNUM"].asInt();

				ValueVector vec = levelinfo["DIFFS"].asValueVector();
				ValueMap vm;

				for (int i = 0; i < _levelinfofd.diffNum; i++) {
					Vec2 pos;
					vm = vec[i].asValueMap();
					pos.x = vm["X"].asInt();
					pos.y = vm["Y"].asInt();
					_levelinfofd.diffList.push_back(pos);
				}

				for (int i = 0; i < _levelinfofd.diffNum; i++) {
					log("readfile: %d, x = %f, y = %f",i,_levelinfofd.diffList[i].x,_levelinfofd.diffList[i].y);
				}
				
				AppDelegate *app = (AppDelegate*)Application::getInstance();
				//app->setPromoteProduct(_levelinfofd.sku);
				app->setPromoteProduct("");
				bRet = true;
			}
		}
	}while(0);

	return bRet;

}

bool FindDiffLayer::writeFile(const char* fileName) {
	
	ValueMap data;
	std::string path = FileUtils::getInstance()->getWritablePath() + fileName;

	ValueMap levelinfo;
	ValueVector levels;

	//Value level(1);
	//Value id("1.1");
	Value name("tweebot");
	Value image1("spot_it/LevelInfo/tweebot.jpg");
	Value image2("spot_it/LevelInfo/tweebot_cp.jpg");
	Value diffnum(5);
	ValueVector vecdiff;
	ValueMap diff1;
	diff1["X"] = 312;
	diff1["Y"] = 388;
	ValueMap diff2;
	diff2["X"] = 414;
	diff2["Y"] = 348;
	ValueMap diff3;
	diff3["X"] = 330;
	diff3["Y"] = 336;
	ValueMap diff4;
	diff4["X"] = 74;
	diff4["Y"] = 122;
	ValueMap diff5;
	diff5["X"] = 352;
	diff5["Y"] = 176;

	vecdiff.push_back(Value(diff1));
	vecdiff.push_back(Value(diff2));
	vecdiff.push_back(Value(diff3));
	vecdiff.push_back(Value(diff4));
	vecdiff.push_back(Value(diff5));

	//levelinfo["LEVEL"] = level;
	//levelinfo["ID"] = id;
	levelinfo["NAME"] = name;
	levelinfo["IMAGE1"] = image1;
	levelinfo["IMAGE2"] = image2;
	levelinfo["DIFFNUM"] = diffnum;
	levelinfo["DIFFS"] = vecdiff;
	data["1"] = Value(levelinfo);
	data["2"] = Value(levelinfo);
	data["3"] = Value(levelinfo);

	bool b = FileUtils::getInstance()->writeToFile(data,path);

	return true;
}

void FindDiffLayer::showHint(){
		

	do{
		//if (_gamestatus != GAME_START) {
		//	break;
		//}

		//_endPt = touch->getLocation();
		if(_bTlEnabled == false) return;
		_bTlEnabled = false;

		_isMoving = false;
		_isResizing = false;
		int prevfound = _df_found;
		Vec2 posError;
		int score = 0;

		if (_nTlimit > 0){
			AppDelegate *app =  (AppDelegate*) Application::getInstance();

			if(_nTlcount > 0){
				_nTlcount--;
				app->setToolsNumberInfo(_nTlcount);
			}else{
				if(_lTtlScore >= 10){
					
					showDeductPointEffect(SPOT_UI_TEXT_SCORE);

					score = 10;
					_lTtlScore = _lTtlScore - 10;
					AppDelegate *app =  (AppDelegate*) Application::getInstance();
					app->UpdateUserScore(0,10);
					


				}else{
					return;
				}

			}
			app->PlayEffect(SOUND_TOOL_CLICK);
			_nTlimit--;
			if (_nTlimit == 0){
				auto btnTips = dynamic_cast<Button*>(_UINode->getChildByName(SPOT_UI_BTN_TIPS));
				CC_BREAK_IF(!btnTips);
				btnTips->setEnabled(false);
				btnTips->setColor(ccc3(84,84,84));//not disabled button, so use color instead
			}

		}else {
			return;
		}
		//auto bound_img1 = this->getChildByTag(FINDDIFF_IMG_1)->getBoundingBox();
		Sprite * img1 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1); //get 1st Image sprite
		Sprite * img2 = (Sprite*) this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2); //get 1st Image sprite
		CC_BREAK_IF(img1 == nullptr);
		CC_BREAK_IF(img2 == nullptr);

		//auto bound_img1 = this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1)->getBoundingBox(); // get 1st image's boundingbox
		//auto bound_img2 = this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2)->getBoundingBox(); //get 2nd image's boundingbox
		//Vec2 touchinImg1 = this->getChildByTag(FINDDIFF_CLIPPER_1)->getChildByTag(FINDDIFF_IMG_1)->convertTouchToNodeSpace(touch);
		//Vec2 touchinImg2 = this->getChildByTag(FINDDIFF_CLIPPER_2)->getChildByTag(FINDDIFF_IMG_2)->convertTouchToNodeSpace(touch);

		auto hide_img1 = dynamic_cast <Sprite *> (_UINode->getChildByName(BG_IMG1));
		auto hide_img2 = dynamic_cast <Sprite *> (_UINode->getChildByName(BG_IMG2));
		CC_BREAK_IF(hide_img1 == nullptr);
		CC_BREAK_IF(hide_img2 == nullptr);


		for(auto sp : _vec_circle) { //Check touch position matches differences
			//log("sprite tag = %d", sp->getTag());
			if (sp->isVisible() == false) {
					sp->setVisible(true);	//set touched difference visiable
					auto sp2 = img2->getChildByTag(sp->getTag()+5);	//get touched difference sprite in 2nd image
					sp2->setVisible(true);	//set touched difference visiable in 2nd image
					_df_found++;  //update found differences count
					break;
				}
		}
		//
		for (int j = 0; j < _df_found; j++) {
			if (j < 5){ 
			_arrlight[j]->setBright(false);
			}
		}

		this->scheduleOnce(schedule_selector(FindDiffLayer::enableTools), 2.0f);

	} while(0);

	return;
}

void FindDiffLayer::ShowHintCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	//if (eEventType != ui::Widget::TouchEventType::BEGAN)
    if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
			FindDiffLayer::showHint(); 
	}
	return;
}


void FindDiffLayer::showDeductPointEffect(std::string sLabel){

	auto scoreLabel = dynamic_cast<Text*> (_UINode->getChildByName(sLabel.c_str()));
	if(scoreLabel == nullptr) return;

	//show score effect
	auto blink2 = Blink::create(1.5,3);
	auto tintto = TintTo::create(0.01f, 255.0f, 0.0f, 0.0f);
	auto tintback = TintTo::create(0.01f, 255.0f, 255.0f, 255.0f);
	auto seq1 = Sequence::create(tintto,blink2,tintback, nullptr);
	scoreLabel->runAction(seq1);

	return;
}

void FindDiffLayer::enableTools(float dt){
	_bTlEnabled = true;
}

void FindDiffLayer::delayWinQuit(float dt){
	auto scene = GameWin::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void FindDiffLayer::delayFailQuit(float dt){
	auto scene = GameFailed::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}
