#ifndef __CCALERT_H__
#define __CCALERT_H__
#include "cocos2d.h"
USING_NS_CC;
enum AlertType{
    AlertType_OkAndCancel=0,
    AlertType_OkOnly,
};
enum{
    Item_OK_tag=0,
    Item_Cancel_tag,
};
class CCAlert;
class CCAlertDelegate
{
public:
    virtual void onOk(CCAlert* pSender){};
    virtual void onCancel(CCAlert* pSender){};
};


class CCAlert: public CCLayer, public CCAlertDelegate
{
public:
    CCAlert(void);
    ~CCAlert(void);
    static CCAlert* create(std::string sMessage,int alertTag, CCAlertDelegate* del, AlertType type=AlertType_OkAndCancel);
    bool init(std::string sMessage,int alertTag, CCAlertDelegate* del, AlertType type);

    void alertFunc(Ref* pSender);

    virtual void onEnter();
    virtual void onExit();

//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
protected:
    AlertType m_alertType;
    CCAlertDelegate* m_delegate;
};

#endif