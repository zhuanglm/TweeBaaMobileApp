#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class SpinLucky : public CCObject
{
public:
	SpinLucky(void);
	~SpinLucky(void);

	
	std::string Username;
	std::string RewardItemName;
	std::string sLuckyDate;
};