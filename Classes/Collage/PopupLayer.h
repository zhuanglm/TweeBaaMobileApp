
//===========Tweebaa==============
//PopupLayer.h	--Sep 16 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#ifndef __crossDT_PopupLayer__
#define __crossDT_PopupLayer__
 
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;

//using namespace cocos2d::extension;
#include "ui/UIScale9Sprite.h"
#include "define.h"
using namespace ui;

 
class PopupLayer :public Layer
{
public:
    PopupLayer();
    ~PopupLayer();
    virtual bool init();
    CREATE_FUNC(PopupLayer);
 
	//EventListenerTouchOneByOne * listener;
    bool onTouchBegan(Touch *touch, Event *unused_event);
 
    static PopupLayer * create(const char* backgroundImage);
    void setTitle(const char* title , const Color3B & color,int fontsize=28);
    void setContentText(const char* text , const Color3B & color,int fontsize=20 ,int padding=50 ,int paddintTop=100);
    void setCallbackFunc(Object* target, SEL_CallFuncND callfun,void *);
     
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, const Color3B & color, int tag=0);
	bool addContentButton(const char* normalImage, const char* selectedImage, const char* title, const Color3B & color, int tag=0);
	 /**
     * Add EditBox . 
	 1,背景图
	 2，提示文字
	 3，文字颜色
	 4，文字大小
	 5，文字输入框尺寸
	 6，最大输入字符数
     */
	bool addEditBox(const char* normalImage,const char*, const char* text , const Color3B & color,int fontsize ,const Size rectBox, int nMaxChars);
    virtual void onEnter();
    virtual void onExit();

	//void editBoxTextChanged(EditBox *editBox, const std::string &text);
 
private:
    void buttonCallback(Object* pSender);
 
    // 文字内容两边的空白区
    int m_contentPadding;
    int m_contentPaddingTop;
	bool	isEditBox;
     
    Object* m_callbackListener;
    SEL_CallFuncND m_callback;
	void * m_pData;
 
    CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Menu*, m__pContentMenu, ContentButton);
    CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(::ui::Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);
	CC_SYNTHESIZE(::ui::EditBox*, m__pEditBox, Editbox);
 
 
};
 
 
#endif