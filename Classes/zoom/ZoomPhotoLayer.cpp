#include "ZoomPhotoLayer.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "AppDelegate.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "SimpleAudioEngine.h"
#include "../utilits/Tutorial.h"
#include "../MsgBox/MsgBox.h"
#include "cocostudio/CocoStudio.h"//for animation
#include "theme/ThemeLayer.h"

//#include "global_const.h"

using namespace cocostudio;
static int _nMsgQuit = 1;
static int _nMsgIncomplete = 2;
static int PREVIEW_TIME = 359;

ZoomPhotoLayer::ZoomPhotoLayer(){};

ZoomPhotoLayer::~ZoomPhotoLayer(){};

bool ZoomPhotoLayer::init(){

	bool bRet = false;
	do{

		int currenlevel = 0;
		CC_BREAK_IF(!Layer::init());

		_bzoomin = true; //set flag to zoom in
		_iHlpCount = 1;	//only allow to use once help count
		_gametime = GAME_TIME_EASY; //use 90 seconds

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		CC_BREAK_IF(!app);

		_nTlimit = 3; // total tools can be used in each game
		_nTlcount = app->getToolsNumberInfo(); // total tools user has
		_nLvl = app->GetUserLevel(); //totol tips can be used
		_fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
		_lTtlScore = app->GetUserTotalScore();		
		currenlevel = app->getCurrentGameLevel();

		//read level info from plist file
		//std::string str = ZOOM_LEVEL_INFO;
		//char lvl[10];
		//snprintf(lvl, 10, "%d", indexLvl);
		//CC_BREAK_IF(!readFile(str.c_str(),lvl));
		int indexLvl = currenlevel - 1;
		CC_BREAK_IF(!loadGameProduct(indexLvl));

		CC_BREAK_IF(!loadCSB());
		CC_BREAK_IF(!loadimage());
		CC_BREAK_IF(!loadbuttons());
		CC_BREAK_IF(!loadAnswerSprites());

		auto lvlbar = dynamic_cast<Sprite*> (_UINode->getChildByName(ZOOM_UI_LEVELBAR));
		CC_BREAK_IF(lvlbar == nullptr);
		lvlbar->setAnchorPoint(Vec2(0,0.5));
		//lvlbar->setPositionX(lvlbar->getPositionX()-lvlbar->getContentSize().width/2);
		lvlbar->setScaleX(_fPerToNLvl);

		auto btnTips = dynamic_cast<Button*>(_UINode->getChildByName(ZOOM_UI_BTN_TIPS));
		CC_BREAK_IF(btnTips == nullptr);
		btnTips->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::ShowHintCallback, this));

		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = CC_CALLBACK_2(ZoomPhotoLayer::onTouchBegan, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(ZoomPhotoLayer::onTouchEnded, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(ZoomPhotoLayer::onTouchMoved, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(ZoomPhotoLayer::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	
		if ( app->getBGMstatus() == BGMusic_flag::ON){
			app->StartBGMusic(BGM_GAME);
		}

		//load tutorial first time run
		//UserDefault *load = UserDefault::getInstance();
		//string sFirstFlip = load->getStringForKey("FIRSTTIME_ZOOM","0");
		//{
		//	if (atoi(sFirstFlip.c_str()) == 0)
		//	{
		//		scheduleOnce(schedule_selector(ZoomPhotoLayer::loadFirstTutorial), 1.1f);
		//		load->setStringForKey("FIRSTTIME_ZOOM", "1");
		//	}
		//}

		//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f);


		LoadPreviewLayer();
		_nMsgType = 0;
		_bTlEnabled = true;

		bRet = true;

	}while(0);

	return bRet;

}

void ZoomPhotoLayer::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");

	//auto location1 = touch->getLocation();
	//auto location2 = touch->getStartLocation();
	//auto distance = location1.distance(location2);
	//
	//auto touchtip = touch->getLocation();
	//auto spTips = dynamic_cast<Sprite*>(_UINode->getChildByName(ZOOM_UI_BTN_TIPS));
	//if(spTips == nullptr) return;

	//auto bound = spTips->getBoundingBox();
	//
	//if (distance > 3) {
	//	return;
	//}
	//if (bound.containsPoint(touchtip)){
	//	ZoomPhotoLayer::showHint();
	//	log("this is tips");
	//}
	return;

}
void ZoomPhotoLayer::loadFirstTutorial(float dt)
{
	//unschedule(schedule_selector(ZoomPhotoLayer::loadFirstTutorial));
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		Director::getInstance()->pushScene(scene);
	}
}
bool ZoomPhotoLayer::loadCSB() {
	//load all resources from csb
	
	bool bRet = false;
	do{
		_UINode = CSLoader::createNode(ZOOM_UINODE_NAME);
		CC_BREAK_IF(_UINode == nullptr);
		this->addChild(_UINode);

		auto game_pic = dynamic_cast<Sprite*>(_UINode->getChildByName(ZOOM_GAME_PICTURE));
		CC_BREAK_IF(game_pic == nullptr);
		game_pic->setVisible(false);
		_imgPosition = game_pic->getPosition();

		_gametimer = dynamic_cast<Text*>(_UINode->getChildByName(LABEL_TIMER));
		CC_BREAK_IF(_gametimer == nullptr);
		this->schedule(schedule_selector(ZoomPhotoLayer::updateTimer), 0.01f);

		auto btnMain = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_MAIN));
		CC_BREAK_IF(btnMain == nullptr);
		btnMain->setTag(MENU_MAIN);
		btnMain->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::menuStartCallback, this));

		auto btnQuit = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_TIPS));
		CC_BREAK_IF(btnQuit == nullptr);
		btnQuit->setTag(MENU_TIPS);
		btnQuit->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::menuStartCallback, this));

		auto btnSetting = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_SETTING));
		CC_BREAK_IF(btnSetting == nullptr);
		btnSetting->setVisible(false);
		btnSetting->setTag(MENU_SETTING);
		btnSetting->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::menuStartCallback, this));

		auto btnVoice = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_VOICE));
		CC_BREAK_IF(btnVoice == nullptr);
		btnVoice->setVisible(false);
		//btnVoice->setTag(MENU_VOICE);
		//btnVoice->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::menuStartCallback, this));

		auto btnMagnifier = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_MAGNIFIER));
		CC_BREAK_IF(btnMagnifier == nullptr);
		btnMagnifier->setVisible(false);

		auto btnClear = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_CLEAR));
		CC_BREAK_IF(btnClear == nullptr);
		btnClear->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::btnClear, this));

		auto btnSubmit = dynamic_cast<Button*>(_UINode->getChildByName(BUTTON_SUBMIT));
		CC_BREAK_IF(btnSubmit == nullptr);
		btnSubmit->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::btnSubmit, this));

		//Add by Long for theme mode
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->iGameMode == MobileGameMode::Theme_Mode){
			//btnMain->setVisible(false);
			//btnSetting->setVisible(false);
			btnVoice->setVisible(false);
			btnQuit->setVisible(false);
		}
		//Add by Long EOF

		//-----------------------------------add animation----------------------------------------------------------

		auto mastco = dynamic_cast<ImageView *> (_UINode->getChildByName(MASCOTT));
		//int mastcoZOrder=-1;
		if (mastco)
		{
			//mastcoZOrder = mastco->getZOrder();
			mastco->setVisible(false);
		}

		ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMATION_FILE);
		Armature *armature = Armature::create(ARMATURE);
		armature->setAnchorPoint(Vec2(0.5, 0.5));
		armature->setVisible(true);
		armature->setTag(888);
		nAniFlag = 1;
		auto imgBackFrame = dynamic_cast<ImageView*> (_UINode->getChildByName(BACK_FRAME));
		CC_BREAK_IF(imgBackFrame == nullptr);
		imgBackFrame->setZOrder(1);
		auto spFrame = dynamic_cast<Sprite*> (_UINode->getChildByName(FRAME));
		CC_BREAK_IF(spFrame == nullptr);
		spFrame->setZOrder(1);
		char cSpName[10];
		for (int i = 1; i < 11; i++)
		{
			sprintf(cSpName, "Sprite_%d", i);
			string strSpName = cSpName;
			auto spSprite = dynamic_cast<Sprite*> (_UINode->getChildByName(strSpName));
			CC_BREAK_IF(spSprite == nullptr);
			spSprite->setZOrder(2);

		}
		schedule(schedule_selector(ZoomPhotoLayer::PlayAnimation), 5.0f);
		_UINode->addChild(armature);
		//-------------------------------------------------------------------------------------
		bRet = true;
	}while(0);

	return bRet;

}

