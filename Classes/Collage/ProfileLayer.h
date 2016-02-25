//===========Tweebaa==============
//ProfileLayer.h	--Nov 25 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#pragma once

#include "Collage_Template.h"
#include "HttpConnection.h"
#include "DesignLayer.h"
#include <cstdlib>
#include <fstream>

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "GetImage.h"
#endif

using namespace std;
using namespace ui;

USING_NS_CC;


class ProfileLayer :public Layer
{
	private:
		
		void TrendsEvent(Ref * pSender, PageView::EventType type);

		void clickImgCallback(cocos2d::Ref *pSender, Widget::TouchEventType event) ;
		void MsgBox_Load_Callback(cocos2d::Node *pNode,void*);
		bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
		void touchDel(Ref* object,cocos2d::ui::Widget::TouchEventType type);
		void MsgBox_Del_Callback(cocos2d::Node *pNode,void*);
		void Close_Callback(cocos2d::Object *pSender,TouchEventType type);
		void Avatar_Callback(cocos2d::Object *pSender,TouchEventType type);

		Object* m_callbackListener;
		SEL_CallFuncND m_callback;
		void * m_pData;

		Button *buttonDel;

		CC_SYNTHESIZE_RETAIN(Node *, m__pPopBox, BackGround);
	
	public:
		Size visiableSize;
		static int nCurPage;
		ProfileLayer();
		~ProfileLayer(void);

		virtual bool init();
		CREATE_FUNC(ProfileLayer);
		void setCallbackFunc(Object* target, SEL_CallFuncND callfun,void *);

		static ProfileLayer * create(const char *);
		virtual void onEnter();
		virtual void onExit();

};



