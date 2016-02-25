#pragma once
//#include "AtlasLoader.h"
#include "CheckoutLayer.h"
//#include "BackgroundLayer.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class CheckoutScene : public Scene{
public:
	std::string sProductID;
	CheckoutScene(void);
	~CheckoutScene(void);
	bool virtual init();
	CREATE_FUNC(CheckoutScene);
};