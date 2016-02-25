//===========Tweebaa==============
//GetImage.h	--Dec 2 2015	
//Version 1.0 by Raymond Zhuang at Toronto
// last modified at

#ifndef _GETIMAGE_H_
#define _GETIMAGE_H_

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif	// CC_PLATFORM_ANDROID

/**
 * 图像选择器
 */
class GetImage
{
public:
    static GetImage* getInstance();

    static void destoryInstance();
public:
    // 显示本地相册与相机选择器
    void callGetImageWithPhotoAndCamera(const std::function<void(std::string)>& callback);
    
    void setListener(const std::function<void(std::string)>& callback);

    void removeListener();
    
    // 打开相册
    void openPhoto();
    
    // 打开相机
    void openCamera();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // 设置AppController
    void setViewController(void* viewController);
#endif // CC_PLATFORM_IOS

protected:
    // 初始化
    bool init();
    
    GetImage();
    
protected:
    std::function<void(std::string)> m_callback;
    static GetImage* s_instance;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void* m_viewController;
#endif // CC_PLATFORM_IOS
    
};

#endif // _GETIMAGE_H_