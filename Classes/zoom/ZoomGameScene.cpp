#include "ZoomGameScene.h"

ZoomGameScene::ZoomGameScene(){};

ZoomGameScene::~ZoomGameScene(){};

bool ZoomGameScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _zoomphotoLayer = ZoomPhotoLayer::create();
		//auto _zoomphotoLayer = StatusLayer::create();
		CC_BREAK_IF(!_zoomphotoLayer);
		this->addChild(_zoomphotoLayer);

		// start ansyc method load the atlas.png
		//Director::getInstance()->getTextureCache()->addImageAsync("zoom_atlas.png", CC_CALLBACK_1(ZoomGameScene::loadingCallBack, this));
		bRet = true;
	}while(0);
	return bRet;
}


void ZoomGameScene::loadingCallBack(Texture2D *texture){
	//AtlasLoader::getInstance()->loadAtlas("zoom_atlas.txt", texture);

}