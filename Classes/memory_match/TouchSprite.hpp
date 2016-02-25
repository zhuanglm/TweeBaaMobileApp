#ifndef _TOUCSPRITE_HPP_
#define _TOUCSPRITE_HPP_

#include "cocos2d.h"
#include "network/HttpClient.h"
using namespace cocos2d::network;

class TouchSprite : public cocos2d::Sprite
{
private:
		const std::string sIDTag;
    public:
		//bool bIsStop;
        TouchSprite();
        ~TouchSprite();
        bool create(const std::string &filename);

       // void initOptions();

//		bool CheckCard(int iFrst,int iSecond,Vector<Sprite*> spCard);
        //void addEvents();
        //void touchEvent(cocos2d::Touch* touch, cocos2d::Point _p);
		void setup_rollback_timer(float dt=1);
		//void setup_reset_timer();
		void cancel_rollback_timer();
		void RollBackClickMe(float dt);
		//void ResetBackClickMe(float dt);
		//void Move2WinScene();
		//void Move2WinSceneTimer(float dt);
		//void SetIDTag(const std::string tag);
		//const std::string getIDTag();
		void GameWinJump(float dt);
    private:
		//void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};

#endif // _TOUCSPRITE_HPP_