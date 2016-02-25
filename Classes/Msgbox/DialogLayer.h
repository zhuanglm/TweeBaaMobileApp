#ifndef _DIALOGLAYER_H_
#define _DIALOGLAYER_H_

//#include "../memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
#include "time.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "WelcomeScene.h"
#include "GameRoundWinScene.h"
#include "GameWinScene.h"
#include "GameFailedScene.h"
//#include "global_const.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace ui;

#define UINODE_NAME_CSB "MsgBox/MsgBoxScene.csb"
#define UI_NODE 1001

// 原理：
// 模态对话框 能够拦截所有下层的消息
// cocos2d-x 消息处理优先级【-128,128】越小优先级越高
// 菜单kCCMenuHandlerPriority优先级 -128 要想拦截所有消息(包括菜单) 
// 必须将我们的模态对话框优先级<-128 但是也意味着拦截了当前模态对话框的菜单消息(-128)
// 我们需要手动将消息传给模态对话框上的按钮进行处理
// 使用：1.重载OnInitDialog将需要的控件添加到dlg上
//       2.需要响应事件的按钮使用pushMenu添加
//       3.派生类需要实现宏CREATE_FUNC(class_name)

class DialogLayer: public CCLayerColor
{
public:
    DialogLayer();
    virtual ~DialogLayer();
	
public:
    /** 初始化对话框 */
	virtual bool onInitDialog() = 0;

    /** 所有的菜单按钮压入到容器中 */
    void pushMenu(cocos2d::MenuItem *pMenu);

    virtual bool init();


    virtual void onEnter();
    virtual void onExit();

    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

private:
    /** 初始化菜单 将菜单添加到模态对话框上*/
    bool initMenu();

private:
    CCMenu *mMenu;             // 模态对话框菜单
	//CCArray *mMenuItemArray;   // 所有菜单
	cocos2d::Vector<cocos2d::MenuItem*> mMenuItemArray;
    bool mTouchedMenu;                  // 记录菜单点击
	cocos2d::Node* _UINode;
};
#endif
