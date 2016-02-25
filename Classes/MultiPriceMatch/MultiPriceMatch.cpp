
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "AppDelegate.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "MultiPriceMatch.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <string>
#include <algorithm>
#include <random>
#include <stdio.h>
#include <stdlib.h>

#include "../Msgbox/MsgBox.h"
#include "../utilits/Tutorial.h"
#include "cocostudio/CocoStudio.h"//for animation

#include "theme/ThemeLayer.h"


using namespace std;
using namespace ui;
using namespace cocostudio;

static int _TOTAL_PRODUCTS = 3;

MultiPriceMatch::MultiPriceMatch(){};

MultiPriceMatch::~MultiPriceMatch(){};

bool MultiPriceMatch::init(){

	bool bRet = false;
	do{
		if(!Layer::init()){
			break;
		}

		int currenlevel = 0;

		//if (_gamestatus != GAME_START) {
		//	_gamestatus = GAME_START;
		//}
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		CC_BREAK_IF(!app);

		_nTlimit = 3; // total tools can be used in each game
		_nTlcount = app->getToolsNumberInfo(); // total tools user has
		_nLvl = app->GetUserLevel(); //totol tips can be used
		_fPerToNLvl = app->GetPercentToNextLevel(); //Percent to next level
		_lTtlScore = app->GetUserTotalScore();
		currenlevel = app->getCurrentGameLevel();

		_gametime = GAME_TIME_EASY;

		//_zoomin = true;
		//saveProductInfo();
		CC_BREAK_IF(!loadProductInfo());

		_SORT_UINode = CSLoader::createNode(SORT_UINODE_CSB);
		CC_BREAK_IF(_SORT_UINode == nullptr);
		_SORT_UINode->setTag(SORT_UI_NODE);
		this->addChild(_SORT_UINode);

		_vec_Price.clear();
		_vec_Price_original.clear();
		_vec_Price_index.clear();

		char strName[20] = LBL_SP_PDX;
		char strLabel[20] = IMG_PD_PRICE;
		char strBtn[20] = BTN_PD_PRICE;

		//string price, price2;

		_vec_lblPrice.clear();
		_vec_btnPrice.clear();

		std::vector <std::string> vec_price;
		std::vector <std::string> vec_Price_original;

		std::random_device rd1;
		std::mt19937 g1(rd1());
 		std::shuffle(std::begin(_product_list), std::end(_product_list), g1);


		for (int i = 0; i < 3; i++){
			std::string price = StringUtils::format("%s",_product_list[i].price.c_str());
			//log("Before: price for %d is %s",i, price.c_str());
			vec_price.push_back(price);
			vec_Price_original.push_back(price);
		}
	
		//srand(unsigned(time(NULL)));
		//std::random_shuffle(vec_price.begin(), vec_price.end());


		std::random_device rd;
		std::mt19937 g(rd());
 		std::shuffle(std::begin(vec_price), std::end(vec_price), g);

		//auto engine = std::default_random_engine();
		//std::shuffle(std::begin(vec_price), std::end(vec_price), engine);

		//for (int i = 0; i < 3; i++){
		//	log("After: price for %d is %s",i, vec_price[i].c_str());
		//}

		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				if (vec_Price_original[i].compare(vec_price[j].c_str()) == 0){
					_vec_Price_index.push_back(j);
				}
			}
		}

		for (int i = 0; i < 3; i++){
			log("%d", _vec_Price_index[i]);
		}

		for (int i = 0; i < 3; i++){
			log("for %d", i);
			strName[5] = LETTER_NUM_1 + i;
			strLabel[6] = LETTER_NUM_1 + i;
			strBtn[6] = LETTER_NUM_1 + i;
			//std::string price = StringUtils::format("%s",_product_list[i].price->getCString());

			//add Image
			auto sp = dynamic_cast<Sprite*>(_SORT_UINode->getChildByName(strName));
			CC_BREAK_IF(sp == nullptr);
			sp->setTag(PRODUCT_IMG_1 + i);
			//sp->setTexture(_product_list[i].image->getCString());
			sp->setTexture(_product_list[i].image.c_str());
			sp->setScale((sp->getBoundingBox().size.width/sp->getContentSize().width)*0.6);//resize to 300 
			sp->setZOrder(2);

			//sp->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::btnPriceCallback, this));



			//add Price Label
			auto label = dynamic_cast<Sprite*>(_SORT_UINode->getChildByName(strLabel));
			CC_BREAK_IF(label == nullptr);
			label->setTag(LBL_PRICE_1 + i);
			label->setZOrder(0);
			_vec_lblPrice.push_back(label);
			//_vec_originalPos.push_back(label->getPosition());

			//add Price Button
			auto btn = dynamic_cast<Button*>(_SORT_UINode->getChildByName(strBtn));
			CC_BREAK_IF(btn == nullptr);
			btn->setTag(BTN_PRICE_1 + i);
			btn->setTitleText(vec_price[i]);
			btn->setTitleColor(ccc3(255,248,220));
			btn->setTitleFontSize(32);
			btn->setZOrder(1);
			btn->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::btnPriceCallback, this));
			_vec_originalPos.push_back(btn->getPosition());
			_vec_btnPrice.push_back(btn);

		}

		auto lblTimer = dynamic_cast<Text*>(_SORT_UINode->getChildByName(TXT_TIMER));
		CC_BREAK_IF(lblTimer == nullptr);
		this->schedule(schedule_selector(MultiPriceMatch::updateTimeLabel), 0.01f);

		auto btnMain = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BUTTON_MAIN));
		CC_BREAK_IF(btnMain == nullptr);
		//auto btnSetting = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BUTTON_SETTING));
		//auto btnVoice = dynamic_cast<Button*>(_SORT_UINode->getChildByName("btnVoice"));
		auto btnQuit = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BUTTON_QUIT));
		CC_BREAK_IF(btnQuit == nullptr);

		if (app->iGameMode == MobileGameMode::Theme_Mode){
			//btnMain->setVisible(false);
			//btnQuit->setVisible(false);
		}
		//auto btMagnifier = dynamic_cast<Button*>(_SORT_UINode->getChildByName("btMagnifier"));
		//Add by Long for theme mode

		//Add by Long EOF

		//test button
		btnMain->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::menuKeyCallback, this));
		btnQuit->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::ShowTutorialCallback, this));


		auto btnClr = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_CLEAR));
		CC_BREAK_IF(btnClr == nullptr);
		btnClr->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::btnClear, this));
		btnClr->setTitleColor(ccc3(255,248,220));
		btnClr->setTitleFontSize(32);
		btnClr->setTitleText("CLEAR");
		btnClr->setVisible(false);

		auto txtClear = dynamic_cast<Text*>(_SORT_UINode->getChildByName("clear"));
		txtClear->setVisible(false);
		//btnClr->addChild(txtClear);

		auto btnSub = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_SUBMIT));
		CC_BREAK_IF(btnSub == nullptr);
		btnSub->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::btnSubmit, this));
		btnSub->setTitleColor(ccc3(255,248,220));
		btnSub->setTitleFontSize(32);
		btnSub->setTitleText("SUBMIT");
		btnSub->setVisible(false);

		auto txtsubmit = dynamic_cast<Text*>(_SORT_UINode->getChildByName("submit"));
		txtsubmit->setVisible(false);

		auto txtPrice1 = dynamic_cast<Text*>(_SORT_UINode->getChildByName("price_1"));
		auto txtPrice2 = dynamic_cast<Text*>(_SORT_UINode->getChildByName("price_2"));
		auto txtPrice3 = dynamic_cast<Text*>(_SORT_UINode->getChildByName("price_3"));
		txtPrice1->setVisible(false);
		txtPrice2->setVisible(false);
		txtPrice3->setVisible(false);

		auto lvlbar = dynamic_cast<Sprite*> (_SORT_UINode->getChildByName(LINK_UI_LEVELBAR));
		lvlbar->setAnchorPoint(Vec2(0,0.5));
		//lvlbar->setPositionX(lvlbar->getPositionX()-lvlbar->getContentSize().width/2);
		lvlbar->setScaleX(_fPerToNLvl);

		auto btnTips = dynamic_cast<Button*>(_SORT_UINode->getChildByName(LINK_UI_BTN_TIPS));
		CC_BREAK_IF(btnTips == nullptr);
		btnTips->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::ShowHintCallback, this));


		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = CC_CALLBACK_2(MultiPriceMatch::onTouchBegan, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(MultiPriceMatch::onTouchEnded, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(MultiPriceMatch::onTouchMoved, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(MultiPriceMatch::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		//AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->iGameMode == MobileGameMode::Theme_Mode){
			btnMain->setVisible(false);
			//btnSetting->setVisible(false);
			//btnVoice->setVisible(false);
			btnQuit->setVisible(false);
		}

		if ( app->getBGMstatus() == BGMusic_flag::ON){
			app->StartBGMusic(BGM_GAME);
		}

		//UserDefault *load = UserDefault::getInstance();
		//string sFirstFlip = load->getStringForKey("FIRSTTIME_LINK","0");
		//{
		//	if (atoi(sFirstFlip.c_str()) == 0)
		//	{
		//		scheduleOnce(schedule_selector(MultiPriceMatch::loadFirstTutorial), 1.1f);
		//		load->setStringForKey("FIRSTTIME_LINK", "1");
		//	}
		//}
		_bTlEnabled = true;

		//-----------------------------------add animation----------------------------------------------------------

		ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMATION_FILE);
		Armature *armature = Armature::create(ARMATURE);
		CC_BREAK_IF(armature == nullptr);
		armature->setAnchorPoint(Vec2(0.5, 0.5));
		armature->setVisible(true);
		armature->setPosition(Vec2(235, 480));
		armature->setTag(888);
		auto imgBackFrame = dynamic_cast<Sprite*> (_SORT_UINode->getChildByName(BOARD));
		CC_BREAK_IF(imgBackFrame == nullptr);
		imgBackFrame->setZOrder(1);
		schedule(schedule_selector(MultiPriceMatch::PlayAnimation), 5.0f);
		_SORT_UINode->addChild(armature);


		//-------------------------------------------------------------------------------------
		bRet = true;
	}while(0);

	return bRet;
}

