#include "Gzi.h"
#include "../ZhuanPanSystem/Zp_ImgPath.h"
//#include "../VisibleRect.h"

Gzi::Gzi(Sprite* sp,Zp_BoxData* data)
{
	string setbd;
	if(data->get_xuanzhong()==true)
	{
		setbd = IMG_ZP_KUANGBD_XUANZHONG;
	}
	else
	{
		setbd = IMG_ZP_KUANGBD;
	}
	Sprite* bd = Sprite::create(setbd.c_str());

	bd->setPosition(data->get_point());
	int bd_w = bd->getContentSize().width;
	int bd_h = bd->getContentSize().height;

	int iBoxId = data->get_boxid();
	if (iBoxId == 0 || iBoxId == 7 || iBoxId == 12 || iBoxId == 19)//tips
	{
		Sprite* tips = Sprite::create(IMG_ZP_TIPS);
		tips->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(tips, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 1 || iBoxId == 15)//Point_1
	{
		Sprite* points_1 = Sprite::create(IMG_ZP_Points_1);
		points_1->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(points_1, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 2 || iBoxId == 5)//Point_10
	{
		Sprite* points_10 = Sprite::create(IMG_ZP_Points_10);
		points_10->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(points_10, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 3 || iBoxId == 13)//Point_50
	{
		Sprite* points_50 = Sprite::create(IMG_ZP_Points_50);
		points_50->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(points_50, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 8 || iBoxId == 18)//Point_100
	{
		Sprite* points_100 = Sprite::create(IMG_ZP_Points_100);
		points_100->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(points_100, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 16)//Tweebucks_5
	{
		Sprite* tweebucks_5 = Sprite::create(IMG_ZP_Tweebucks_5);
		tweebucks_5->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(tweebucks_5, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 20)//Tweebucks_10
	{
		Sprite* tweebucks_10 = Sprite::create(IMG_ZP_Tweebucks_10);
		tweebucks_10->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(tweebucks_10, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 14)//Wonder_Cup
	{
		Sprite* wonder_cup = Sprite::create(IMG_ZP_Wonder_Cup);
		wonder_cup->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(wonder_cup, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else if (iBoxId == 4)//Tweebot
	{
		Sprite* tweebot = Sprite::create(IMG_ZP_Tweebot);
		tweebot->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(tweebot, -1, TAG_ZP_GEZI_NEIRONG);
	}
	else
	{
		Sprite* chips_meiyou = Sprite::create(IMG_ZP_MEIYOU);
		chips_meiyou->setPosition(Vec2(bd_w / 2, bd_h / 2));
		bd->addChild(chips_meiyou, -1, TAG_ZP_GEZI_NEIRONG);
	}


	sp->addChild(bd,3,data->get_boxid());
}

Gzi::~Gzi(void)
{
}
