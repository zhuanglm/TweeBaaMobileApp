#include "SettingScene.h"
#include "../HelloWorldScene.h"
#include "ChooseLayer.h"
#include "../Collage/CollageScene.h"
#include "../utilits/NetworkUtil.h"
#include "AppDelegate.h"
#include "global_const.h"
#include "../Msgbox/MsgBox.h"

#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../ZhuanPanSystem/TurntableSystem.h"

//#include "befool/SimpleVideoEngine.h"

SettingScene::SettingScene(){};

SettingScene::~SettingScene(){};

bool SettingScene::init(){

	bool bRet = false;
	do{
		//_ttl_time = 0;
		CC_BREAK_IF(!Scene::init());
		 //befool::SimpleVideoEngine::getShared()->playVideo("renai-circulation.mp4");
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		CC_BREAK_IF(!FileUtils::getInstance()->isFileExist(SETTING_UI_CSB));

		Node* _UINode = CSLoader::createNode(SETTING_UI_CSB);
		_UINode->setTag(1000);
		this->addChild(_UINode);

		auto btnClose= dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_CLOSE));
		auto btnSoundON= dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_ON));
		auto btnSoundOFF= dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_OFF));
		auto btnMoreInfo = dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_MORE));
		auto btnTeam = dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_TEAM));
		auto btnTerm = dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_TERM));
		auto btnClrCache = dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_CLEARCACHE));
		Sprite* imgBackground = Sprite::create(SETTING_BACKGROUND);
		Text* txtVerion = Text::create();

		CC_BREAK_IF(btnClose == nullptr);
		CC_BREAK_IF(btnSoundON == nullptr);
		CC_BREAK_IF(btnSoundOFF == nullptr);
		CC_BREAK_IF(btnMoreInfo == nullptr);
		CC_BREAK_IF(btnTeam == nullptr);
		CC_BREAK_IF(btnTerm == nullptr);
		CC_BREAK_IF(btnClrCache == nullptr);
		CC_BREAK_IF(imgBackground == nullptr);

		btnMoreInfo->setEnabled(true);
		btnMoreInfo->setBright(true);

		if(app->getBGMstatus() == true){
			btnSoundON->setBright(false);
			btnSoundOFF->setBright(true);
		}else{
			btnSoundON->setBright(true);
			btnSoundOFF->setBright(false);
		}
		//auto btnContinue = Button::create();
		//btnContinue->setTouchEnabled(true);
		//btnContinue->loadTextures("res/public/contiune_btn.png", "res/public/contiune_btn_press.png", "");
		//btnContinue->setPosition(Vec2(visiblesize.width-80, 50));
		btnClose->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnQuitCallBack, this));
		btnSoundON->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnSoundONCallBack, this));
		btnSoundOFF->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnSoundOFFCallBack, this));
		btnMoreInfo->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnMoreCallBack, this));
		btnTeam->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnTeamCallBack, this));
		btnTerm->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnTermCallBack, this));
		btnClrCache->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnClearCacheCallBack, this));
		imgBackground->setAnchorPoint(Vec2(0, 0));
		imgBackground->setPosition(Vec2(0, 0));
		txtVerion->setColor(ccc3(72, 37, 0));
		txtVerion->setText(VERSION);
		txtVerion->setPosition(Vec2(870, 30));
		txtVerion->setFontSize(36.0f);
		txtVerion->setFontName("Humanst521 BT.ttf");


		//btnContinue->setTag(2003);
		//this->addChild(btnClose);
		//this->addChild(btnSoundON);
		//this->addChild(btnSoundOFF);
		//this->addChild(btnMoreInfo);
		//this->addChild(btnTeam);
		//this->addChild(btnTerm);
		this->addChild(imgBackground, -1, 222);
		this->addChild(txtVerion, 999, 223);

		//preload button click effect
		SimpleAudioEngine::getInstance()->preloadEffect(SOUND_BUTTON_CLICK);

		auto btnClear = dynamic_cast<Button*>(_UINode->getChildByName(SETTING_CLEAR_CACHE));
		CC_BREAK_IF(btnClear == nullptr);
		//std::string  sUsername = UserDefault::getInstance()->getStringForKey("tweebaa_username", "\0");

		if (m_UserID>0)
		{
			btnClear->setTitleText("LOG OUT        ");//space for align
		}
		else
		{
			btnClear->setTitleText("CLEAR CACHE");
		}
		bRet = true;

	}while(0);
	return bRet;
}

