
//===========Tweebaa==============
//PublishLayer.h	--Oct 8 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#ifndef __crossDT_PublishLayer__
#define __crossDT_PublishLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

//using namespace cocos2d::extension;
#include "ui/UIScale9Sprite.h"
using namespace ui;

 
class PublishLayer :public Layer//,public TextFieldDelegate
{
public:
    PublishLayer();
    ~PublishLayer();
    virtual bool init();
    CREATE_FUNC(PublishLayer);
 
	bool onTouchBegan(Touch *touch, Event *unused_event);
 
    static PublishLayer * create(const char* backgroundCSB,bool,const char *);

	TextField * TextTitle;
	TextField * TextDes;
	static std::string InputCont1;
	static std::string InputCont2;
   
    void setCallbackFunc(Object* target, SEL_CallFuncN callfun);
	void textFieldEvent(Ref * pSender, TextField::EventType type);
     
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, const Color3B & color, int tag=0);
	virtual void onEnter();
    virtual void onExit();

	/*virtual bool 	onTextFieldAttachWithIME (TextFieldTTF *sender);
	virtual bool 	onTextFieldDetachWithIME (TextFieldTTF *sender);
	virtual bool 	onTextFieldInsertText (TextFieldTTF *sender, const char *text, size_t nLen);
	virtual bool 	onTextFieldDeleteBackward (TextFieldTTF *sender, const char *delText, size_t nLen);
	virtual bool 	onVisit (TextFieldTTF *sender, Renderer *renderer, const Mat4 &transform, uint32_t flags);*/

	//void editBoxTextChanged(EditBox *editBox, const std::string &text);
 
private:
	bool m_bPosFlag;
    void buttonCallback(Object* pSender,TouchEventType type);
	void PublishCallback(cocos2d::Object *pSender,TouchEventType type);
 
    // 文字内容两边的空白区
    int m_contentPadding;
    int m_contentPaddingTop;
     
    Object* m_callbackListener;
    SEL_CallFuncN m_callback;
 
    CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE(bool, m_OK_Flag, OK_Flag);
	CC_SYNTHESIZE_RETAIN(Node *, m__pPopBox, BackGround);
	CC_SYNTHESIZE(const char *, m__pPicFile, Pic_File);
 
};
 
 
#endif