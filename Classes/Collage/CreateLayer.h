//===========Tweebaa==============
//CreateLayer.h	--Jly 27 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at
#pragma once

#include "cocos2d.h"
#include "category.h"
#include "DesignLayer.h"
#include "PublishLayer.h"


//#include "../Msgbox/Dialogbox.h"
//#include "../Msgbox/MsgBox.h"

#include "../MemberCenter/RegisterView.h"

class MainMenu;

class CreateLayer :public Layer
{
	public:
		char m_sFN[128];
		//MainMenu(cocos2d::Node* menuNode);
		CreateLayer(cocos2d::Node* rootNode);
		~CreateLayer(void);
		Node* currentNode;
		DesignLayer* designLayer;
		Category * categoryLayer;

		Node* _design;
		Scene* pScene;
		Point ancPos;

		enum right_menuItem{AddItem=MenuItem_AddItem,Flip=MenuItem_Fit,Clear=MenuItem_Clear,Save=MenuItem_Save,Publish=MenuItem_Publish};
		static right_menuItem currentMenuItem;
		static Size visiableSize;
		static bool m_bIsTheme;
		Point origin;

		void initialize(Node*);
		void touchAddItem(Node* object,TouchEventType type);
		void touchClear(Node* object,TouchEventType type);
		void touchSave(Node* object,TouchEventType type);
		void touchPublish(Node* object,TouchEventType type);
		void touchClone(Node* object,TouchEventType type);
		void touchUndo(Node* object,TouchEventType type);
		void touchRedo(Node* object,TouchEventType type);
		void touchFlip(Node* object,TouchEventType type);
		//void touchText(Node* object,TouchEventType type);
		void ClickButton(Ref* sender);
		//void ResetRMButton(Node*);

		void onSaveToFile(RenderTexture*, const std::string);
		void afterCapture(bool succeed, const std::string& outputFile);

		
	
	private:
		Rect rectDesign;
		HttpConnection * http_connect;
		Button *buttonSave;
		Button *buttonPublish;
		Node	*m__pMain;

		void	popupLayer(Node* baseLayer,Vec2 popPos,bool,const char *);
		void	popupCallback(cocos2d::Node *pNode);
		void	MsgBox_Publish_Callback(cocos2d::Node *pNode,void*);
		void	MsgBox_Published(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent);
		void	MsgBoxCallback(cocos2d::Node *pNode,void*);
		void	MsgBox_Share(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent);
		void	MsgBox_Share_Callback(cocos2d::Node *pNode,void*);
		void	MsgBox_Clear_Callback(cocos2d::Node *pNode,void*);
		void	MsgBox_Save_Callback(cocos2d::Node *pNode,void*);
		
};