void ZoomPhotoLayer::PlayAnimation(float dt)
{

	Armature *armature = dynamic_cast<Armature *>(_UINode->getChildByTag(888));
	if (nAniFlag == 1)
	{
		armature->setPosition(Vec2(135, 400));
		armature->setRotation(270);
		nAniFlag = 2;
	}
	else
	{
		armature->setPosition(Vec2(700, 430));
		armature->setRotation(0);
		nAniFlag = 1;
	}
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	armature->getAnimation()->setAnimationScale(0.8);
}

bool ZoomPhotoLayer::loadimage(){

	
	bool bRet = false;
	do{

		Size visiableSize = Director::getInstance()->getVisibleSize();
		Point origin = Director::getInstance()->getVisibleOrigin();

		auto stencil = Sprite::create(ZOOM_DUMMY_IMAGE);
		auto clipper = ClippingNode::create();
		CC_BREAK_IF(stencil == nullptr);
		CC_BREAK_IF(clipper == nullptr);

		auto game_pic = dynamic_cast<Sprite*>(_UINode->getChildByName(ZOOM_GAME_PICTURE));
		CC_BREAK_IF(game_pic == nullptr);
		float w1 = game_pic->getBoundingBox().size.width;
		float w2 = stencil->getContentSize().width;
		stencil->setScale(w1/w2);

		clipper->setStencil(stencil);//
		clipper->setInverted(false);//
		clipper->setAlphaThreshold(0);//Set Alpha value to 0
		clipper->setTag(CLIPPER_TAG);
		this->addChild(clipper,99);//4


		CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(_levelInfoZM.image));
		Sprite *photo  = CCSprite::create(_levelInfoZM.image);
		CC_BREAK_IF(photo == nullptr);
		photo->setTag(PHOTO_MAX_TAG);
		_fscalefactor = w1 / float(photo->getContentSize().width);
		_nZmLevel = 2.5; //set zoom level to 2x
		photo->setScale(_nZmLevel*_fscalefactor);
		
		clipper->setPosition(_imgPosition);
		clipper->addChild(photo);

		bRet = true;
	}while(0);

	return bRet;
}

