#pragma once
#include "GameScene.h"
#include "GameLayer.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class GameScene : public Scene{
public:
	GameScene(void);
	~GameScene(void);
	bool virtual init();
	CREATE_FUNC(GameScene);

};