#include "SpinLayer.h"
#include "WelcomeScene.h"

#include "../MemberCenter/RegisterTabScene.h"
#include "global_const.h"
#include "../theme/ThemeLayer.h"
#include "ui/CocosGUI.h"
#include "../Collage/CollageScene.h"
#include "../ZhuanPanSystem/SpinLuckyItemAdapter.h"
#include "AppDelegate.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../utilits/pugixml.hpp"
#include "../utilits/NetworkUtil.h"
#include "../Msgbox/Dialogbox.h"
#include "ChooseLayer.h"
#include "../ZhuanPanSystem/TurntableSystem.h"

using namespace ui; 


Scene* SpinLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto pLayer = new SpinLayer();

	if (scene && pLayer && pLayer->init())
	{
		pLayer->autorelease();
		scene->addChild(pLayer);
		return scene;
	}
	else
	{
		delete pLayer;
		pLayer = NULL;
		return NULL;
	}
}

SpinLayer::SpinLayer(){


}
SpinLayer::~SpinLayer(){

}


bool SpinLayer::init()
{
	//////////////////////////////  
	// 1. super init first  
	if (!Layer::init())
	{
		return false;
	}

	
	LayerColor *layer1 = LayerColor::create(Color4B(0, 0, 0, 180));
	TurntableSystem* price =new TurntableSystem(layer1);
	layer1->setTag(CHILD_TURNTABLE);
	addChild(layer1,99);
	
	return true;

}

