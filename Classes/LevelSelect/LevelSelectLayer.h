//
//  LevelSelectLayer.h
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#ifndef __PageTest__LevelSelectLayer__
#define __PageTest__LevelSelectLayer__

#include "cocos2d.h"
#include "LevelSelectContent.h"
USING_NS_CC;

const int LevelSelectTopCoinTag = 1;

class LevelSelectLayer :public Layer{
    
public:
    LevelSelectLayer();
    ~LevelSelectLayer();
    virtual bool init();
    CREATE_FUNC(LevelSelectLayer);
    static cocos2d::Scene* createScene();
private:
	void menuCallback(Ref *sender);
    LevelSelectContent* levelSelectContent;
    int _currentPage;
    int _maxPage;
    Menu* _leftMenu;
    Menu* _rightMenu;
    
    void initNavigation();
    void initAllLevels();
    void nextPageBack(Ref* sender);
    void prePageBack(Ref* sender);
};

#endif /* defined(__PageTest__LevelSelectLayer__) */
