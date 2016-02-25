#include "FindDiffScene.h"


FindDiffScene::FindDiffScene(){};

FindDiffScene::~FindDiffScene(){};

bool FindDiffScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
		auto _finddiffLayer = FindDiffLayer::create();
		//auto _zoomphotoLayer = StatusLayer::create();
		CC_BREAK_IF(!_finddiffLayer);
		this->addChild(_finddiffLayer);

		// start ansyc method load the atlas.png
		//Director::getInstance()->getTextureCache()->addImageAsync("zoom_atlas.png", CC_CALLBACK_1(FindDiffScene::loadingCallBack, this));
		bRet = true;
	}while(0);

	//if(bRet == false){
	//	//auto videoscene = VideoScene::create();
	//}
	return bRet;
}


void FindDiffScene::loadingCallBack(Texture2D *texture){
	//AtlasLoader::getInstance()->loadAtlas("zoom_atlas.txt", texture);

}