//void MultiPriceMatch::onTouchEnded(Touch* touch, Event* event)
//{
//	cocos2d::log("touch ended");
//
//
//	return;
//
//}

void MultiPriceMatch::PlayAnimation(float dt)
{
	Armature *armature = dynamic_cast<Armature *>(_SORT_UINode->getChildByTag(888));
	armature->getAnimation()->playWithIndex(0, -1, 0);//from the first frame, no loop
	armature->getAnimation()->setSpeedScale(0.8);
	AppDelegate* app = (AppDelegate*)Application::getInstance();
	if (app->getBGMstatus() == BGMusic_flag::ON)
	{
		app->PlayEffect(SOUND_ANIMATION);
	}
}
void MultiPriceMatch::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event){
	cocos2d::log("onTouchesBegan");
}

void MultiPriceMatch::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event){
	cocos2d::log("onTouchesMoved");
	return;
}

void MultiPriceMatch::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event){
	cocos2d::log("onTouchesEnded");

}

void MultiPriceMatch::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event){
	cocos2d::log("onTouchesCancelled");
	_isResizing = false;
}

void MultiPriceMatch::menuStartCallback(Object *sender){
}

void MultiPriceMatch::menuKeyCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){

	AppDelegate *app = (AppDelegate*)Application::getInstance();


	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			if (app->getBGMstatus() == BGMusic_flag::ON){
				app->PlayEffect(SOUND_BUTTON_CLICK);
			}	
			MultiPriceMatch::gameover(); //quit the game
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
void MultiPriceMatch::ShowTutorialCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}		

		auto scene = TutorialScene::create();
		if(scene != nullptr){
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->pushScene(transition);
		}

	}
}
void MultiPriceMatch::ShowHintCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	//if (eEventType != ui::Widget::TouchEventType::BEGAN)

	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			MultiPriceMatch::showHint(); //quit the game
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
};

