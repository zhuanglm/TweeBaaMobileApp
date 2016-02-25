#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
class LeaderBoardItemAdapter : public Layout
{
public:

CREATE_FUNC(LeaderBoardItemAdapter);
virtual bool init();

//void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

ui::Text* itemNum;
ui::Text* itemUserName;
ui::Text* itemScore;

};

