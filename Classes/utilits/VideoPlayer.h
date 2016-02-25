#ifndef __VIDEO_PLAYER_H__
#define __VIDEO_PLAYER_H__

#include "cocos2d.h"
//务必引入以下2个.h文件
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
	* 视频播放状态，只有在android和ios平台有效
	*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif

	CREATE_FUNC(VideoPlayer);
};
#endif 