void MultiPriceMatch::ShowProductCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	//if (eEventType != ui::Widget::TouchEventType::BEGAN)

	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("touch Began");
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("touch Ended");
			MultiPriceMatch::showHint(); //quit the game
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
};

void MultiPriceMatch::btnBack(Ref* pSender, ui::Widget::TouchEventType eEventType){

	//if (eEventType != ui::Widget::TouchEventType::BEGAN)

	//switch (eEventType) {
	//	case ui::Widget::TouchEventType::BEGAN:
	//		cocos2d::log("touch Began");
	//		break;
	//	case ui::Widget::TouchEventType::ENDED:
	//		cocos2d::log("touch Ended");
	//		MultiPriceMatch::gameover(); //quit the game
	//		break;
	//	case ui::Widget::TouchEventType::MOVED:
	//		cocos2d::log("touch Cancel");
	//		break;
	//	case ui::Widget::TouchEventType::CANCELED:
	//		cocos2d::log("touch Cancel");
	//		break;
	//	default:
	//		cocos2d::log("touch Default");
	//		break;
	//}
	//return;

	}
void MultiPriceMatch::btnPriceCallback(Ref* pSender, ui::Widget::TouchEventType eEventType){
	/*if (eEventType != ui::Widget::TouchEventType::BEGAN)*/

	CCNode* node = (CCNode*) pSender;
	int tag = node->getTag();
	bool bSel = false;

	for (int i = 0; i < 3;i++){
		if (tag == (BTN_PRICE_1 + i)) bSel = true;
	}

	if (bSel == false) return;

	cocos2d::log("button's tag is %d",tag);
	auto button = (Button*) _SORT_UINode->getChildByTag(tag);
	AppDelegate *app = (AppDelegate*)Application::getInstance();


	switch (eEventType) {
		case ui::Widget::TouchEventType::BEGAN:
			cocos2d::log("btnPriceCallback Began");
			button->getTouchBeganPosition();
			button->setZOrder(99);
			break;
		case ui::Widget::TouchEventType::ENDED:
			cocos2d::log("btnPriceCallback Ended");
			if (app->getBGMstatus() == BGMusic_flag::ON){
				app->PlayEffect(SOUND_BUTTON_CLICK);

			}	
			button->setZOrder(1);
			MovetoPriceLabel(button);
			break;
		case ui::Widget::TouchEventType::MOVED:
			button->setPosition(button->getTouchMovePosition());
			cocos2d::log("btnPriceCallback Moving");
			break;
		case ui::Widget::TouchEventType::CANCELED:
			cocos2d::log("touch Cancel");
			button->setZOrder(1);
			break;
		default:
			cocos2d::log("touch Default");
			break;
	}
	return;

}
void MultiPriceMatch::MovetoPriceLabel(Button* button){
	
	bool bSet = false;
	bool allset = true;
	string strtaken = LBL_TAKEN;

	do{
		std::vector<Sprite*>::const_iterator v;
		for(v =_vec_lblPrice.begin(); v!=_vec_lblPrice.end(); ++v){
			if ((*v)->getBoundingBox().containsPoint(button->getTouchEndPosition())){
				if ((*v)->getName() != strtaken){
					button->setPosition((*v)->getPosition());
					button->setBright(false);
					button->setTouchEnabled(false);
					bSet = true;
					(*v)->setName(LBL_TAKEN);
				}else{
					//bSet = false;
					//button->setPosition(button->getTouchStartPos());
					//button->setPosition(button->getTouchStartPos());
					//(*v)->setName("");
					//bSet = true;
					break;
				}
			} else if ((*v)->getBoundingBox().containsPoint(button->getTouchBeganPosition())){
				(*v)->setName("");
			}
		}
	
		if (bSet == false) {
			switch (button->getTag()) {
			case BTN_PRICE_1:
				button->setPosition(_vec_originalPos[0]);
				break;
			case BTN_PRICE_2:
				button->setPosition(_vec_originalPos.at(1));
				break;
			case BTN_PRICE_3:
				button->setPosition(_vec_originalPos.at(2));
				break;
			default:
				//button->setPosition(button->getTouchBeganPosition());
				break;
			}
		}

		for(int i = 0; i < 3; i++){
			if (this->getChildByTag(SORT_UI_NODE)->getChildByTag(BTN_PRICE_1+i)->getPosition() == _vec_originalPos[i]){
				allset = false;
			}
		}

		if (allset == true){
			auto btnClr = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_CLEAR));
			CC_BREAK_IF(btnClr == nullptr);
			btnClr->setVisible(true);

			auto btnSub = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_SUBMIT));
			CC_BREAK_IF(btnSub == nullptr);
			btnSub->setVisible(true);		
		}else{
			auto btnClr = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_CLEAR));
			CC_BREAK_IF(btnClr == nullptr);
			btnClr->setVisible(false);

			auto btnSub = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_SUBMIT));
			CC_BREAK_IF(btnSub == nullptr);
			btnSub->setVisible(false);		
		}

	}while(0);

	return;
}

