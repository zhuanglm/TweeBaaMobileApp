#include "TouchSprite.hpp"
#include "GameWinScene.h"
#include "global_const.h"
#include "AppDelegate.h"
#include "../ZhuanPanSystem/SpinLayer.h"

using namespace cocos2d;

extern int iFirstCard;
extern int iSecondCard;
extern int iWaiting;
extern int iMaxCard;
extern int iTotalTime;

TouchSprite::TouchSprite() {
	//bIsStop=false;
}

TouchSprite::~TouchSprite() {}

bool TouchSprite::create(const std::string &filename)
{
	return true;
}

/*
void TouchSprite::setup_reset_timer()
{
	int iTag=this->getTag();
	
	if(iTag>=1001 && iTag<=1000+iMaxCard){
		log("%d ResetBackClickMe timer setup",iTag);
		this->scheduleOnce(schedule_selector(TouchSprite::ResetBackClickMe), 1.0f );
	}else{
		log("unknow tag %d at %s,%d",iTag,__FILE__,__LINE__);
	}
}

void TouchSprite::ResetBackClickMe(float dt)
{
	int iTag=this->getTag();
	
	if(iTag>=1001 && iTag<=1000+iMaxCard)
	{
		log("%d ResetBackClickMe timer run",iTag);
		this->setScale(1);
		this->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("filped.png")); 
	}
	else
	{
		log("unknow tag %d at %s,%d",iTag,__FILE__,__LINE__);
	}
	
}
*/
void TouchSprite::GameWinJump(float dt)
{

	int score = 0;
	int ttlseconds = iTotalTime;

	AppDelegate *app = (AppDelegate*)Application::getInstance();
	if(app->iGameMode==MobileGameMode::Theme_Mode)
	{
		app->GameRunRoundNext();
	}
	else
	{
		app->setPromoteProduct("");
		app->updateGameWinScore(ttlseconds);
		app->SaveGamePassLevel();//Add by Long to Save Level
		app->moveToNextGameLevel();

		auto scene = GameWin::createScene();
		TransitionScene *transition = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void TouchSprite::setup_rollback_timer(float dt)
{
	int iTag=this->getTag();
	
	if(iTag>=1001 && iTag<=1000+iMaxCard)
	{
		log("%d rollback timer setup",iTag);
		this->scheduleOnce(schedule_selector(TouchSprite::RollBackClickMe), dt );
	}else{
		log("unknow tag %d at %s,%d",iTag,__FILE__,__LINE__);
	}
}

void TouchSprite::cancel_rollback_timer()
{
	int iTag = this->getTag();

	if (iTag >= 1001 && iTag <= 1000 + iMaxCard)
	{
		log("%d rollback timer setup", iTag);
		this->unschedule(schedule_selector(TouchSprite::RollBackClickMe));
	}
	else{
		log("unknow tag %d at %s,%d", iTag, __FILE__, __LINE__);
	}
}



void TouchSprite::RollBackClickMe(float dt)
{
	int iTag=this->getTag();
	
	if(iTag>=1001 && iTag<=1000+iMaxCard)
	{
		log("%d rollback timer run",iTag);
		this->setScale(1);
		this->setTexture("res/memory_game/memory_square.png");
		//this->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("memory_square.png")); 
		if(iFirstCard==-1 && iSecondCard==-1){
			iWaiting=0;
		}
		iFirstCard = -1;
		iSecondCard = -1;
	}
	else
	{
		log("unknow tag %d at %s,%d",iTag,__FILE__,__LINE__);
	}
	
}/*
void TouchSprite::initOptions()
{
}

void TouchSprite::addEvents()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {   
        Point p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if(rect.containsPoint(p))
        {
            return true; // to indicate that we have consumed it.
        }

        return false; // we did not consume this event, pass thru.
    };

    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
		Point p = touch->getLocation();
        TouchSprite::touchEvent(touch,p);
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void TouchSprite::touchEvent(cocos2d::Touch* touch, cocos2d::Point _point)
{
    CCLOG("touched TouchSprite");
}

void TouchSprite::SetIDTag(const std::string tag)
{
}

const std::string TouchSprite::getIDTag()
{
return this->sIDTag;
}

void TouchSprite::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
if (!response)
{
GameWinJump();
return;
}

// You can get original request type from: response->request->reqType
if (0 != strlen(response->getHttpRequest()->getTag()))
{
log("%s completed", response->getHttpRequest()->getTag());
}
int statusCode = response->getResponseCode();
char statusString[64] = {};
sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
// _labelStatusCode->setString(statusString);
log("response code: %d", statusCode);
if (!response->isSucceed())
{
log("response failed");
log("error buffer: %s", response->getErrorBuffer());
GameWinJump();
return;
}

GameWinJump();

}
void TouchSprite::Move2WinSceneTimer(float dt){
HttpRequest* request = new HttpRequest();
request->setUrl(UCAPIWebPostURL);
request->setRequestType(HttpRequest::Type::POST);
request->setResponseCallback(CC_CALLBACK_2(TouchSprite::onHttpRequestCompleted, this));

// write the post data
std::string s = CCUserDefault::sharedUserDefault()->getStringForKey("UserGuid");
char p[10]; char p2[20];
sprintf(p, "%d", MobileGameType::Memory_Match);
sprintf(p2, "%d", thetime);
std::string sPost = "action=save_ranking&username=" + s + "&gametype=" + p + "&score=" + p2;

const char* postData = sPost.c_str();
request->setRequestData(postData, strlen(postData));
request->setTag("save_ranking");
cocos2d::network::HttpClient::getInstance()->send(request);
request->release();
}


void TouchSprite::Move2WinScene()
{

//this->scheduleOnce(schedule_selector(TouchSprite::Move2WinSceneTimer), 1.0f );
GameWinJump
}


*/