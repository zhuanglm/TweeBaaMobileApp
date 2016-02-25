#include "TurntableScene.h"
#include "TurntableSystem.h"

#include "AppDelegate.h"

USING_NS_CC;



Scene* TurntableScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TurntableScene::create();



    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TurntableScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	//TurntableSystem::TurntableSystem(this);

   
    return true;
}