bool ZoomPhotoLayer::loadbuttons(){

	bool bRet = false;
	do{
		_nCurAns = 0;

		//Initial letters and correct answers
		//Add correct answers
		_vec_alphabet.clear();
 		_vec_Opt.clear();

		for(int i = 0; i < _levelInfoZM.answer.size(); i++){
			_vec_Opt.push_back(_levelInfoZM.answer[i]);
		}

		//Add random letters
		int N = 0;
		for (int i = 0; i < (12 - _levelInfoZM.answer.size()); i++){
			N = rand() % 25;
			_vec_Opt.push_back(LETTER_TAG_A + N);
		}
		//Random sequence
		srand(unsigned(time(NULL)));
		std::random_shuffle(_vec_Opt.begin(),_vec_Opt.end());

		std::vector<int>::const_iterator v3;
		v3 = _vec_Opt.begin();
		std::string emptystring = "";
	
		//load letters to button
		for (int j = 0; j < 2 ; j++){
			for (int k = 0; k < 6; k++) {

				std::string strbtn = ZOOM_LETTER_BUTTON;
				std::string btnN = ZOOM_LETTER_NORMAL;
				std::string btnP= ZOOM_LETTER_PRESS;
				std::string btnD= ZOOM_LETTER_DISABLE;
				std::size_t found = strbtn.find_first_of("X");

				strbtn[found] = LETTER_NUM_1 + j;
				found = strbtn.find_first_of("Y");
				strbtn[found] = LETTER_NUM_1 + k;
				found = btnN.find_first_of("X");
				btnN[found] = *v3;
				found = btnP.find_first_of("X");
				btnP[found] = *v3;
				cocos2d::log("current button name is %S", strbtn.c_str());
				_arr_btn[j][k] = dynamic_cast<Button*>(_UINode->getChildByName(strbtn));
				CC_BREAK_IF(_arr_btn[j][k] == nullptr);

				_arr_btn[j][k]->setTitleText(emptystring);
				//_arr_btn[j][k]->setSize(Size(49.f,56.f));
				_arr_btn[j][k]->setTag(*v3);
				_arr_btn[j][k]->loadTextureNormal(btnN);
				_arr_btn[j][k]->loadTexturePressed(btnP);
				_arr_btn[j][k]->loadTextureDisabled(btnP);
				_arr_btn[j][k]->setSize(getImageSize(btnN.c_str()));
				_arr_btn[j][k]->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::onTouchLetter, this));
				v3++;
			}
		}
		bRet = true;

	}while(0);

	return bRet;
}

bool ZoomPhotoLayer::loadAnswerSprites() {

	bool bRet = false;
	do{
		auto bg = dynamic_cast<ImageView*>(_UINode->getChildByName(BACK_FRAME));
		CC_BREAK_IF(bg == nullptr);

		std::vector <Vec2> vecPos; 

		int total = _levelInfoZM.answer.length();
		for (int i = 0; i < 10; i++){
		std::string str = "Sprite_";

		std::string s;
		std::stringstream out;
		out << (i+1);
		s = out.str();
		str.append(s);
		log("%s", str.c_str());

		//std::string str2 = std::to_string(i+1); //Not working in Android system
		auto sp = dynamic_cast<Sprite*>(_UINode->getChildByName(str.c_str()));
		CC_BREAK_IF(sp == nullptr);

		vecPos.push_back(sp->getPosition());
		if (i < total){
				//sp->setTexture("/games_lance/letters/normal/X.png");
				//sp->setContentSize(Size(45,45));
				//sp->setTextureRect(Rect(0.0f,0.0f,70.0f,70.0f));
				_vec_answer.push_back(sp);
			}
			else {
				sp->setVisible(false);
			}

		}

		int start = (10 - total)/2; 
		for (int i = 0; i < total; i++) {
			_vec_answer[i]->setPosition(vecPos[i+start]);

		}

		bRet = true;
	}while(0);
	return bRet;

}

void ZoomPhotoLayer::menuStartCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	/*
	SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	this->removeChildByTag(BIRD_SPRITE_TAG);
	auto scene = GameScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
	*/
	if (eEventType != ui::Widget::TouchEventType::ENDED) return;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_BUTTON_CLICK);
	}

	CCNode* node = (CCNode*) pSender;
	int tag = node->getTag();
	if (tag == MENU_TIPS) {
		//ZoomPhotoLayer::setBGMusic();
		auto scene = TutorialScene::create();
		if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->pushScene(transition);
		}

	}
	else if (tag == MENU_VOICE) {
		//ZoomPhotoLayer::btnClear(sender);
	/*	ZoomPhotoLayer::setBGMusic();*/
	}
	else if (tag == MENU_SETTING) {
		//ZoomPhotoLayer::btnClear(sender);
		//ZoomPhotoLayer::test();
		playAnimation();
	}
	else if (tag == MENU_MAIN) {
		//ZoomPhotoLayer::btnClear(sender);
		ZoomPhotoLayer::quitgame();
	} 
	return;
	}

void ZoomPhotoLayer::menuKeyCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){

	if (eEventType != ui::Widget::TouchEventType::ENDED) return;

	CCNode* node = (CCNode*) pSender;
	auto btn = (Button*) pSender;

	int tag = node->getTag();
	char c;
	c = tag;

	//sp->setTextureRect(Rect(0.0f,0.0f,70.0f,70.0f));
	std::string str = ZOOM_LETTER_NORMAL;
	std::size_t found = str.find_first_of("X");
	str[found] = c;

	if (_nCurAns < _levelInfoZM.answer.length()) {
		_vec_answer[_nCurAns]->setTexture(str);
		_vec_answer[_nCurAns]->setTag(c);
		_nCurAns++;
	}
	btn->setEnabled(false);
	btn->setBright(false);

	return;
}

void ZoomPhotoLayer::btnSubmit(Ref *pSender, Widget::TouchEventType type){
	if (type != ui::Widget::TouchEventType::ENDED) return;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->PlayEffect(SOUND_BUTTON_CLICK);
	}

	if (_nCurAns == _levelInfoZM.answer.length()) {
		std::string str = "";
		for(int i = 0; i < _levelInfoZM.answer.length(); i++){
			char c = _vec_answer[i]->getTag();
			str.append(1u,c);
		}
		if (_levelInfoZM.answer.compare(str) == 0) {
			gamewin();
		}
		else{ //Add by Long
			gamelose();
		}
	}
	else{
		//Add by Long
		//cocos2d::MessageBox(ZOOM_MSG_LESSLETTER, ZOOM_MSG_TITLE);

		MsgBoxType m_type = TP_INFO;
		std::string title = ZOOM_MSG_TITLE;
		std::string msg = ZOOM_MSG_LESSLETTER;
		MsgBox::setMsgBox(m_type,&title,&msg);
		auto msgbox = MsgBox::create();
		msgbox->setTag(MSGBOX_TAG);
		this->addChild(msgbox,999);
	
		//Add Event to process
		auto btnOk = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_OK"));// Magnifier Button
		//auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
		if(btnOk == nullptr) return;
		btnOk->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::okCallback, this));
		_nMsgType = _nMsgIncomplete;

		return;
		//showMessage();
	}
}

