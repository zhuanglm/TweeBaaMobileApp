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
 * ͼ��ѡ����
 */
class GetImage
{
public:
    static GetImage* getInstance();

    static void destoryInstance();
public:
    // ��ʾ������������ѡ����
    void callGetImageWithPhotoAndCamera(const std::function<void(std::string)>& callback);
    
    void setListener(const std::function<void(std::string)>& callback);

    void removeListener();
    
    // �����
    void openPhoto();
    
    // �����
    void openCamera();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // ����AppController
    void setViewController(void* viewController);
#endif // CC_PLATFORM_IOS

protected:
    // ��ʼ��
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