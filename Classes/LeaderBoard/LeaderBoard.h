#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class LeaderBoard : public CCObject
{
public:
	LeaderBoard(void);
	~LeaderBoard(void);

	int game_type_id;
	std::string Username;
	std::string UserScore;
};