void ZoomPhotoLayer::btnClear(Ref *pSender, Widget::TouchEventType type){

	if (type != ui::Widget::TouchEventType::ENDED) return;
	
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON){
		log("bgm volume is %f",SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
		log("effect volume is %f",SimpleAudioEngine::sharedEngine()->getEffectsVolume());
		app->PlayEffect(SOUND_BUTTON_CLICK);
	}
	std::string str = ZOOM_ANSWER_LABEL;

	for (int i = 0; i < _levelInfoZM.answer.length(); i++){
		_vec_answer[i]->setTexture(str.c_str());
		_vec_answer[i]->setTag(0);
	}

	for (int j = 0; j < 2 ; j++){
		for (int k = 0; k < 6; k++) {
			_arr_btn[j][k]->setEnabled(true);
			_arr_btn[j][k]->setBright(true);
		}
	}
	_nCurAns = 0;
	return;

}

void ZoomPhotoLayer::btnZoomCallback(Object *sender){

	//redeem user points to help 
	//user can use 1 free chance, after 1 free deduct 1o points each time
	if (_iHlpCount >= 1){
		_iHlpCount--;
		//MessageBox("m_iUserTotalScore > 10", "Success");		
		if (m_iUserTotalScore > 10) {//
			if (updatescore(2,10) != true){
				MessageBox("fail to update user score", "Fail");
				return;
			}
		} else {
				//m_iUserTotalScore -= 10;
				MessageBox("Less than 10 points, fail to update user score", "Fail");
				return;
		}
	} else{
		MessageBox("Sorry, no enough points", "Points not enough");		
	}

	ClippingNode* clipper = (ClippingNode*) this->getChildByTag(CLIPPER_TAG); 
	Sprite* photo_max = (Sprite*) clipper->getChildByTag(PHOTO_MAX_TAG);

	if (_bzoomin == true) {
		if (_nZmLevel > 1) {
			_nZmLevel = _nZmLevel - 0.5;
			photo_max->setScale(_nZmLevel * _fscalefactor);
		}
	}
	else {
		if (_nZmLevel < 2) {
			_nZmLevel = _nZmLevel + 0.5;
			photo_max->setScale(_nZmLevel,_nZmLevel);
			if (_nZmLevel == 2) {
				_bzoomin = true;
			}

		}
	}

	return;
}

bool ZoomPhotoLayer::onTouchBegan(Touch* touch, Event* event)
{
   //labelTouchInfo->setPosition(touch->getLocation());
   //labelTouchInfo->setString("You Touched Here");
	cocos2d::log("touch Began");
	return true;
}

void ZoomPhotoLayer::onTouchMoved(Touch* touch, Event* event)
{
   cocos2d::log("touch moved");
}

void ZoomPhotoLayer::onTouchCancelled(Touch* touch, Event* event)
{
   cocos2d::log("touch cancelled");
}

void ZoomPhotoLayer::updateTimer(float dt) {

	if(_bIsPreview == true){
		int seconds = _nPreviewSec / 60;
		char cTimer[20];
		sprintf(cTimer,"00:%02d", seconds);
		string str = cTimer;
		auto lbl = dynamic_cast<Text*> (this->getChildByTag(ZOOM_TAG_PREVIEW)->getChildByName(LABEL_TIMER));
		if (lbl == nullptr) return;
		lbl->setString(str);
		lbl->setFontSize(20);
		if (_nPreviewSec <= 0) ZoomPhotoLayer::RemovePreviewLayer();
		_nPreviewSec -= 1;
		return;
	}


	_gametime -= 1;
	int ttlseconds = _gametime / 60;
	int seconds = ttlseconds % 60;
	int min = ttlseconds / 60;
	char lblTimer[100];
	sprintf(lblTimer,"%d:%02d", min, seconds);
	string string1 = lblTimer;
	auto timerLabel = dynamic_cast<Text*> (_UINode->getChildByName(LABEL_TIMER));
	timerLabel->setString(string1);
	timerLabel->setFontSize(20);

	if (_gametime < 10 * 60) {
			timerLabel->setColor(ccc3(255,0,0));
	}

	char lblTips[10];
	sprintf(lblTips,"%d", _nTlcount);
	string string2 = lblTips;
	auto tipsLabel = dynamic_cast<Text*> (_UINode->getChildByName(ZOOM_UI_TEXT_TIPS));
	tipsLabel->setString(string2);

	char lblLvl[10];
	sprintf(lblLvl,"%d", _nLvl);
	string string3 = lblLvl;
	auto lvlLabel = dynamic_cast<Text*> (_UINode->getChildByName(ZOOM_UI_TEXT_LEVELBAR));
	lvlLabel->setString(string3);

	char lblScore[20];
	sprintf(lblScore,"%lu", _lTtlScore);
	string string4 = lblScore;
	auto scoreLabel = dynamic_cast<Text*> (_UINode->getChildByName(ZOOM_UI_TEXT_SCORE));
	scoreLabel->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	scoreLabel->setString(string4);

	//resize image every 30 sec
	if ((ttlseconds == 60 && _nZmLevel ==2.5) || 
		(ttlseconds == 30 && _nZmLevel == 2)|| 
		(ttlseconds == 10 && _nZmLevel == 1.5)){
		ClippingNode* clipper = (ClippingNode*) this->getChildByTag(CLIPPER_TAG); 
		Sprite* photo_max = (Sprite*) clipper->getChildByTag(PHOTO_MAX_TAG);

		if (_bzoomin == true) {
			if (_nZmLevel > 1) {
				_nZmLevel = _nZmLevel - 0.5;
				photo_max->setScale(_nZmLevel*_fscalefactor);
			}
		}
		//else {
		//	if (_nZmLevel < 2) {
		//		_nZmLevel = _nZmLevel + 0.5;
		//		photo_max->setScale(_nZmLevel,_nZmLevel);
		//		if (_nZmLevel == 2) {
		//			_bzoomin = true;
		//		}
		//	}
		//}
	}

	updateProgress();
}