void MultiPriceMatch::btnClear(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type != cocos2d::ui::Widget::TouchEventType::ENDED) return;
	do{
		//readProductInfo();
		for(int i = 0; i < 3; i++){
			auto button = (Button*) this->getChildByTag(SORT_UI_NODE)->getChildByTag(BTN_PRICE_1+i);
			button->setPosition(_vec_originalPos[i]);
			button->setTouchEnabled(true);
			button->setBright(true);
		}

		std::vector<Sprite*>::const_iterator v;
		for(v =_vec_lblPrice.begin(); v!=_vec_lblPrice.end(); ++v){
				(*v)->setName("");
		}

		auto btnClr = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_CLEAR));
		CC_BREAK_IF(btnClr == nullptr);
		btnClr->setVisible(false);

		auto btnSub = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_SUBMIT));
		CC_BREAK_IF(btnSub == nullptr);
		btnSub->setVisible(false);		
	}while(0);
	return;
}

void MultiPriceMatch::btnSubmit(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		if (gameresult() == true) {
			gamewin();
		} else {
			gamelose();
		}
	}
}

void MultiPriceMatch::btnZoomInCallback(Object *sender){

	return;
	}

void MultiPriceMatch::btnZoomOutCallback(Object *sender){

	return;
	}

bool MultiPriceMatch::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch Began");
	_sel = -1;

	Vector<Button*>::const_iterator v;
	for(v = _vec_btnPrice.begin(); v!= _vec_btnPrice.end(); ++v){
	  //cocos2d::log("%f %f",(*v).x,(*v).y);
		if((*v)->getBoundingBox().containsPoint(touch->getLocation())){
			_sel = (*v)->getTag();
			_originalPt = (*v)->getPosition();
		}

	}

	cocos2d::log("button selected is %d", _sel);

	return true;
}

void MultiPriceMatch::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");
	_drop = -1;

	do{

		
		auto prod = (Sprite*)this->getChildByTag(SORT_UI_PRODUCTPAGE);
		if (prod != nullptr){
			return;
		}


		auto location1 = touch->getLocation();
		auto location2 = touch->getStartLocation();
		auto distance = location1.distance(location2);
	
		auto touchtip = touch->getLocation();
		auto spTips = dynamic_cast<Button*>(_SORT_UINode->getChildByName(LINK_UI_BTN_TIPS));
		CC_BREAK_IF(spTips == nullptr);
		auto bound = spTips->getBoundingBox();
		

		
		auto img1_bound = _SORT_UINode->getChildByTag(PRODUCT_IMG_1)->getBoundingBox();
		auto img2_bound = _SORT_UINode->getChildByTag(PRODUCT_IMG_2)->getBoundingBox();
		auto img3_bound = _SORT_UINode->getChildByTag(PRODUCT_IMG_3)->getBoundingBox();
		
		if (distance > 3) {
			break;
		}
		if (bound.containsPoint(touchtip)){
			MultiPriceMatch::showHint();
			log("this is tips");
		} else {

			std::string strName;
			std::string strImage;
			std::string strDesp;

			if (img1_bound.containsPoint(touchtip)){
				strName = _product_list[0].name.c_str();
				strImage = _product_list[0].image.c_str();
				strDesp = _product_list[0].description.c_str();
			}else if(img2_bound.containsPoint(touchtip)){
				strName = _product_list[1].name.c_str();
				strImage = _product_list[1].image.c_str();
				strDesp = _product_list[1].description.c_str();
			}else if(img3_bound.containsPoint(touchtip)){
				strName = _product_list[2].name.c_str();
				strImage = _product_list[2].image.c_str();
				strDesp = _product_list[2].description.c_str();
			}else{
				break;
			}

			loadProductPage(strName,strImage,strDesp);
			
		}
	}while(0);

	return;

}

bool MultiPriceMatch::loadProductPage(string sName,string sImg,string sDes){
	bool bRet = false;
	do{
		auto node = CSLoader::createNode(LINK_PRODUCT_CSB);
		CC_BREAK_IF(node == nullptr);
		node->setTag(SORT_UI_PRODUCTPAGE);
		this->addChild(node);

		auto btnPDQuit = dynamic_cast<Button*>(node->getChildByName(LINK_PRODUCT_BTN));
		CC_BREAK_IF(btnPDQuit == nullptr);
		btnPDQuit->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::PDQuitCallback, this));

		auto pdImage = dynamic_cast<ImageView*>(node->getChildByName(LINK_PRODUCT_IMG));
		CC_BREAK_IF(pdImage == nullptr);
		pdImage->loadTexture(sImg.c_str());

		auto pdName = dynamic_cast<Text*>(node->getChildByName(LINK_PRODUCT_NAME));
		CC_BREAK_IF(pdName == nullptr);
		pdName->setString(sName.c_str());

		auto pdDesp = dynamic_cast<Text*>(node->getChildByName(LINK_PRODUCT_DESCRIPTION));
		CC_BREAK_IF(pdDesp == nullptr);
		pdDesp->setString(sDes.c_str());

		bRet = true;

	}while(0);
	return bRet;

}

