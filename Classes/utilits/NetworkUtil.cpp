//#include "cocos.h"
#include "NetWorkUtil.h"




#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#import <SystemConfiguration/SystemConfiguration.h>
#include "NetworkDection.h"

#endif

bool NetworkUtil::isNetworkAvaible()
{
	bool isExistenceNetwork;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    isExistenceNetwork = NetworkDection::isNetworkAvaible();
   /*
    Reachability* r = [Reachability reachabilityWithHostName:@"www.google.com"];
    
    switch ([r currentReachabilityStatus]) {
        case NotReachable:
            isExistenceNetwork = false;
            break;
        case ReachableViaWWAN:
            isExistenceNetwork = true;
            break;
        case ReachableViaWiFi:
            isExistenceNetwork = true;
            break;
    }*/
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#endif
    return isExistenceNetwork;
}

NetworkType NetworkUtil::getNetworkType()
{
    NetworkType type = NetworkTypeNone;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    int iType= NetworkDection::getNetworkType();
    switch (iType) {
        case 0:
            type = NetworkTypeNone;
            break;
        case 1:
            type = NetworkType3G;
            break;
        case 2:
            type = NetworkTypeWifi;
            break;
    }
    /*
     NetworkTypeNone = 0,
     NetworkType3G = 1,
     NetworkTypeWifi = 2
    Reachability* r = [Reachability reachabilityWithHostName:@"www.google.com"];
    
    switch ([r currentReachabilityStatus]) {
        case NotReachable:
            type = NetworkTypeNone;
            break;
        case ReachableViaWWAN:
            type = NetworkType3G;
            break;
        case ReachableViaWiFi:
            type = NetworkTypeWifi;
            break;
     */
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		log("NetworkUtil::CheckConnectivity(): *****CC_PLATFORM_ANDROID*****");
        cocos2d::JniMethodInfo methodInfo;

        if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/CheckConnectivity", "checkConnectivity", "()I")) {
			log("NetworkUtil::CheckConnectivity(): getStaticMethodInfo failed");
			return type;
        }

        jint jRet = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
		if (jRet == 1){
			type = NetworkType3G;
			log("NetworkUtil::CheckConnectivity(): type = NetworkType3G");
			
		}
		else if (jRet == 2){
			type = NetworkTypeWifi;
			log("NetworkUtil::CheckConnectivity(): type = NetworkTypeWifi");
		}
		else {
			type = NetworkTypeNone;
			log("NetworkUtil::CheckConnectivity(): type = NetworkTypeNone");
		}
		log("NetworkUtil::CheckConnectivity(): type = %d", type);
#endif
    return type;
}
void NetworkUtil::ShowNetworkType(){
	

	NetworkType type = NetworkUtil::getNetworkType();

	switch (type)
	{
		case NetworkTypeNone:
			MessageBox("No Connection!","Network Connection");
			break;
		case NetworkType3G:
			MessageBox("3G !","Network Connection");
			break;
		case NetworkTypeWifi:
			MessageBox("Wifi !","Network Connection");
			break;
		default:
			MessageBox("Default: No Connection!","Network Connection");
			break;
	}
}

