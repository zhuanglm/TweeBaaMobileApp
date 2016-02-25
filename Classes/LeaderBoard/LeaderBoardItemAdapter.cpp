#include "LeaderBoardItemAdapter.h"
//#include "VisibleRect.h"

bool LeaderBoardItemAdapter::init()
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!Layout::init());

		auto winSize = Director::getInstance()->getWinSize();
		//itemIcon = ui::ImageView::create();//Icon
		//itemIcon->setScaleY(VisibleRect::getImageScaleY());
		//itemIcon->setScaleX(VisibleRect::getImageScale());
		//itemIcon->ignoreAnchorPointForPosition(false);
		//itemIcon->setAnchorPoint(Vec2(0.5,0.5));
		// itemIcon->setPosition(VisibleRect::center());

		//auto rIcon = RelativeLayoutParameter::create();
		//rIcon->setAlign(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
		//itemIcon->setLayoutParameter(rIcon);
		//addChild(itemIcon);

		LinearLayoutParameter* lp1 = LinearLayoutParameter::create();
		//lp1->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
		lp1->setMargin(Margin(0, 10, 0, 10));
		lp1->setGravity(LINEAR_GRAVITY_LEFT);

		Layout* layout1 = Layout::create();
		layout1->setLayoutType(LAYOUT_ABSOLUTE);

		Layout* layout2 = Layout::create();
		layout2->setContentSize(Size(200.0, 30.0));

		itemNum = ui::Text::create();
		itemNum->ignoreAnchorPointForPosition(false);
		itemNum->setAnchorPoint(Vec2(0, 0));
		itemNum->setPosition(Vec2(10, 0));
		itemNum->setFontSize(22);

		itemNum->setColor(Color3B(73, 34, 0));
		itemNum->setLayoutParameter(lp1);
		layout2->addChild(itemNum);
		layout1->addChild(layout2);


		Layout* layout3 = Layout::create();
		layout3->setContentSize(Size(300.0, 30.0));
		itemUserName = ui::Text::create();//Text
		itemUserName->ignoreAnchorPointForPosition(false);
		itemUserName->setAnchorPoint(Vec2(0, 0));
		itemUserName->setPosition(Vec2(130,0));
		itemUserName->setFontSize(22);
		
		itemUserName->setColor(Color3B(73, 34, 0));

		/*
		auto rprogress = RelativeLayoutParameter::create();
		rprogress->setAlign(cocos2d::ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
		rprogress->setMargin(Margin(10,10,10,10));
		itemTitleName->setLayoutParameter(rprogress);
		*/
		itemUserName->setLayoutParameter(lp1);
		layout3->addChild(itemUserName);
		layout1->addChild(layout3);


		Layout* layout4 = Layout::create();
		layout4->setContentSize(Size(210.0, 30.0));
		itemScore = ui::Text::create();//Text
		itemScore->ignoreAnchorPointForPosition(false);
		itemScore->setAnchorPoint(Vec2(0, 0));
		itemScore->setPosition(Vec2(450, 0));
		itemScore->setFontSize(22);
		
		itemScore->setColor(Color3B(73, 34, 0));
		itemScore->setLayoutParameter(lp1);
		layout4->addChild(itemScore);
		layout1->addChild(layout4);

		this->addChild(layout1);
		bRet = true;
	}while(0);
	return bRet;
}