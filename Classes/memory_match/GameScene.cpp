#include "GameScene.h"


GameScene::GameScene(){};

GameScene::~GameScene(){};

bool GameScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _GameLayer = GameLayer::create();
		//auto _zoomphotoLayer = StatusLayer::create();
		CC_BREAK_IF(!_GameLayer);
		this->addChild(_GameLayer);


		bRet = true;
	} while (0);


	return bRet;
}
