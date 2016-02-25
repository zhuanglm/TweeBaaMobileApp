//===========Tweebaa==============
//category.h	--Jly 27 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#pragma once
//#include "..\..\cocos2d\cocos\2d\cclayer.h"
#include "cocos2d.h"
//#include "Collage_Template.h"

//#include "Gif/GIFMovie.h"
//#include "Gif/CacheGif.h"
//#include "Gif/InstantGif.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include "platform/android/jni/JniHelper.h"  
#include<jni.h>  
#endif 

#include "TTWaitting.h"
#include "global_const.h"
#include "HttpConnection.h"
#include "DesignLayer.h"
#include "Collage_Template.h"
#include <cstdlib>
#include "ui/CocosGUI.h"

#include "cocostudio/CocoStudio.h"
//#include "extensions/cocos-ext.h"
//#include "cocos-ext.h"
 
USING_NS_CC;
//USING_NS_CC_EXT;

using namespace std;
using namespace cocostudio; 
using namespace cocos2d::ui;


class Category : public cocos2d::Layer,public ui::EditBoxDelegate
{
	private:

		SPRITE_INFO sprite_info;
		CollageTemplateList *pCTL;
		bool	bIsMoved,bLoadedFlag,bIsNewItemDone;
		string	_fileName,_searchKeyword;
		unsigned int Number_Resources,nErrorFile;
		unsigned int nDisplayedItem;
		size_t	m_nItemNum;
		float	m_Load_ratio;
		Label	*label_Search;
		Label	*label_ratio;
		ui::EditBox* editBox ;
		ui::ScrollView *_scrollDecoration;
		
		vector<sctPicList> v_temp_info;

		HttpConnection * http_connect;
		TTWaitting * m_waitingCtrl;
		unsigned int nFlag;	//waiting spin flag:0 none/1 draw/2 stop/3,4,5 success&stop
		ssize_t		nListCurIndex;	//listview current selected index 解决增删后getindex的值不变 -1 为未赋值
		std::mutex	updateMutex;

		void	UpdateIcon(vector<sctPicList>*,string,AppendFlag flag);
		void	UpdateListview(float);
		bool	LoadCatImg(string);

		void	clickImgCallback(cocos2d::Ref *pSender, Widget::TouchEventType event);
		void	clickBtnCallback(cocos2d::Ref *pSender, Widget::TouchEventType event);

	public:
		Category(cocos2d::Node* rootNode);
		~Category(void);
		
		unsigned static int sel_ID,nProductListOffset,nDecoListOffset;
		GDirection	gd_direction;

		static EventListenerTouchOneByOne * listener;
		static bool	bLongPressed,bMainCated,bProductCated;
		static Layer *	DragLayer;
		static Node *_detailsLayer;
		static Sprite * selSprite;
		static Vec2	curPos,StartPos;
		static	Node* MainCatLayer;
		static	Node* _createLayer;

		Vec2	clickPos;
		
		ListView *_listMainCat;
		//ListView *_listDecoration;
		ListView *_listviewPic;

		void	initialize(Node*);
		void	ActionCallback(Node *pSender,void*data);
		void	ActionCancelCallback(Node *pSender);
		
		void	textCallback(cocos2d::Ref* object,TouchEventType type);
		void	popupLayer(Node* baseLayer,Vec2 popPos);
		void	popupCallback(cocos2d::Node *pNode,void*);
		
		void	CatQuit(cocos2d::Ref* object,TouchEventType type);
		void	CatBack(cocos2d::Ref* object,TouchEventType type);
		void	longPress(float dt);

		bool	onTouchBegan(Touch* touch, Event* event);
		void	onTouchMoved(Touch* touch, Event* event);
		void	onTouchEnded(Touch* touch, Event* event);

		void	ProductViewMoveCallback(cocos2d::Ref *pSender, cocos2d::ui::ScrollView::EventType eventType) ;
		void	DecoViewMoveCallback(cocos2d::Ref *pSender, cocos2d::ui::ScrollView::EventType eventType) ;

		void 	AffixItemRow(ListView* list_view,vector<sctPicList>& v_info,string,AppendFlag);

		virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
		virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
		virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text);
		virtual void editBoxReturn(cocos2d::extension::EditBox *editBox);

};