void MultiPriceMatch::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
	cocos2d::log("button selected is %d", _sel);
	if (_sel == -1) return;

	auto sp = dynamic_cast<Sprite*> (_SORT_UINode->getChildByTag(_sel));
	if(sp == nullptr) return;
	sp->setZOrder(99);
	sp->setPosition(touch->getLocation());

}

void MultiPriceMatch::onTouchCancelled(Touch* touch, Event* event)
{
   cocos2d::log("touch cancelled");
   onTouchEnded(touch, event);
}

void MultiPriceMatch::updateTimeLabel(float dt) {
	_gametime -= 1;
	int ttlseconds = _gametime / 60;
	int seconds = ttlseconds % 60;
	int min = ttlseconds / 60;
	char lblTimer[100];
	sprintf(lblTimer,"%d:%02d", min, seconds);
	string string1 = lblTimer;
	auto timerLabel = dynamic_cast<Text*> (_SORT_UINode->getChildByName(TXT_TIMER));
	timerLabel->setFontSize(20);
	timerLabel->setString(string1);

	if (_gametime < 10 * 60) {
			timerLabel->setColor(ccc3(255,0,0));
	}

	char lblTips[10];
	sprintf(lblTips,"%d", _nTlcount);
	string string2 = lblTips;
	auto tipsLabel = dynamic_cast<Text*> (_SORT_UINode->getChildByName(LINK_UI_TEXT_TIPS));
	tipsLabel->setString(string2);

	char lblLvl[10];
	sprintf(lblLvl,"%d", _nLvl);
	string string3 = lblLvl;
	auto lvlLabel = dynamic_cast<Text*> (_SORT_UINode->getChildByName(LINK_UI_TEXT_LEVELBAR));
	lvlLabel->setString(string3);

	//auto lvlbar = dynamic_cast<Sprite*> (_SORT_UINode->getChildByName("lv_bar_4"));
	//lvlbar->setAnchorPoint(Vec2(0,0.5));
	//lvlbar->setScaleX(_fPerToNLvl);

	char lblScore[20];
	sprintf(lblScore,"%lu", _lTtlScore);
	string string4 = lblScore;
	auto scoreLabel = dynamic_cast<Text*> (_SORT_UINode->getChildByName(LINK_UI_TEXT_SCORE));
	scoreLabel->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	scoreLabel->setString(string4);

	updateprogress();

}

void MultiPriceMatch::updateprogress(){

	if (_gametime > 0) {
		return;
	}
	else {
		if (gameresult() == true) {
			gamewin();
		} else {
			gamelose();
		}
	}
	return;
}

bool MultiPriceMatch::gameresult(){

	bool bRet = true;

	do{
		Vector<Sprite*>::const_iterator vlbl;
		Vector<Button*>::const_iterator vbtn;
		for(int j = 0;j < 3; j++) {
			if (_vec_btnPrice[j]->getPosition() == _vec_originalPos[j]){
				bRet = false;
				break;
			}

		}

		int i = 0;
		for(vlbl =_vec_lblPrice.begin(); vlbl!=_vec_lblPrice.end(); ++vlbl){
			for(vbtn =_vec_btnPrice.begin(); vbtn!=_vec_btnPrice.end(); ++vbtn){
				if ((*vbtn)->getPosition() == (*vlbl)->getPosition()){
					if ((*vbtn)->getTitleText().compare(_product_list[i].price.c_str()) != 0){
						bRet = false;
					}
				}
			}
			log("return value is %d", bRet);
			i++;
		}
		//bRet = true;

	}while(0);

	return bRet;

}

void MultiPriceMatch::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}		

		_vec_circle.clear();
		_vec_circle2.clear();
		this->unschedule(schedule_selector(MultiPriceMatch::updateTimeLabel));
		//this->unschedule(schedule_selector(FindDiffLayer::updateDragMove));

		//AppDelegate *app = (AppDelegate*)Application::getInstance();
//		app->SaveGamePassLevel();
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

void MultiPriceMatch::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
			
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}		
		
		//Don't quit the game
		this->removeChildByTag(9000);
		//enable the touch event
		//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(FindDiffLayer::resetTouchEvent, this));
		//this->runAction(Sequence::create(DelayTime::create(1.0f), runCallback, nullptr));
        CCLOG("touch ended.");
    }
}

void MultiPriceMatch::PDQuitCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){
		//Quit the game
		this->removeChildByTag(SORT_UI_PRODUCTPAGE);
        CCLOG("touch ended.");
    }
}


void MultiPriceMatch::gameover(){

	MsgBoxType m_type = TP_Q_YESNO;
	std::string title = MSG_QUIT_TITLE;
	std::string msg = MSG_QUIT_MSG;
	MsgBox::setMsgBox(m_type,&title,&msg);
	auto msgbox = MsgBox::create();
	msgbox->setTag(9000);
	this->addChild(msgbox,999);

	auto btnYes = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
	auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
	btnYes->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::yesCallback, this));
	btnNo->addTouchEventListener(CC_CALLBACK_2(MultiPriceMatch::noCallback, this));

	//this->unschedule(schedule_selector(MultiPriceMatch::updateTimeLabel));

	//auto scene = WelcomeScene::create();
	//if(scene != nullptr){
	//	TransitionScene *transition = TransitionFade::create(1, scene);
	//	Director::getInstance()->replaceScene(transition);
	//}
}

