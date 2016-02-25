#pragma once
#ifndef _Gzi_
#define _Gzi_
#include "cocos2d.h"
#include "../ZhuanPanSystem/Zp_BoxData.h"
USING_NS_CC;
class Gzi :public cocos2d::Sprite
{
public:
	Gzi(Sprite* sp,Zp_BoxData* data);
	~Gzi(void);
	
};

#endif