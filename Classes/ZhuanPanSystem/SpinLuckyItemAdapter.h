#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
class SpinLuckyItemAdapter : public Layout
{
public:

CREATE_FUNC(SpinLuckyItemAdapter);
virtual bool init();

//void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

//ui::Text* itemNum;
ui::Text* itemUserName;
ui::Text* itemPrize;
ui::Text* itemDateTime;
};