void ZoomPhotoLayer::updateProgress(){

	if (_gametime <= 0) {
		gamelose();
	}
	
}

void ZoomPhotoLayer::onTouchLetter(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
	if (eEventType != ui::Widget::TouchEventType::BEGAN)

	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			ZoomPhotoLayer::menuKeyCallback(pSender,ui::Widget::TouchEventType::ENDED);
			break;
		case ui::Widget::TouchEventType::MOVED:
			cocos2d::log("touch Moved");
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

void ZoomPhotoLayer::onTouchZoom(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
	if (eEventType != ui::Widget::TouchEventType::ENDED) return;

	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			ZoomPhotoLayer::btnZoomCallback(pSender);
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

void ZoomPhotoLayer::quitgame(){
	do{
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
	CC_BREAK_IF(btnYes == nullptr);
	CC_BREAK_IF(btnNo == nullptr);
	btnYes->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::yesCallback, this));
	btnNo->addTouchEventListener(CC_CALLBACK_2(ZoomPhotoLayer::noCallback, this));

	_nMsgType = _nMsgQuit;

	}while(0);
	return;
}

void ZoomPhotoLayer::gamewin(){
	//Update user score
	long curscore = m_iUserTotalScore;
	int score = 0;
	int ttlseconds = _gametime / 60;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	//Modify by Long 
	this->unschedule(schedule_selector(ZoomPhotoLayer::updateTimer));

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


	//AppDelegate *app = (AppDelegate*)Application::getInstance();
	if (app->iGameMode == MobileGameMode::Theme_Mode){
		app->GameRunRoundNext();
	}
	else{
		//app->UpdateUserScore(1,score);
		//app->setReward(score);
		app->updateGameWinScore(ttlseconds);
		app->SaveGamePassLevel();//Add by Long to Save Level
		app->moveToNextGameLevel();

		this->scheduleOnce(schedule_selector(ZoomPhotoLayer::delayWinQuit),1.0f);

		//auto scene = GameWin::createScene();
		//TransitionScene *transition = TransitionFade::create(1, scene);
		//Director::getInstance()->replaceScene(transition);
	}
}

void ZoomPhotoLayer::gamelose(){
	//modify by Long
	this->unschedule(schedule_selector(ZoomPhotoLayer::updateTimer));

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	app->moveToNextGameLevel();//Add by Long to Save Level
	this->scheduleOnce(schedule_selector(ZoomPhotoLayer::delayFailQuit),1.0f);
	//auto scene = GameFailed::createScene();
	//TransitionScene *transition = TransitionFade::create(1, scene);
	//Director::getInstance()->replaceScene(transition);
}

