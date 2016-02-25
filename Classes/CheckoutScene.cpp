#include "CheckoutScene.h"

CheckoutScene::CheckoutScene(){};

CheckoutScene::~CheckoutScene(){};

bool CheckoutScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _welcomeLayer = CheckoutLayer::create();
		//auto _welcomeLayer = StatusLayer::create();
		CC_BREAK_IF(!_welcomeLayer);
		this->addChild(_welcomeLayer);
		bRet = true;
	}while(0);
	return bRet;
}


