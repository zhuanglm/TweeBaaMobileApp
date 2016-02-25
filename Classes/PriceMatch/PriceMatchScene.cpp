#include "PriceMatchScene.h"

PriceMatchScene::PriceMatchScene(){};

PriceMatchScene::~PriceMatchScene(){};

bool PriceMatchScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _welcomeLayer = PriceMatchLayer::create();
		//auto _welcomeLayer = StatusLayer::create();
		CC_BREAK_IF(!_welcomeLayer);
		this->addChild(_welcomeLayer);
		bRet = true;
	}while(0);
	return bRet;
}


