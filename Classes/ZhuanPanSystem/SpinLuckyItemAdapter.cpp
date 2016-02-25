#include "SpinLuckyItemAdapter.h"
//#include "VisibleRect.h"

bool SpinLuckyItemAdapter::init()
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
		layout2->setColor(Color3B(255, 0, 0));
		layout2->setContentSize(Size(260.0, 30.0));

		itemUserName = ui::Text::create();
		itemUserName->ignoreAnchorPointForPosition(false);
		itemUserName->setAnchorPoint(Vec2(0, 0));
		itemUserName->setPosition(Vec2(10, 0));
		itemUserName->setFontSize(22);
		//itemUserName->setColor(Color3B(128, 128, 128));
		itemUserName->setLayoutParameter(lp1);
		layout2->addChild(itemUserName);
		layout1->addChild(layout2);


		Layout* layout3 = Layout::create();
		layout3->setContentSize(Size(300.0, 30.0));

		itemPrize = ui::Text::create();//Text
		itemPrize->ignoreAnchorPointForPosition(false);
		itemPrize->setAnchorPoint(Vec2(0, 0));
		itemPrize->setPosition(Vec2(195, 0));
		itemPrize->setFontSize(22);
		//itemPrize->setColor(Color3B(128, 128, 128));
		itemPrize->setLayoutParameter(lp1);

		layout3->addChild(itemPrize);
		layout1->addChild(layout3);


		Layout* layout4 = Layout::create();
		layout4->setContentSize(Size(210.0, 30.0));
		itemDateTime = ui::Text::create();//Text

		itemDateTime->ignoreAnchorPointForPosition(false);
		itemDateTime->setAnchorPoint(Vec2(0, 0));
		itemDateTime->setPosition(Vec2(450, 0));
		itemDateTime->setFontSize(22);
		//itemDateTime->setColor(Color3B(73, 34, 0));
		itemDateTime->setLayoutParameter(lp1);
		layout4->addChild(itemDateTime);
		layout1->addChild(layout4);

		this->addChild(layout1);
		bRet = true;
	}while(0);
	return bRet;
}