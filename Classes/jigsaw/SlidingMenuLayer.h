#pragma once
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

//#include "cocos-ext.h" 
USING_NS_CC; 
//using namespace cocos2d::extension;
//USING_NS_CC_EXT;

#include "ui/CocosGUI.h"
/*
#include "cocos-ext.h"
using namespace cocos2d::extension;
*/
static  int SCROLLVIEW_TAG = 300;
class SlidingMenuLayer :public Layer {
private:
	float generalscalefactor;
	/*
	private static CGSize screenSize;
	CCScrollView scrollView;
	CCBitmapFontAtlas statusLabel ;
	private CGPoint startlocation; //keep track of touch starting point
	private CGPoint endlocation;   ////keep track of touch ending point
	float tilescale ;

	 = 0.0f ;
	*/

	
public:
	SlidingMenuLayer(void);
	~SlidingMenuLayer(void);
	virtual bool init();
	CREATE_FUNC(SlidingMenuLayer);
	//Controls the events launched when a menu item is clicked
	void launchmenu( int i);
	void LevelChoose();
	void menuCallback(Ref *sender);
	virtual bool TouchesBegan(Touch *pTouch, Event *pEvent) ;

	virtual bool TouchesEnded(Touch *pTouch, Event *pEvent);
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);

};

