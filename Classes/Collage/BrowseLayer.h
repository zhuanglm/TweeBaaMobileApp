//===========Tweebaa==============
//BrowseLayer.h	--Jly 20 2015	
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

using namespace std;
using namespace ui;

USING_NS_CC;

class BrowseLayer :public Layer
{
	private:
		
		static Text*	textLike, *textShare;
		float	m_fRatio;
		Slider*	m_Slider;
		int		m_LikeFlag;
		
		static	Label *label_Designer;
		static	Label *label_Title,*label_Description;
		HttpConnection * http_connect;
		CheckBox*	buttonLike;
		Button*	buttonShare;
		Button*	buttonLeft,*buttonRight;

		CollageTemplateList *pCTL;	//product list from xml

		void sliderEvent(Ref * pSender, Slider::EventType type);
		void TrendsEvent(Ref * pSender, PageView::EventType type);
		void touchLike(Ref* object,CheckBoxEventType type);
		void touchShare(Ref* object,cocos2d::ui::Widget::TouchEventType type);
		void touchLeft(Ref* object,cocos2d::ui::Widget::TouchEventType type);
		void touchRight(Ref* object,cocos2d::ui::Widget::TouchEventType type);

	
	public:
		Size visiableSize;
		static int nCurPage;
		//static ListView*	infoArea;
		static Layer*	infoArea;
		BrowseLayer(cocos2d::Node*);
		~BrowseLayer(void);

		virtual bool init();
		void initialize(cocos2d::Node* pNode);
		void	RightBarCallBack(cocos2d::Ref *pSender, Widget::TouchEventType event);


};



