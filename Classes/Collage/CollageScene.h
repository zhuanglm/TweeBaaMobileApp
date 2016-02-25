//===========Tweebaa==============
//CollageScenen.h	--Jun 23 2015	
//Version 1.1 by Raymond Zhuang at Toronto
// last modified at Nov 17

#ifndef __COLLAGE_SCENE_H__
#define __COLLAGE_SCENE_H__

#include "cocos2d.h"
#include "MainMenu.h"
#include "global_const.h"

//#include "Gif/GIFMovie.h"
//#include "Gif/CacheGif.h"
//#include "Gif/InstantGif.h"
//#include "cocostudio/CCSSceneReader.h"

USING_NS_CC;

class CollageScene : public Layer
{
	private:
		CollageTemplateList *pCTL;	//list from xml
		sqlite3*	pDB;
		
		ProgressTimer *ct;
		Text	*textPercentage;
		//string	strFileName[MAX_CacheFile];
		unsigned int Number_Resources;
		unsigned int	nErrorFlag;		//0: no error
		string		sErrorMsg;
		unsigned int nTime;

		std::mutex	updateMutex;
		
		void	loadingTutorial(float dt);
		void	loadingCallBack(Ref* pSender);
		void	LoadResources(void);

		void	popupLayer(Node* baseLayer,Vec2 popPos,const char *sTitle,string sContent);
		void	popupCallback(cocos2d::Node *pNode,void*);
		bool	CreateFolder(const char *path);

	public:
		CollageScene(void);
		~CollageScene(void);
        //void LoadXMLFiles(float dt);

		MainMenu *mainMenu;
		HttpConnection * http_connect;
		TutorialScene* Tutorial_Scene;

		static Vec2 posLeftMenu;
		static Vec2 posRightMenu;

		// there's no 'id' in cpp, so we recommend returning the class instance pointer
		static Scene* createScene();
		
		void WaitSpin(void);

		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
		virtual bool init();

		// implement the "static create()" method manually
		CREATE_FUNC(CollageScene);
};

#endif // __COLLAGE_SCENE_H__
