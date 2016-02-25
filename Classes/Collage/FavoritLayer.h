//===========Tweebaa==============
//FavoritLayer.h	--Aug 27 2015	
//Version 1.1 by Raymond Zhuang at Toronto
// last modified at Nov 17
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


class FavoritLayer :public Layer
{
	private:
		
		static Text*	textLike, *textShare;
		float	m_fRatio;
		Slider*	m_Slider;
		ImageView*	m_Draft;
		
		static	Label *label_Designer;
		static	Label *label_Title,*label_Description;
		HttpConnection * http_connect;
		CheckBox*	buttonLike;
		Button*	buttonShare;
		Button*	buttonLeft,*buttonRight,*buttonDel;

		CollageTemplateList *pCTL;	//product list from xml

		void sliderEvent(Ref * pSender, Slider::EventType type);
		void TrendsEvent(Ref * pSender, PageView::EventType type);
		void touchLike(Node* object,CheckBoxEventType type);
		void touchShare(Node* object,TouchEventType type);
		void touchLeft(Ref* object,cocos2d::ui::Widget::TouchEventType type);
		void touchRight(Ref* object,cocos2d::ui::Widget::TouchEventType type);
		void touchDel(Ref* object,cocos2d::ui::Widget::TouchEventType type);

		void clickImgCallback(cocos2d::Ref *pSender, Widget::TouchEventType event) ;
		void MsgBox_Load_Callback(cocos2d::Node *pNode,void*);
		
		void MsgBox_Del_Callback(cocos2d::Node *pNode,void*);
	
	public:
		Size visiableSize;
		static int nCurPage;
		static Layer*	infoArea;
		FavoritLayer(cocos2d::Node*);
		~FavoritLayer(void);

		virtual bool init();
		void initialize(cocos2d::Node* pNode);
		void	RightBarCallBack(cocos2d::Ref *pSender, Widget::TouchEventType event);

};