void MultiPriceMatch::gamestart(){
	//_gamestatus = GAME_START;
}

void MultiPriceMatch::gamepause(){

}

void MultiPriceMatch::gamewin(){

	long curscore = m_iUserTotalScore;
	AppDelegate *app = (AppDelegate*)Application::getInstance();
	this->unschedule(schedule_selector(MultiPriceMatch::updateTimeLabel));

	int score = 0;
	int ttlseconds = _gametime / 60;

	//std::string strSKU = "1400039210";
	//app->setPromoteProduct("");
	//if(app->getGameLevelPassedInfo() == 0){
	//	if (ttlseconds > 60 && ttlseconds <= 90){
	//		score = WIN_POINT_HIG;
	//	}else if(ttlseconds > 30 && ttlseconds <= 60){
	//		score = WIN_POINT_MED;	
	//	}else{rea
	//		score = WIN_POINT_LOW;
	//	}
	//}else{
	//	score = WIN_POINT_LOW;		
	//}

	//_gamestatus = GAME_OVER;
	_vec_circle.clear();
	_vec_circle2.clear();
	

    
//	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if(app->iGameMode == MobileGameMode::Theme_Mode){
		app->GameRunRoundNext();
	}else{
		//app->UpdateUserScore(1,score);
		//app->setReward(score);
		app->updateGameWinScore(ttlseconds);
		app->SaveGamePassLevel();//Add by Long to Save Level
		app->moveToNextGameLevel();
		this->scheduleOnce(schedule_selector(MultiPriceMatch::delayWinQuit),1.0f);
		//auto scene = GameWin::createScene();
		//if(scene != nullptr){
		//	TransitionScene *transition = TransitionFade::create(1, scene);
		//	Director::getInstance()->replaceScene(transition);
		//}
	}
}

void MultiPriceMatch::gamelose(){
	_vec_circle.clear();
	_vec_circle2.clear();
	//_gamestatus = GAME_OVER;

	this->unschedule(schedule_selector(MultiPriceMatch::updateTimeLabel));

	AppDelegate *app = (AppDelegate*)Application::getInstance();
//	app->SaveGamePassLevel();//Add by Long to Save Level
	//std::string strSKU = "1400039210";
	//app->setPromoteProduct("");

	app->moveToNextGameLevel();
	this->scheduleOnce(schedule_selector(MultiPriceMatch::delayFailQuit),1.0f);
	//auto scene = GameFailed::createScene();
	//if(scene != nullptr){
	//	TransitionScene *transition = TransitionFade::create(1, scene);
	//	Director::getInstance()->replaceScene(transition);
	//}
}

void MultiPriceMatch::test(){

	//gameresult();
	//writeFile(LEVEL_INFO_FILE);
	//loadLevelInfo();
}

void MultiPriceMatch::saveLevelInfo(){
	
	CCDictionary * gameinfo = new CCDictionary();
	
	// Current player's level
	CCString userData ="2";
	gameinfo->setObject(&userData ,"current_level");
 
	// Save level 1 player data
	userData ="45000";
	gameinfo->setObject(&userData ,"level01_points");

	userData ="120"; // in seconds
	gameinfo->setObject(&userData ,"level01_time");
 
	// Game configuration
	userData ="off";
	gameinfo->setObject(&userData ,"game_sound");

	userData ="medium";
	gameinfo->setObject(&userData ,"game_diff");
	
	//write to file
	gameinfo->writeToFile(SORT_LEVEL_INFO);

	//AppDelegate* app = (AppDelegate*) Application::getInstance();
	//app->SaveGamePassLevel();

}
bool MultiPriceMatch::readFile(const char* fileName,const char* levelID) {

	bool bRet = false;	
	string id = levelID;
	do{
		CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(fileName));

		ValueMap root = FileUtils::getInstance()->getValueMapFromFile(fileName);
		CC_BREAK_IF(root.empty());
		
		ValueVector pl = root["PRODUCT_LIST"].asValueVector();
		CC_BREAK_IF(pl.empty());
		int nSize = pl.size();
		std::vector<int> levelNum_all;
		std::vector<int> levelNum;

		levelNum_all.clear();
		levelNum.clear();

		for(int i = 0; i < nSize; i++){
			levelNum_all.push_back(i);
		}

		std::random_device rd1;
		std::mt19937 g1(rd1());
 		std::shuffle(std::begin(levelNum_all), std::end(levelNum_all), g1);

		for(int n=0; n< _TOTAL_PRODUCTS; ++n){
			levelNum.push_back(levelNum_all[n]);// generate numbers
		}

		for (int i = 0; i < levelNum.size(); i++){
			ValueMap pd = pl[levelNum[i]].asValueMap();
			PRODUCT_INFO p;
			p.id = pd[PD_ID].asString().c_str();
			p.name = pd[PD_NAME].asString().c_str();
			p.image = pd[PD_IMAGE].asString().c_str();
			p.price = pd[PD_PRICE].asString().c_str();
			p.description = pd[PD_DESCRIPTION].asString().c_str();
			p.sku = pd[PD_SKU].asString().c_str();
			_product_list.push_back(p);
		}

	bRet = true;
	}while(0);

	return bRet;
}

bool MultiPriceMatch::writeFile(const char* fileName) {
	return true;
}