void ZoomPhotoLayer::btnBack(Ref* pSender, ui::Widget::TouchEventType eEventType){

	if (eEventType != ui::Widget::TouchEventType::ENDED) return;

	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			ZoomPhotoLayer::quitgame(); //quit the game
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


void ZoomPhotoLayer::pause(){

	//auto sp = (Sprite*) this->getChildByTag(9999);
	//if (sp = nullptr){
	//	log("sp is empty");
	//	return;
	//}

	//if (_pause == 1){
	//	sp->setVisible(true);
	//	if (_pause == 1) {
	//		_pause++;
	//		this->scheduleOnce(schedule_selector(ZoomPhotoLayer::test3), 0.1f);

	//		//test2();
	//	}
	//	//test();
	//} else if (_pause == 0) {
	//	this->getChildByTag(9999)->setVisible(false);	
	//} else if (_pause == 2){
	//	this->getChildByTag(9999)->setVisible(true);
	//}

	//std::string str = LEVEL_INFO_FILE;
	//std::string str = "levels_zm.plist";
	//std::string id = "1-1";
	//readFile(str.c_str(),id.c_str());
	//int ret = 0;
	//
	//auto obj = Node::create();
	//string data = "this is a test";
	//obj->setTag(9999);
	//obj->setUserData(&data);
	//addChild(obj);
	////dlg->getValue(&ret);
 //   addChild(dlg,99);
 //   printf("return value is %x\n",ret);  

}

Size ZoomPhotoLayer::getImageSize(const char* filename) {
	Size ret;
	auto sprite = Sprite::create(filename);
    if(sprite){
        return ret = Size(sprite->boundingBox().size.width, sprite->boundingBox().size.height);
    }else{
        return ret=Size(0,0);
    }

}

bool ZoomPhotoLayer::loadGameProduct(int gamelvl){
	bool ret = false;
	do {
		std::vector<_ProductInfo>*	pPrdInfoList;
		pPrdInfoList = DataAccess::getProductListbyGame(m_iCurrentGameID);
		CC_BREAK_IF(pPrdInfoList == nullptr);
		_ProductInfo PDInfo;
		LEVEL_INFO_ZM zmpdInfo;

		std::vector<int> levelNum;
		std::vector<int> levelNum_all;
		int nSize = pPrdInfoList->size();
		levelNum_all.clear();
		levelNum.clear();

		for(int i = 0; i < nSize; i++){
			levelNum_all.push_back(i);
		}

		std::random_device rd1;
		std::mt19937 g1(rd1());
 		std::shuffle(std::begin(levelNum_all), std::end(levelNum_all), g1);
		//levelNum_all.push_back(gamelvl);
		
		for(int n = 0; n < 6; ++n){
			if (levelNum_all[n] != gamelvl) levelNum.push_back(levelNum_all[n]);// generate numbers
			if (levelNum.size() == 5) break;
		}

		//get current level product info
		_levelInfoZM.name = pPrdInfoList->at(gamelvl).NAME;
		_levelInfoZM.image = pPrdInfoList->at(gamelvl).IMAGE;
		_levelInfoZM.answer = pPrdInfoList->at(gamelvl).SHORTNAME;
		_levelInfoZM.sku = pPrdInfoList->at(gamelvl).SKU;
		_levelInfoZM.timelimit = GAME_TIME_EASY;

		//load product into preview product list
		_ttlProductlist.clear();
		_ttlProductlist.push_back(_levelInfoZM);

		if(pPrdInfoList->size() > 0){
			for (int i = 0; i < levelNum.size(); i++){
				zmpdInfo.name = pPrdInfoList->at(levelNum.at(i)).NAME;
				zmpdInfo.image = pPrdInfoList->at(levelNum.at(i)).IMAGE;
				zmpdInfo.answer = pPrdInfoList->at(levelNum.at(i)).SHORTNAME;
				zmpdInfo.sku = pPrdInfoList->at(levelNum.at(i)).SKU;
				zmpdInfo.timelimit = GAME_TIME_EASY;
				_ttlProductlist.push_back(zmpdInfo);
			}
		}

		//random 6 product in the preview list
		std::random_device rd2;
		std::mt19937 g2(rd2());
 		std::shuffle(std::begin(_ttlProductlist), std::end(_ttlProductlist), g2);

		//set promote product
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->setPromoteProduct(_levelInfoZM.sku);
		ret = true;
	}while(0);
 	
	return ret;
}
bool ZoomPhotoLayer::readFile(const char* fileName,const char* levelID) {

	bool ret = false;

	do {

			if(0){
				string id = levelID;
				ValueMap zoom = FileUtils::getInstance()->getValueMapFromFile(fileName);
				ValueMap levels = zoom["ZOOM"].asValueMap();
				if (levels.size() > 0) {
					auto it = levels.find(levelID);
				/*	if (it != nullptr){
					}*/
					if (it != levels.end()){
						ValueMap levelinfo = it->second.asValueMap();
						_levelInfoZM.name = levelinfo["NAME"].asString();
						_levelInfoZM.image = levelinfo["IMAGE"].asString();
						_levelInfoZM.answer = levelinfo["ANSWER"].asString();
						//_levelInfoZM.sku = levelinfo["SKU"].asString();
						_levelInfoZM.timelimit = GAME_TIME_EASY;
						ret = true;

					}
				}


				//set promote product
				AppDelegate *app = (AppDelegate*)Application::getInstance();
				//app->setPromoteProduct(_levelInfoZM.sku);
				app->setPromoteProduct("");
				/*************************Add random product************************************/
				std::vector<int> levelNum;
				std::vector<int> levelNum_all;
				int nSize = levels.size();
				int nCurLvl = atoi(levelID);

				levelNum_all.clear();
				levelNum.clear();

				for(int i = 1; i <= nSize; i++){
					levelNum_all.push_back(i);
				}

				std::random_device rd1;
				std::mt19937 g1(rd1());
 				std::shuffle(std::begin(levelNum_all), std::end(levelNum_all), g1);

				for(int n=0; levelNum.size() <= 4; ++n){
					if (levelNum_all[n] != nCurLvl) levelNum.push_back(levelNum_all[n]);// generate numbers
					if (levelNum.size() == 5) break;
				}

				_ttlProductlist.clear();
				LEVEL_INFO_ZM pdInfo = _levelInfoZM;
				_ttlProductlist.push_back(pdInfo);

				char clvl[10];
				for (int i = 0; i < levelNum.size(); i++){
					snprintf(clvl, 10, "%d", levelNum[i]);
					auto it = levels.find(clvl);
					if (it != levels.end()){
						ValueMap levelinfo = it->second.asValueMap();
						pdInfo.name = levelinfo["NAME"].asString();
						pdInfo.image = levelinfo["IMAGE"].asString();
						pdInfo.answer = levelinfo["ANSWER"].asString();
						_ttlProductlist.push_back(pdInfo);
					}
				}

				//random 6 product in the preview list
				std::random_device rd2;
				std::mt19937 g2(rd2());
 				std::shuffle(std::begin(_ttlProductlist), std::end(_ttlProductlist), g2);

			/*************************Add random product************************************/
			}
			else{

				std::vector<_ProductInfo>*	pPrdInfoList;
				pPrdInfoList = DataAccess::getProductListbyGame(m_iCurrentGameID);
				CC_BREAK_IF(pPrdInfoList == nullptr);
				_ProductInfo PDInfo;
				LEVEL_INFO_ZM zmpdInfo;
				int nCurLvl = atoi(levelID); //get current game index

				//get current level product info
				_levelInfoZM.name = pPrdInfoList->at(nCurLvl).NAME;
				_levelInfoZM.image = pPrdInfoList->at(nCurLvl).IMAGE;
				_levelInfoZM.answer = pPrdInfoList->at(nCurLvl).SHORTNAME;
				_levelInfoZM.sku = pPrdInfoList->at(nCurLvl).SKU;
				_levelInfoZM.timelimit = GAME_TIME_EASY;


				std::vector<int> levelNum;
				std::vector<int> levelNum_all;
				int nSize = pPrdInfoList->size();

				levelNum_all.clear();
				levelNum.clear();

				for(int i = 0; i < nSize; i++){
					levelNum_all.push_back(i);
				}

				std::random_device rd1;
				std::mt19937 g1(rd1());
 				std::shuffle(std::begin(levelNum_all), std::end(levelNum_all), g1);

				for(int n = 0; n < 5; ++n){
					if (levelNum_all[n] != nCurLvl) levelNum.push_back(levelNum_all[n]);// generate numbers
					if (levelNum.size() == 5) break;
				}

				//load product into preview product list
				_ttlProductlist.clear();
				_ttlProductlist.push_back(_levelInfoZM);


				if(pPrdInfoList->size() > 0){
					for (int i = 0; i < levelNum.size(); i++){
						zmpdInfo.name = pPrdInfoList->at(levelNum.at(i)).NAME;
						zmpdInfo.image = pPrdInfoList->at(levelNum.at(i)).IMAGE;
						zmpdInfo.answer = pPrdInfoList->at(levelNum.at(i)).SHORTNAME;
						zmpdInfo.sku = pPrdInfoList->at(levelNum.at(i)).SKU;
						zmpdInfo.timelimit = GAME_TIME_EASY;
						_ttlProductlist.push_back(zmpdInfo);
					}
				}

				//random 6 product in the preview list
				std::random_device rd2;
				std::mt19937 g2(rd2());
 				std::shuffle(std::begin(_ttlProductlist), std::end(_ttlProductlist), g2);

				//set promote product
				AppDelegate *app = (AppDelegate*)Application::getInstance();
				app->setPromoteProduct(_levelInfoZM.sku);
				//app->setPromoteProduct("");
			}
		ret = true;
	}while(0);
 	
	return ret;
}

bool ZoomPhotoLayer::writeFile(const char* fileName) {

	ValueMap data;
	std::string path = FileUtils::getInstance()->getWritablePath() + fileName;
	ValueMap levelinfo;

	levelinfo["NAME"] = "CASTLE";
	levelinfo["IMAGE"] = ZOOM_DUMMY_IMAGE;
	levelinfo["ANSWER"] = "CASTLE";
	data["1"] = Value(levelinfo);
	data["2"] = Value(levelinfo);
	data["3"] = Value(levelinfo);

	bool b = FileUtils::getInstance()->writeToFile(data,path);

	return true;
}

void ZoomPhotoLayer::showMessage(){

	//Sprite *msgbox= Sprite::create("games_lance/zoom_wood_displaybox.png");
	//Size visiableSize = Director::getInstance()->getVisibleSize();
	//Vec2 cp;
	//cp.x = visiableSize.width / 2;
	//cp.y = visiableSize.height / 2;
	//msgbox->setPosition(cp);
	//msgbox->setScaleY(2.5f);
	//msgbox->setTag(1000);
	//this->addChild(msgbox,100);
	//auto show = Show::create();
	//msgbox->runAction(show);

}
void ZoomPhotoLayer::showHint(){

	do{
		
		//_nTlimit = 3;
		_nTlcount = 10;
		
		if(_bTlEnabled == false) return;
		_bTlEnabled = false;
		

		int score = 0;		
		AppDelegate *app =  (AppDelegate*) Application::getInstance();
		
		if (_nTlimit > 0){
				if(_nTlcount > 0){
					_nTlcount--;
					app->setToolsNumberInfo(_nTlcount);
				}else{
					if(_lTtlScore >= 10){

						showDeductPointEffect(ZOOM_UI_TEXT_SCORE);

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
					auto btnTips = dynamic_cast<Button*>(_UINode->getChildByName(ZOOM_UI_BTN_TIPS));
					CC_BREAK_IF(!btnTips);
					btnTips->setEnabled(false);
					btnTips->setColor(ccc3(84,84,84));//not disabled button, so use color instead
				}
			}else {
				return;
			}

	//	if (_nTlimit > -1){
			//AppDelegate *app = (AppDelegate*)Application::getInstance();
			//if (app->getBGMstatus() == BGMusic_flag::ON){
			//	log("bgm volume is %f",SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
			//	log("effect volume is %f",SimpleAudioEngine::sharedEngine()->getEffectsVolume());
			//	app->PlayEffect(SOUND_BUTTON_CLICK);
			//}

			//clean all answers
			std::string str = ZOOM_ANSWER_LABEL;
			for (int i = 0; i < _levelInfoZM.answer.length(); i++){
				_vec_answer[i]->setTexture(str.c_str());
				_vec_answer[i]->setTag(0);
			}

			for (int j = 0; j < 2 ; j++){
				for (int k = 0; k < 6; k++) {
					_arr_btn[j][k]->setEnabled(true);
					_arr_btn[j][k]->setBright(true);
				}
			}
			_nCurAns = 0;

			//find the first answer
			int nAns  = 3 - _nTlimit;
			char c;
			Button* pBtn = nullptr;
			int l = 0;
			int tag = 0;

			for (int l = 0; l < nAns;l++){
				if (nAns == _nCurAns) break;
				c = _levelInfoZM.answer.at(l);
				for (int j = 0; j < 2 ; j++){
				if (nAns == _nCurAns) break;
					for (int k = 0; k < 6; k++) {
						if (nAns == _nCurAns) break;
						if ((c == _arr_btn[j][k]->getTag()) && (_arr_btn[j][k]->isEnabled() == true)){
							pBtn = _arr_btn[j][k];
							
							//c = pBtn->getTag();
							//sp->setTextureRect(Rect(0.0f,0.0f,70.0f,70.0f));
							std::string strLbl = ZOOM_LETTER_NORMAL;
							std::size_t found = strLbl.find_first_of("X");
							strLbl[found] = c;

							if (_nCurAns < _levelInfoZM.answer.length()) {
								_vec_answer[_nCurAns]->setTexture(strLbl);
								_vec_answer[_nCurAns]->setTag(c);
								_nCurAns++;
							}
							//disable button
							pBtn->setEnabled(false);
							pBtn->setBright(false);
							break;
						}
					}
				}
			}








		//}
		this->scheduleOnce(schedule_selector(ZoomPhotoLayer::enableTools), 2.0f);
	}while(0);

	return;

	//Sprite *msgbox= Sprite::create("games_lance/zoom_wood_displaybox.png");
	//Size visiableSize = Director::getInstance()->getVisibleSize();
	//Vec2 cp;
	//cp.x = visiableSize.width / 2;
	//cp.y = visiableSize.height / 2;
	//msgbox->setPosition(cp);
	//msgbox->setScaleY(2.5f);
	//msgbox->setTag(1000);
	//this->addChild(msgbox,100);
	//auto show = Show::create();
	//msgbox->runAction(show);

}

bool ZoomPhotoLayer::updatescore(int iFlag,int iScore){
	AppDelegate *app = (AppDelegate*) Application::getInstance();
	app->UpdateUserScore(iFlag,iScore);
	return true;
}

void ZoomPhotoLayer::setBGMusic(){
	AppDelegate *app = (AppDelegate*) Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON){
		app->StopBGMusic();
	}else{
		app->StartBGMusic(BGM_MAIN);
	}
	//app->setBGMusic();
	
	return;
}

void ZoomPhotoLayer::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

			AppDelegate *app = (AppDelegate*)Application::getInstance();
			if (app->getBGMstatus() == BGMusic_flag::ON){
				app->PlayEffect(SOUND_BUTTON_CLICK);
			}		
			this->unschedule(schedule_selector(ZoomPhotoLayer::updateTimer));
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

		CCLOG("touch ended.");
    }
}

