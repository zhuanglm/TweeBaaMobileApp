#include "jigSawScene.h"

jigSawScene::jigSawScene(){};

jigSawScene::~jigSawScene(){};

bool jigSawScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _welcomeLayer = PictureGameLayer::create();
		CC_BREAK_IF(!_welcomeLayer);
		this->addChild(_welcomeLayer);
		bRet = true;
	}while(0);
	return bRet;
}


