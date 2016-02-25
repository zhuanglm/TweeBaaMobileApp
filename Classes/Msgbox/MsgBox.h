#ifndef _MSGBOX_H_
#define _MSGBOX_H_

#include "DialogLayer.h"
#include "Dialogbox.h"

#define APP_CE_OK "CUSTOMEVENT_OK"
#define APP_CE_CANCEL "CUSTOMEVENT_CANCEL"
#define APP_CE_YES "CUSTOMEVENT_YES"
#define APP_CE_NO "CUSTOMEVENT_NO"
#define MSGBOX_UI_CSB "MsgBox/MsgBoxScene.csb"
#define MSGBOX_UINODE 1001


class MsgBox: public DialogLayer
{
public:

	static int msgtype;
	static std::string* m_Msgbody;
	static std::string* m_Title;
	static std::string* m_Info;

	int m_MsgType;
	int mRet;
	//int* pRet;

   cocos2d::Sprite* sprite,*sprite2;
   cocos2d::Label* label;
   bool spritePaused;
   cocos2d::Vector<Node*> pausedNodes;

	virtual bool onInitDialog();
    CREATE_FUNC(MsgBox);

public:
	static void setMsgBox(MsgBoxType type, std::string* pTitle, std::string* pMsg);
	static void setMsgBox(MsgBoxType type, std::string* pTitle, std::string* pMsg, std::string* pInfo);
	void btnClick(Ref* pSender, ui::Widget::TouchEventType eEventType);

    bool test();
	void okMenuItemCallback(CCObject *obj);
    void cancelMenuItemCallback(CCObject *obj);
    void yesMenuItemCallback(CCObject *obj);
    void noMenuItemCallback(CCObject *obj);
    void postNotification(const char *name);
	void update(float) override;
private:
	cocos2d::Node* _MSG_UINode;
};

#endif
