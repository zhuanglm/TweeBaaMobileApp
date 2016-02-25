#ifndef _DLGBOX_H_
#define _DLGBOX_H_

#include "DialogLayer.h"

enum MsgBoxType{
	TP_NONE = 0,
	TP_Q_YESNO = 1,	// "Yes" "No" button
	TP_Q_OKCANCEL = 2,	// "Ok" "Cancel" button
	TP_INFO = 3,	// "OK" button
};

#define APP_CE_OK "CUSTOMEVENT_OK"
#define APP_CE_CANCEL "CUSTOMEVENT_CANCEL"
#define APP_CE_YES "CUSTOMEVENT_YES"
#define APP_CE_NO "CUSTOMEVENT_NO"

class DlgBox: public DialogLayer
{
public:

	static int msgtype;
	static std::string* m_Msgbody;
	static std::string* m_Title;

	int m_MsgType;
	int mRet;
	//int* pRet;

   cocos2d::Sprite* sprite,*sprite2;
   cocos2d::Label* label;
   bool spritePaused;
   cocos2d::Vector<Node*> pausedNodes;

	virtual bool onInitDialog();
    CREATE_FUNC(DlgBox);

public:
	static void setMsgBox(MsgBoxType type,std::string* str);

    bool test();
	void okMenuItemCallback(CCObject *obj);
    void cancelMenuItemCallback(CCObject *obj);
    void yesMenuItemCallback(CCObject *obj);
    void noMenuItemCallback(CCObject *obj);
    void postNotification(const char *name);
	void update(float) override;
private:
};

#endif
