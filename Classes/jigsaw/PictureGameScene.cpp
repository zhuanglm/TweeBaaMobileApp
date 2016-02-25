#include "PictureGameScene.h"

PictureGameScene::PictureGameScene(){

};

PictureGameScene::~PictureGameScene(){};

bool PictureGameScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _welcomeLayer = PictureGameLayer::create();
		//_welcomeLayer->jigSawFile=this->jigSawFile;
		//auto _welcomeLayer = SlidingMenuLayer::create();
		//auto _welcomeLayer = StatusLayer::create();
		CC_BREAK_IF(!_welcomeLayer);
		this->addChild(_welcomeLayer);
		bRet = true;
	}while(0);
	return bRet;
}


