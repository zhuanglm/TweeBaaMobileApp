#pragma once
#ifndef  _EventBoard_H_
#define  _EventBoard_H_
//#include "../AtlasLoader.h"
//#include "FindDiffLayer.h"
//#include "../BackgroundLayer.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "../memory_match/memory_match_AtlasLoader.h"

using namespace cocos2d;
using namespace std;

#define EVENTBOARD_UI_CSB "res/event_board/event_board.csb"
#define EVENTBOARD_BUTTON_CLOSE "close"
#define EVENTBOARD_BTN_TWEEBOT "BtnTweebot"
#define EVENTBOARD_BTN_SLOT "BtnSlot"
#define EVENTBOARD_BTN_10000 "Btn10000"
#define EVENTBOARD_BUTTON1 "Button_1"
#define EVENTBOARD_BUTTON2 "Button_2"
#define EVENTBOARD_BUTTON3 "Button_3"
#define EVENTBOARD_BACKGROUND "res/public/bg_2.png"
#define EVENTBOARD_PAGEVIEW "PageView"
#define EVENTBOARD_PANEL1 "Panel_1"
#define EVENTBOARD_PANEL2 "Panel_2"
#define EVENTBOARD_PANEL3 "Panel_3"

#define MAXPAGE 3
#define EVENTBOARD_BTN_ON "res/event_board/dot_d.png"
#define EVENTBOARD_BTN_OFF "res/event_board/dot.png"


class EventBoard : public Scene{
public:
	int _ttl_time;
	cocos2d::Node* _UINode;
	EventBoard(void);
	~EventBoard(void);
	bool virtual init();
	CREATE_FUNC(EventBoard);

	void btnQuitCallBack(Ref* pSender, ui::Widget::TouchEventType eEventType);
	//void imgOpenPageCallBack(Ref* pSender, ui::Widget::TouchEventType eEventTyp);
	void touchButton(Ref* pSender, ui::Widget::TouchEventType eEventTyp);
	void pageViewEvent(Ref *pSender, ui::PageViewEventType type);
	void playAni();
	void disableAni();

private:
	int iPage;
};


#endif //_EventBoard_H_