bool MultiPriceMatch::loadProductInfo(){
	
	bool bRet = false;
	_product_list.clear();

	do{
		int currenlevel = 0;
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		UserDefault *save = UserDefault::getInstance();
		//std::string str = SORT_LEVEL_INFO;
		std::string str = SORT_PRODUCT_INFO;
		std::string id;
		//char lvl[10];
		//currenlevel = app->getCurrentGameLevel();
		//snprintf(lvl, 10, "%d", currenlevel);
		////nExisted = std::atol(str.c_str());
		//int indexlvl = currenlevel - 1;
		if(0){
			//CC_BREAK_IF(!readFile(str.c_str(),lvl));
		}else{
			std::vector<_ProductInfo>*	pPrdInfoList;
			pPrdInfoList = DataAccess::getProductListbyGame(m_iCurrentGameID);
			CC_BREAK_IF(pPrdInfoList == nullptr);

			int nSize = pPrdInfoList->size();
			std::vector<int> levelNum_all;
			std::vector<int> levelNum;
			levelNum_all.clear();
			levelNum.clear();

			for(int i = 0; i < nSize; i++){
				levelNum_all.push_back(i);
			}

			std::random_device rd1;
			std::mt19937 g1(rd1());
 			std::shuffle(std::begin(levelNum_all), std::end(levelNum_all), g1);

			for(int n=0; n < 3; ++n){
				levelNum.push_back(levelNum_all[n]);// generate numbers
			}

			for (int i = 0; i < levelNum.size(); i++){
				PRODUCT_INFO p;
				p.id = pPrdInfoList->at(levelNum.at(i)).ID;
				p.name = pPrdInfoList->at(levelNum.at(i)).NAME;
				p.image = pPrdInfoList->at(levelNum.at(i)).IMAGE;
				p.price = pPrdInfoList->at(levelNum.at(i)).PRICE;
				p.description = pPrdInfoList->at(levelNum.at(i)).DESCRIPTION;
				p.sku = pPrdInfoList->at(levelNum.at(i)).SKU;
				_product_list.push_back(p);
			}		
		}

		app->setPromoteProduct(_product_list[0].sku);
		//app->setPromoteProduct("");

		bRet = true;
	}while(0);

	return bRet;
}
bool MultiPriceMatch::showHint(){

	bool bRet = false;
	bool allset = true;
	Vec2 vecbtn;
	
	do{
		if(_bTlEnabled == false) break;
		_bTlEnabled = false;

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		//long score = app->GetUserTotalScore();
		int score = 0;

		if (_nTlimit > 0){
			if(_nTlcount > 0){
				_nTlcount--;
				//AppDelegate *app =  (AppDelegate*) Application::getInstance();
				app->setToolsNumberInfo(_nTlcount);
			}else{
				if(_lTtlScore >= 10){
					showDeductPointEffect(LINK_UI_TEXT_SCORE);
					score = 10;
					_lTtlScore = _lTtlScore - 10;
					AppDelegate *app =  (AppDelegate*) Application::getInstance();
					app->UpdateUserScore(0,10);
				}else{
					break;
				}
			}
			
			app->PlayEffect(SOUND_TOOL_CLICK);
			_nTlimit--;
			if (_nTlimit == 0){
				auto spTips = dynamic_cast<Button*>(_SORT_UINode->getChildByName(LINK_UI_BTN_TIPS));
				CC_BREAK_IF(!spTips);
				spTips->setEnabled(false);
				spTips->setColor(ccc3(84,84,84));//not disabled button, so use color instead
			}

		}else {
			break;
		}

		//if (score < 10) {
		//	MessageBox("Oops, Your score should be greater than 10 to use powerup","Spot it");
		//	break;
		//}
		//_vec_currentPos.clear();

		//for (int i = 0; i < 3; i++){
		//	_vec_currentPos.push_back(_vec_btnPrice[i]->getPosition());
		//}

		//btnClear();
		//int i,j = 0;
		//_vec_Price_index
		for (int i = 0; i < 3; i++){ 	
			for (int j = 0; j < 3; j++){
				vecbtn = _vec_btnPrice[_vec_Price_index[i]]->getPosition();//get correct button position
				if (_vec_lblPrice[i]->getName().compare(LBL_TAKEN) == 0){
					if (_vec_lblPrice[i]->getPosition() == _vec_btnPrice[j]->getPosition()){
						if (_vec_btnPrice[j]->getTitleText().compare(_product_list[i].price.c_str()) == 0){ //price is correct 
							j = 3;
							break;
						}else{//price is wrong
//							vecbtn = _vec_btnPrice[_vec_Price_index[i]]->getPosition();//get correct button position
							_vec_btnPrice[_vec_Price_index[i]]->setPosition(_vec_lblPrice[i]->getPosition()); // move correct button to the position
							_vec_btnPrice[_vec_Price_index[i]]->setBright(false);
							_vec_btnPrice[_vec_Price_index[i]]->setTouchEnabled(false);

							_vec_btnPrice[j]->setPosition(_vec_originalPos[j]); //set wrong button back to original place
							_vec_btnPrice[j]->setTouchEnabled(true);
							_vec_btnPrice[j]->setBright(true);

							_vec_lblPrice[i]->setName(LBL_TAKEN); //set new position is taken
							j = 3;
							i = 3;
							break;
						}
					}
				}else{
					_vec_btnPrice[_vec_Price_index[i]]->setPosition(_vec_lblPrice[i]->getPosition()) ;
					_vec_btnPrice[_vec_Price_index[i]]->setBright(false);
					_vec_btnPrice[_vec_Price_index[i]]->setTouchEnabled(false);
					//_vec_btnPrice[j]->setPosition(_vec_lblPrice[i]->getPosition()) ;
					_vec_lblPrice[i]->setName(LBL_TAKEN);
					j = 3;
					i = 3;
					break;
				}
			}
		}
		for (int i = 0; i < 3; i++){
			if (_vec_lblPrice[i]->getPosition() == vecbtn){
				_vec_lblPrice[i]->setName("");
			}
		}


		for(int i = 0; i < 3; i++){
			if (this->getChildByTag(SORT_UI_NODE)->getChildByTag(BTN_PRICE_1+i)->getPosition() == _vec_originalPos[i]){
				allset = false;
			}
		}

		if (allset == true){
			auto btnClr = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_CLEAR));
			CC_BREAK_IF(btnClr == nullptr);
			btnClr->setVisible(true);

			auto btnSub = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_SUBMIT));
			CC_BREAK_IF(btnSub == nullptr);
			btnSub->setVisible(true);		
		}else{
			auto btnClr = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_CLEAR));
			CC_BREAK_IF(btnClr == nullptr);
			btnClr->setVisible(false);

			auto btnSub = dynamic_cast<Button*>(_SORT_UINode->getChildByName(BTN_SUBMIT));
			CC_BREAK_IF(btnSub == nullptr);
			btnSub->setVisible(false);		
		}
		//i = 0;
		//for (int i = 0; i < 3; i++){
		//	if (_vec_btnPrice[0]->getTitleText().compare(_product_list[i].price.getCString()) == 0){
		//			_vec_btnPrice[0]->setPosition(_vec_lblPrice[i]->getPosition()) ;
		//			break;
		//	}
		//	i++;
		//}
		this->scheduleOnce(schedule_selector(MultiPriceMatch::enableTools), 2.0f);

		bRet = true;
	}while(0);

	return bRet;
}

