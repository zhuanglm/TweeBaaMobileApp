#pragma once
#include <string>

#include "cocos2d.h"
using namespace cocos2d;
class GameRound: public Node
{
public:
	GameRound(void);
	~GameRound(void);

	int round_id;
	int round_step;
	int round_type;
	std::string round_info;
	};