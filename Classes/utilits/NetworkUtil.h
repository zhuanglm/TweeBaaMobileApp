#ifndef __NETWORKUTIL_H__
#define __NETWORKUTIL_H__

#include "cocos2d.h"
//#include "network/HttpClient.h"
using namespace cocos2d;
//using namespace CocosDenshion;
//using namespace cocos2d::network;
//#include "CCAlert.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

typedef enum
{
    NetworkTypeNone = 0,
    NetworkType3G = 1,
    NetworkTypeWifi = 2
}NetworkType;

class NetworkUtil
{
public:
    static bool isNetworkAvaible();
    static NetworkType getNetworkType();
    static void ShowNetworkType();
};

#endif // __NETWORKUTIL_H__