void ZoomPhotoLayer::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {
			
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}		
		//Don't quit the game
		this->removeChildByTag(MSGBOX_TAG);

		//enable the touch event
        CCLOG("touch ended.");
    }
}

void ZoomPhotoLayer::okCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}		
		//Don't quit the game
		this->removeChildByTag(MSGBOX_TAG);		

		CCLOG("touch ended.");
    }
}

void ZoomPhotoLayer::ShowHintCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	//if (eEventType != ui::Widget::TouchEventType::BEGAN)
    if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
			ZoomPhotoLayer::showHint(); 
	}
	return;
};

void ZoomPhotoLayer::playAnimation(){

	//std::string strplist = "res/Media/animation/20.plist";
	//std::string strPicFile = "res/Media/animation/20.png";
	std::string strplist = "res/Media/animation/coin0.plist";
	std::string strPicFile = "res/Media/animation/coin0.png";

	SpriteBatchNode* spritebatch = SpriteBatchNode::create(strPicFile.c_str());
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(strplist.c_str());
	auto Sprite1 = Sprite::createWithSpriteFrameName("coin0001.png");
	//auto Sprite1 = Sprite::createWithSpriteFrameName("20001.png");

	spritebatch->addChild(Sprite1);
	addChild(spritebatch);

	Vector<SpriteFrame*> animFrames(48);
	char str[100] = {0};
	for(int i = 1; i < 7; i++) 
	{
		//sprintf(str, "200%02d.png", i);
		sprintf(str, "coin00%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);

	Size visiblesize = Director::getInstance()->getVisibleSize();
	Sprite1->setPosition(visiblesize.width/2,visiblesize.height/2);
	//Sprite1->setZOrder(99);
	Sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );

}

void ZoomPhotoLayer::showDeductPointEffect(std::string sLabel){

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

void ZoomPhotoLayer::enableTools(float dt){
	_bTlEnabled = true;
}
void ZoomPhotoLayer::delayWinQuit(float dt){
	auto scene = GameWin::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void ZoomPhotoLayer::delayFailQuit(float dt){
	auto scene = GameFailed::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void ZoomPhotoLayer::RemovePreviewLayer(){
	auto node = (Node*) this->getChildByTag(ZOOM_TAG_PREVIEW);
	if(node==nullptr)return;
	this->removeChildByTag(ZOOM_TAG_PREVIEW);
	_bIsPreview = false;

}

void ZoomPhotoLayer::LoadPreviewLayer(){

	do{
		auto node = CSLoader::createNode(ZOOM_UI_PREVIEW_CSB);
		CC_BREAK_IF(node == nullptr);
		node->setTag(ZOOM_TAG_PREVIEW);
		this->addChild(node,100);			//auto node = CSLoader::createNode(LINK_PRODUCT_CSB);


		std::vector<ImageView*> pImagelist;
		std::vector<Text*> pTextlist;

		auto pImage1 = dynamic_cast<ImageView *> (node->getChildByName("Product1"));
		auto txtProduct1 = dynamic_cast<Text*>(node->getChildByName("Text_1"));
		pImagelist.push_back(pImage1);
		pTextlist.push_back(txtProduct1);

		auto pImage2 = dynamic_cast<ImageView *> (node->getChildByName("Product2"));
		auto txtProduct2 = dynamic_cast<Text*>(node->getChildByName("Text_1_0"));
		pImagelist.push_back(pImage2);
		pTextlist.push_back(txtProduct2);

		auto pImage3 = dynamic_cast<ImageView *> (node->getChildByName("Product3"));
		auto txtProduct3 = dynamic_cast<Text*>(node->getChildByName("Text_1_1"));
		pImagelist.push_back(pImage3);
		pTextlist.push_back(txtProduct3);

		auto pImage4 = dynamic_cast<ImageView *> (node->getChildByName("Product4"));
		auto txtProduct4 = dynamic_cast<Text*>(node->getChildByName("Text_1_2"));
		pImagelist.push_back(pImage4);
		pTextlist.push_back(txtProduct4);

		auto pImage5 = dynamic_cast<ImageView *> (node->getChildByName("Product5"));
		auto txtProduct5 = dynamic_cast<Text*>(node->getChildByName("Text_1_0_0"));
		pImagelist.push_back(pImage5);
		pTextlist.push_back(txtProduct5);

		auto pImage6 = dynamic_cast<ImageView *> (node->getChildByName("Product6"));
		auto txtProduct6 = dynamic_cast<Text*>(node->getChildByName("Text_1_1_0"));
		pImagelist.push_back(pImage6);
		pTextlist.push_back(txtProduct6);

		for(int i = 0; i < _ttlProductlist.size(); i++)
		{
			pImagelist[i]->loadTexture(_ttlProductlist[i].image.c_str());
			pTextlist[i]->setText(_ttlProductlist[i].answer.c_str());
		}

		auto timer = dynamic_cast<Text*>(node->getChildByName("lblTimer"));
		timer->setText("00:05");
		_nPreviewSec = PREVIEW_TIME;
		_bIsPreview = true;
		//this->scheduleOnce(schedule_selector(ZoomPhotoLayer::RemovePreviewLayer), 5.5f);

	}while(0);

}