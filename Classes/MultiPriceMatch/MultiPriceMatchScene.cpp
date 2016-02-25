#include "MultiPriceMatchScene.h"
#include "MultiPriceMatch.h"

MultiPriceMatchScene::MultiPriceMatchScene(){};

MultiPriceMatchScene::~MultiPriceMatchScene(){};

bool MultiPriceMatchScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _finddiffLayer = MultiPriceMatch::create();
		//auto _zoomphotoLayer = StatusLayer::create();
		CC_BREAK_IF(!_finddiffLayer);
		this->addChild(_finddiffLayer);

		// start ansyc method load the atlas.png
		//Director::getInstance()->getTextureCache()->addImageAsync("zoom_atlas.png", CC_CALLBACK_1(MultiPriceMatchScene::loadingCallBack, this));
		bRet = true;
	}while(0);
	return bRet;
}


void MultiPriceMatchScene::loadingCallBack(Texture2D *texture){
	//AtlasLoader::getInstance()->loadAtlas("zoom_atlas.txt", texture);

}