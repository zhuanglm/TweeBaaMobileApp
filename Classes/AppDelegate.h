#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#include "C2DXShareSDK/C2DXShareSDK.h"
#include "tinyxml2/tinyxml2.h"
#include "network/HttpClient.h"
//#include "theme/GameRound.h"
#include "utilits/NetworkUtil.h"

#include "global_const.h"
#include "DataAccess/DataAccess.h"
#include <vector>
using namespace tinyxml2;
using namespace cocos2d::network;

#define UPDATE_SCORE_PUBLISH 0
#define UPDATE_SCORE_SHARE 1

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/

typedef struct _GameRound
{
	int round_id;
	int round_step;
	int round_type;
	std::string round_info;
	std::string round_effect;
}stGameRound;

class  AppDelegate : private cocos2d::Application
{
	private:
    //初始化平台配置
    void initPlatformConfig();
	void GameJump(int i);

	cocos2d::Size screenSize;
public:
	void SaveUserToolsDiff(int iScore);
	int GetToolsDiff();
	std::string GetAccessString();
	std::string string_to_hex(const std::string& input);
	std::string random_string(size_t length);
	std::string encode_data_for_http(std::string strSource);
	std::string base64_xxtea_encode(std::string strSource);
	std::string base64_xxtea_decode(std::string strSource);

	//game theme
	short iGameMode;
	int iCurrentStep;
	int iRound;
	int iThemeID;
	std::string m_strThemeImg;
	std::string m_strLangCode;

	//XMLElement *rootRound;
	vector<stGameRound> sp_GameRound;
	long GetUserTotalScore();
	void UpdateUserScore(int iFlag, int iScore);
	int GetUserLevel();
	float GetPercentToNextLevel();


	NetworkType GetNetWorkStatus();
	bool StartBGMusic(const char* pAudiofile);
	bool PlayEffect(const char* pAudiofile);
	bool StopBGMusic();
	bool getBGMstatus();

	//bool initGameLevelPassedInfo();	
	//bool saveGameLevelPassedInfo();
	//int getGameLevelPassedInfo();
	int getCurrentGameLevel();
	int getTotalPassedLevelInfo();
	int getToolsNumberInfo();
	bool setToolsNumberInfo(int nToolNum);
	void moveToNextGameLevel();

	std::string GetUserGuid();
	//void SetUserGuid(string strUserGuid);

	long GetUserScoreDiff();
	void SaveUserScoreDiff(int iScore);
	std::string GetLayoutString();
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	void SaveGamePassLevel();
	void LogAccess(std::string m_strLayerName);
	void GameRunRoundNext();
	void StartTheme();

	void setReward(int iScore,int iStar);
	int getReward();
	int getStar();

	void SyncPoint();
	void onSyncCompleted(HttpClient *sender, HttpResponse *response);
	
	void ResetDailyScore();
	void updateGameWinScore(int nSec);
	bool initGameDefault(); 
	void updateCollageScore(int nMode);

	PromoteProduct getPromoteProduct(); //return promotion product 
	bool setPromoteProduct(std::string strSKU); //set promotion product in each game

	AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};


#endif // _APP_DELEGATE_H_

