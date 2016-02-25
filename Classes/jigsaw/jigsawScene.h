#pragma once
//#include "SlidingMenuLayer.h"
#include "PictureGameLayer.h"

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class jigSawScene : public Scene{
public:
	jigSawScene(void);
	~jigSawScene(void);
	bool virtual init();
	CREATE_FUNC(jigSawScene);
};