void MultiPriceMatch::loadFirstTutorial(float dt)
{
	//unschedule(schedule_selector(MultiPriceMatch::loadFirstTutorial));
	auto scene = TutorialScene::create();
	if (scene != nullptr){
		Director::getInstance()->pushScene(scene);
	}
	return;
}

void MultiPriceMatch::saveProductInfo(){

	//string pFileName = LEVEL_INFO_FILE;
	//CCDictionary* productinfo = new CCDictionary();
	//CCDictionary* products = CCDictionary::create();

	//productinfo->setObject(CCString::create("Drag Price"),"game_name");
	//productinfo->setObject(CCString::create("3"),"product number");

	//CCDictionary* pd1 = CCDictionary::create();
	//pd1->setObject(CCString::create("1"), "product id");
	//pd1->setObject(CCString::create("Mrs. Stick -Twiggy"), "product name");
	//pd1->setObject(CCString::create("price_the_pic/Mrs. Stick -Twiggy.jpg"), "image name");
	//pd1->setObject(CCString::create("$12.99"), "price");

	//CCDictionary* pd2 = CCDictionary::create();
	//pd2->setObject(CCString::create("2"), "product id");
	//pd2->setObject(CCString::create("PowerPot V"), "product name");
	//pd2->setObject(CCString::create("price_the_pic/PowerPot V.jpg"), "image name");
	//pd2->setObject(CCString::create("$84.95"), "price");

	//CCDictionary* pd3 = CCDictionary::create();
	//pd3->setObject(CCString::create("3"), "product id");
	//pd3->setObject(CCString::create("Bamboo Bluetooth Keyboard"), "product name");
	//pd3->setObject(CCString::create("price_the_pic/Bamboo Bluetooth Keyboard.jpg"), "image name");
	//pd3->setObject(CCString::create("$59.95"), "price");

	//productinfo->setObject(CCArray::create(pd1, pd2, pd3,NULL), "product list");	

	//productinfo->writeToFile(LEVEL_INFO_FILE);
}
void MultiPriceMatch::showDeductPointEffect(std::string sLabel){
	auto scoreLabel = dynamic_cast<Text*> (_SORT_UINode->getChildByName(sLabel.c_str()));
	if(scoreLabel == nullptr) return;

	//show score effect
	auto blink2 = Blink::create(1.5,3);
	auto tintto = TintTo::create(0.01f, 255.0f, 0.0f, 0.0f);
	auto tintback = TintTo::create(0.01f, 255.0f, 255.0f, 255.0f);
	auto seq1 = Sequence::create(tintto,blink2,tintback, nullptr);
	scoreLabel->runAction(seq1);
	return;
}

void MultiPriceMatch::enableTools(float dt){
	_bTlEnabled = true;
}
void MultiPriceMatch::delayWinQuit(float dt){
	auto scene = GameWin::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void MultiPriceMatch::delayFailQuit(float dt){
	auto scene = GameFailed::createScene();
	if(scene != nullptr){
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

//void MultiPriceMatch::onEnter(){
//	log("onEnter()");
//}
//
//void MultiPriceMatch::onEnterTransitionDidFinish(){
//	log("onEnterTransitionDidFinish()");
//}

//void MultiPriceMatch::showCurrency(double dv)
//{
//	int width = 14;
//	const string radix = ".";
//	const string thousands = ",";
//	const string unit = "$";
//	unsigned long v = (unsigned long) ((dv * 100.0) + .5);
//	string fmt,digit;
//	int i = -2;
//	do {
//		if(i == 0) {
//			fmt = radix + fmt;
//		}
//		if((i > 0) && (!(i % 3))) {
//			fmt = thousands + fmt;
//		}
//		digit = (v % 10) + '0';
//		fmt = digit + fmt;
//		v /= 10;
//		i++;
//	}
//	while((v) || (i < 1));
//	cout << unit << setw(width) << fmt.c_str() << endl;
//}
