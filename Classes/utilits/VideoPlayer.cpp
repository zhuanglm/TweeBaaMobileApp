#include "VideoPlayer.h"
#include "GameWinScene.h"
#include "AppDelegate.h"
#include "TwbIntroScene.h"
USING_NS_CC;
char* _sVideoName = "\0";
int _iMode = 0;//0:intro video, other:Jigsaw game

Scene* VideoPlayer::createScene(char* sVideoName, int iMode)
{
	auto scene = Scene::create();
	auto layer = VideoPlayer::create();
	scene->addChild(layer);
	//sprintf(sVideoName, sVN);
	_sVideoName = sVideoName;
	_iMode = iMode;
	return scene;
}
bool VideoPlayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

void VideoPlayer::onEnter(){
	Layer::onEnter();
	//char *sName = "product_videos/KittyLitty.mp4";//

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		showVideo(_sVideoName);
	#else
		schedule(schedule_selector(VideoPlayer::ScheShow), 2.0f);//waiting for finishing creating this scene
	#endif
}
void VideoPlayer::ScheShow(float dt)
{
	this->unschedule(schedule_selector(VideoPlayer::ScheShow));
	showVideo(_sVideoName);
}
void VideoPlayer::showVideo(const char *sName){

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		Size size = Director::getInstance()->getVisibleSize();
		auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
		videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
		videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		videoPlayer->setContentSize(Size(size.width, size.height));
		videoPlayer->setFullScreenEnabled(false);
		this->addChild(videoPlayer);
		if (videoPlayer)
		{
			videoPlayer->setFileName(sName);
			videoPlayer->play();
		}
		videoPlayer->addEventListener(CC_CALLBACK_2(VideoPlayer::videoEventCallback, this));
	#else
	videoPlayOverCallback();
	#endif
}

/**
* 视频播放完成的回调函数
*/
void VideoPlayer::videoPlayOverCallback()
{
	/*
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);*/

	//game win
	if (_iMode == 0)//intro video
	{
		//auto introscene = TwbIntroScene::create();
		//auto introscene = Login::createScene();
		//TransitionScene *transition = TransitionFade::create(0.1f, introscene);
		//Director::getInstance()->replaceScene(transition);
		this->cleanup();
		Director::getInstance()->popScene();
	}
	else
	{
		AppDelegate *app = (AppDelegate*)Application::getInstance();
		if (app->iGameMode == MobileGameMode::Theme_Mode)
		{
			app->GameRunRoundNext();
		}
		else
		{
			int score = 5;
			app->UpdateUserScore(1, score);
			app->SaveGamePassLevel();//Add by Long to Save Level
			//app->saveGameLevelPassedInfo();
			app->moveToNextGameLevel();
			app->setReward(score,1);

			auto scene = GameWin::createScene();
			TransitionScene *transition = TransitionFade::create(1, scene);
			Director::getInstance()->replaceScene(transition);
		}

	}
}
/**
*  视频播放的状态
*  注意这里的代码，此处代码只有在android平台和Ios平台有效
*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void VideoPlayer::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
	switch (eventType) {
	case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
		videoPlayOverCallback();
		break;
	default:
		break;
	}
}
#endif