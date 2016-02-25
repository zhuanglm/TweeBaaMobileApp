#pragma once

//#include "memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
//#include "GameScene.h"
#include "time.h"
#include "cocos2d.h"
//#include "BirdSprite.h"
#include "ui/CocosGUI.h"
#include "PriceMatchQuestion.h"
#include "network/HttpClient.h"

#include "CCAlert.h"


using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::network;
static int SequenceA[3] = {3,2,4};//c,b,d
static int SequenceB[3] = {1,4,3};//a,d,c
static int SequenceC[3] = {2,1,4};//b,a,d
static int SequenceD[3] = {3,2,1};//c,b,a

class PriceMatchLayer : public Layer{
public:
	 
	Vector<PriceMatchQuestion*> vecQuestion;
	PriceMatchQuestion* Current_Question;
	int m_iTotalQuestion;

	void GameWinJump();
	void GameWin(float dt);
	void UseTips(int iSequence);
	void DisableImg(int iButton);
	void DisableButton(int iButton);
	int m_iCurrentIndex;
	//void NextQuestion();
	void GameFailed(float dt);
	PriceMatchLayer(void);
	~PriceMatchLayer(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(PriceMatchLayer); 
	bool _bTlEnabled;
	void showDeductPointEffect(std::string sLabel);
	void enableTools(float dt);
	void PlayAnimation(float dt);
    
private:
	cocos2d::Node* _rootNode;
	//void menumobile_Callback(Ref *sender);
	//void menuvoice_Callback(Ref *sender);
	void updateTimeLabel(float dt);
	//void menuCallback(Ref *sender); 
    void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType);
	void menuCallbackOptionA(Ref *sender);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	/**
     * This method is make the land have a scroll animation
     */
	//void scrollLand(float dt);
	void switchUI();
	void yesCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void noCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateLabels(float dt);
	void loadFirstTutorial(float dt);


	int _nTlcount; //totol tips can be used
	int _nTlimit; //tips can be used in this level
	int _nLvl; //Lvl
	float _fPerToNLvl; //Percent to next level
	long _lTtlScore;

};