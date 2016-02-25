#ifndef __VIDEO_PLAYER_H__
#define __VIDEO_PLAYER_H__

#include "cocos2d.h"
//�����������2��.h�ļ�
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;


class VideoPlayer : public Layer
{
public:
	static Scene* createScene(char* sVideoName, int iMode);

	virtual bool init();

	void onEnter();

	void videoPlayOverCallback();

	void showVideo(const char *sName);

	void ScheShow(float dt);

	/**
	* ��Ƶ����״̬��ֻ����android��iosƽ̨��Ч
	*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif

	CREATE_FUNC(VideoPlayer);
};
#endif 