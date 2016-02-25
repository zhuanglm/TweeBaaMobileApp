//
//  GameConstants.h
//  PageTest
//
//  Created by wx5 on 12/31/14.
//
//

#ifndef __PageTest__GameConstants__
#define __PageTest__GameConstants__

#ifndef PageTest_GameConstants_h
#define PageTest_GameConstants_h

typedef enum{
    kAlreadyPass,
    kNotPassYet,
    kLockLevel
}LevelItemType;





//每一页的关卡数量
const int g_EachPageCount = 15;
//每一行的关卡数量
const float g_EachLineCount = 5;
const int levelMarginX = 113;
const int levelMarginY = 134;
extern int g_passLevelCount;
//extern int g_currentLevel;


#endif

#endif /* defined(__PageTest__GameConstants__) */