void SettingScene::btnQuitCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		//MessageBox("This is button","OK");
		Director::getInstance()->popScene();
	}
	return;
}

void SettingScene::btnSoundONCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		//MessageBox("This is button","OK");

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		app->StartBGMusic(BGM_MAIN);

		auto btnSoundON= dynamic_cast<Button*>(this->getChildByTag(1000)->getChildByName(SETTING_BUTTON_ON));
		auto btnSoundOFF= dynamic_cast<Button*>(this->getChildByTag(1000)->getChildByName(SETTING_BUTTON_OFF));
		if (btnSoundON != nullptr)
		{
			btnSoundON->setBright(false);
		};
		if (btnSoundOFF != nullptr)
		{
			if (btnSoundOFF->isBright() == false){
				btnSoundOFF->setBright(true);
			}
		};
		//Director::getInstance()->popScene();
	}
	return;
}

void SettingScene::btnSoundOFFCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		//AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		app->StopBGMusic();
		log("app->StopBGMusic();");
		auto btnSoundON= dynamic_cast<Button*>(this->getChildByTag(1000)->getChildByName(SETTING_BUTTON_ON));
		auto btnSoundOFF= dynamic_cast<Button*>(this->getChildByTag(1000)->getChildByName(SETTING_BUTTON_OFF));
		if (btnSoundOFF != nullptr)
		{
			btnSoundOFF->setBright(false);
		};
		if (btnSoundON != nullptr)
		{
			if (btnSoundON->isBright() == false){
				btnSoundON->setBright(true);
			}
		};
		//MessageBox("This is button","OK");
		//Director::getInstance()->popScene();
	}
	return;
}
void SettingScene::btnTeamCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		//MessageBox("This is button","OK");
		Node* creditnode = CSLoader::createNode(SETTING_TEAM_UI_CSB);
		creditnode->setTag(SETTING_TEAM_NODE_TAG);
		this->addChild(creditnode);

		auto btnBack = dynamic_cast<Button*>(creditnode->getChildByName(SETTING_TEAM_BUTTON));
		if(btnBack == nullptr) return;
		btnBack->addTouchEventListener(CC_CALLBACK_2(SettingScene::btnTeamBackCallBack, this));
	}
	return;
}

void SettingScene::btnMoreCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		//MessageBox("This is button","OK");
		//Director::getInstance()->popScene();
		//Director::getInstance()->popScene();
		//LayerColor *layer1 = LayerColor::create(Color4B(0, 0, 0, 180));
		//TurntableSystem* price =new TurntableSystem(layer1);
		//layer1->setTag(CHILD_TURNTABLE);
		//addChild(layer1,99);

		//Link to More info
		Application::getInstance()->openURL(MOREINFO_URL);
		
	}
	return;
}

void SettingScene::btnTermCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		//MessageBox("This is button","OK");
		//Director::getInstance()->popScene();
		Application::getInstance()->openURL(TERM_URL);
	}
	return;
}

void SettingScene::btnTeamBackCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		//MessageBox("This is button","OK");
		//Director::getInstance()->popScene();
		if (this->getChildByTag(SETTING_TEAM_NODE_TAG) != nullptr){
			this->removeChildByTag(SETTING_TEAM_NODE_TAG);
		}
	}
	return;
}


