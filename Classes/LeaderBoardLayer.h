#pragma once

//#include "memory_match/memory_match_AtlasLoader.h"
#include "SimpleAudioEngine.h"
//#include "CCMenuItem.h"
//#include "GameScene.h"
#include "time.h"
#include "cocos2d.h"
//#include "BirdSprite.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "Stage.h"
#include "cocos-ext.h"
#include "LeaderBoard/LeaderBoard.h"


using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::network;

USING_NS_CC;
USING_NS_CC_EXT;

enum Table
{
	Table_Left = 0,
	Table_Center,
	Table_Right
};


class LeaderBoardLayer : public Layer, TableViewDataSource{
public:
	static cocos2d::Scene* createScene();
	LeaderBoardLayer();
	~LeaderBoardLayer(void);
	virtual bool init();
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	// implement the "static create()" method manually
	//CREATE_FUNC(LeaderBoardLayer);
    
private:
	void touchButton(Ref* obj,::ui::Widget::TouchEventType eventType) ;
	//void checkNetwork();
	//void menuCallbackPopupNo(Ref *sender);
	//void menuCallbackPopupYes(Ref *sender);

	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	//static size_t writeData(uint8_t* ptr,size_t size,size_t nmemb,void *stream); 

	cocos2d::Node* _rootNode;
	cocos2d::Node* _popNode;

	TableView * m_leftTable;
	Vector<LeaderBoard*> leader;
protected:
    virtual void initialize();
};