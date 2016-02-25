//===========Tweebaa==============
//MainMenu.h	--Jly 20 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#pragma once

#include "cocos2d.h"
#include "BrowseLayer.h"
#include "CreateLayer.h"
#include "FavoritLayer.h"
#include "ProfileLayer.h"
#include "../utilits/Tutorial.h"
#include "../theme/ThemeLayer.h"


class MainMenu : public Layer
{
	private:
		virtual bool init();
		//void MsgBox_Quit(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent);
		//void MsgBox_Quit_Callback(cocos2d::Node *pNode,void*);

		void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) ;

		Button *buttonCreate;
		char sSelectFile[128];

	public:
		//MainMenu(cocos2d::Node* menuNode);
		//MainMenu(cocos2d::Node* rootNode);
		MainMenu(void);
		~MainMenu(void);
		Node* currentNode;
		CreateLayer * cLayer;
		BrowseLayer * bLayer;
		FavoritLayer * fLayer;
		
		static menuItem currentMenuItem;

		void initialize(Node*);
		void touchProfile(Node* object,TouchEventType type);
		void touchTrends(Node* object,TouchEventType type);
		void touchFeed(Node* object,TouchEventType type);
		void touchCreate(Node* object,TouchEventType type);
		void touchHome(Node* object,TouchEventType type);
		void touchHelp(Node* object,TouchEventType type);
		void ClickButton(Ref* sender);
		void ResetLMButton(Node*);
		void Forward2Create(cocos2d::Node *,void *);

		void MsgBox_Quit(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent);
		void MsgBox_Quit_Callback(cocos2d::Node *pNode,void*);
		void MsgBox_Reg_Callback(cocos2d::Node *pNode,void* data);
	
		//Add by Long
		void touchShop(Node* object, TouchEventType type);

		static cocos2d::Scene* createScene();
		CREATE_FUNC(MainMenu);

};