void SettingScene::btnClearCacheCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}

		MsgBoxType m_type = TP_Q_YESNO;
		std::string title = MSGBOX_TITLE;
		std::string msg = "All temporary files will be deleted, are you sure?";

		MsgBox::setMsgBox(m_type,&title,&msg);
		auto msgbox = MsgBox::create();
		msgbox->setTag(9000);
		this->addChild(msgbox,999);
	
		//Add Event to process
		auto btnYes = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_Yes"));// Magnifier Button
		auto btnNo = dynamic_cast<Button*>(msgbox->getChildByTag(MSGBOX_UINODE)->getChildByName("btn_No"));// Magnifier Button
		btnYes->addTouchEventListener(CC_CALLBACK_2(SettingScene::yesCallback, this));
		btnNo->addTouchEventListener(CC_CALLBACK_2(SettingScene::noCallback, this));
		return;
	}

	//auto director = Director::getInstance();
	//auto scene = CollageScene::createScene();
	//director->replaceScene(scene);
	//getEventDispatcher()->removeCustomEventListeners(APP_CE_OK);
}

void SettingScene::btnContinueCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType){
	if (eEventType == cocos2d::ui::Widget::TouchEventType::ENDED){
		return;
	}

	//auto director = Director::getInstance();
	//auto scene = CollageScene::createScene();
	//director->replaceScene(scene);
	//getEventDispatcher()->removeCustomEventListeners(APP_CE_OK);
}
void SettingScene::loadingCallBack(Texture2D *texture){
	//AtlasLoader::getInstance()->loadAtlas("zoom_atlas.txt", texture);

}
void SettingScene::updateCallBack(float dt) {
	//AppDelegate *app = (AppDelegate*)Application::getInstance();
	//app->initGameDefault();
}

void SettingScene::yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED){

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}

		UserDefault *load = UserDefault::getInstance();

		//remove Cache directory
		std::string fileName = SETTING_PATH_DIRECTORY;
		string path = FileUtils::getInstance()->getWritablePath() + fileName;

		if (FileUtils::getInstance()->isDirectoryExist(path)){
			//MessageBox("SettingScene: Cache directory exists", MSGBOX_TITLE);
			if (FileUtils::getInstance()->removeDirectory(path) == false){
				log("Oops, Fail to clear directory %s",path.c_str());
//				MessageBox("Oops, Fail to clear temporary image file", MSGBOX_TITLE);
			}

		}

		//remove db file on disk
		path = FileUtils::getInstance()->getWritablePath() + DB_File;
		if (FileUtils::getInstance()->isFileExist(path)){
			//MessageBox("SettingScene: Cache directory exists", MSGBOX_TITLE);
			if (FileUtils::getInstance()->removeFile(path) == false){
				log("Oops, Fail to remove db3 %s",path.c_str());
			}

		}

		//reset user id to not login
		m_UserID = -1;
		UserDefault::getInstance()->setIntegerForKey("CurrentUserId", m_UserID);
		//initialize DB
		UserDefault::getInstance()->setStringForKey("DBInitial", "\0");
		DataAccess::initDB();

		//Reset all setting
		load->setIntegerForKey("IsUserDefaultInited", 2);
		app->initGameDefault();
		
		m_Refresh = 1;

		//Refresh 
		Node* _UINode = this->getChildByTag(1000);
		auto btnSoundON= dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_ON));
		auto btnSoundOFF= dynamic_cast<Button*>(_UINode->getChildByName(SETTING_BUTTON_OFF));
		if(app->getBGMstatus() == true){
			btnSoundON->setBright(false);
			btnSoundOFF->setBright(true);
			app->StartBGMusic(BGM_MAIN);
		}else{
			btnSoundON->setBright(true);
			btnSoundOFF->setBright(false);
			app->StopBGMusic();
		}
		
		this->removeChildByTag(9000);
        CCLOG("touch ended.");
    }
}

void SettingScene::noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED)         {

		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->getBGMstatus() == BGMusic_flag::ON){
			app->PlayEffect(SOUND_BUTTON_CLICK);
		}
		
		//Don't quit the game
		this->removeChildByTag(9000);
		//enable the touch event
        CCLOG("touch ended.");
    }
}

