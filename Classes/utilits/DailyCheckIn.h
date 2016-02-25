#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class DailyCheckIn : public CCObject
{
public:
	DailyCheckIn(void);
	~DailyCheckIn(void);

	/*
	<createtime>2015-09-28T10:25:26.737</createtime>
	<SameWeek>0</SameWeek>
	<WeekDay>mon</WeekDay>
	*/
	std::string checkin_date;
	int isSameWeek;
	std::string sWeekDay;
};