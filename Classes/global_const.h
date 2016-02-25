
#ifndef __GLOBAL_CONST_H__
#define __GLOBAL_CONST_H__





#define UCAPIWebPostURL "http://67.224.94.82/mobileApp/UCAPI.ashx"
//#define UCAPIWebPostURL "http://localhost:54023/mobileApp/UCAPI.ashx"

#define PromotionWebGetURL "http://67.224.94.82/mobileApp/Promotion.ashx"
#define MemberCenterURL "http://67.224.94.82/Home/Index.aspx"
#define CollageFilePostURL "http://67.224.94.82/mobileApp/CollageFileAPI.ashx"
#define CollageAPIURL "http://67.224.94.82/mobileApp/CollageAPI.ashx"
#define GameAPIURL  "http://67.224.94.82/mobileApp/GameAPI.ashx"
//#define GameAPIURL  "http://localhost:54023/mobileApp/GameAPI.ashx"
/*
#define UCAPIWebPostURL "http://localhost:2339/mobileApp/UCAPI.ashx"

#define PromotionWebGetURL "http://localhost:2339/mobileApp/Promotion.ashx"
#define MemberCenterURL "http://67.224.94.82/Home/Index.aspx"
#define CollageFilePostURL "http://localhost:2339/mobileApp/CollageFileAPI.ashx"
*/

#define VersionCheckURL "https://67.224.94.82/mobileApp/version.txt"
#define iCurrentVersion  1

#define PATH_PROMOTION_PRODUCT "res/PromotionProduct.plist"
#define ACTION_BUYNOW "&action=BuyNow"
#define ACTION_SHARE "&action=Share"
#define MOREINFO_URL "https://www.tweebaa.com/page.aspx?id=2"
#define TERM_URL "https://www.tweebaa.com/College/Genernal_Terms_ofUse.aspx"

 enum MobileGameType{
	Text_Theme_Info =-2,
	Random_Game = -1,
	Collage_mode =0,
	Find_difference= 1,
    Zoom_game=2,
	Pixel_Shuffle= 3,
	Memory_Match = 4,
	Price_Right =5,
	Sort_Price =6
};

 enum MobileGameMode{
	Collage_Mode=1,
	Game_Mode =2,
	Theme_Mode=3
};

 enum BGMusic_flag{
	 OFF = 0,
	 ON = 1,
};



const char PlayerPassLevelCountKey[][40] = {"","PlayerPass_Find_difference_Key","PlayerPass_Zoom_game_Key",
										  "PlayerPass_Pixel_Shuffle_Key","PlayerPass_Memory_Match_Key",
										  "PlayerPass_Price_Right_Key","PlayerPass_Sort_Price_Key"};
const char LevelInfoKey[][40] = {"","LevelPass_Find_difference_Key","LevelPass_Zoom_game_Key",
										  "LevelPass_Pixel_Shuffle_Key","LevelPass_Memory_Match_Key",
										  "LevelPass_Price_Right_Key","LevelPass_Sort_Price_Key"};
const char TotalPassLevelCountKey[][40] = {"","TotalPassLevel_Find_difference_Key","TotalPassLevel_Zoom_game_Key",
										  "TotalPassLevel_Pixel_Shuffle_Key","TotalPassLevel_Memory_Match_Key",
										  "TotalPassLevel_Price_Right_Key","TotalPassLevel_Sort_Price_Key"};
const char DailyScoreCountKey[][40] = {"","DailyScore_Spot_Key","DailyScore_Zoom_Key",
										  "DailyScore_Shuffle_Key","DailyScore_Flip_Key",
										  "DailyScore_Match_Key","TotalPassLevel_Link_Key"};

const char TutorialIsFirstTime[][40] = {"","Tutorial_IsFirst_Spot","Tutorial_IsFirst_Zoom",
										  "Tutorial_IsFirst_Shuffle","Tutorial_IsFirst_Flip",
										  "Tutorial_IsFirst_Match","Tutorial_IsFirst_Link"};

const char TutorialStory[][40] = {"Tutorial_IsFirst_Christmas"};
#define GAMG_COUNT 6
/*const char sAnimation[GAMG_COUNT][100] = { "res/animations/Spot/a.ExportJson",
									"res/animations/Shuffle/c.ExportJson",
									"res/animations/Flip/d.ExportJson",
									"res/animations/Match/e.ExportJson",
									"res/animations/Zoom/b.ExportJson",
									"res/animations/Link/f.ExportJson" };

const char sAniName[GAMG_COUNT][10] = { "a", "c", "d", "e", "b", "f" };*/


//Game Max Level
const float g_maxLevel[7] = {0,23,25,20,20,20,20};

extern int m_iCurrentGameID ;
extern int m_iCurrentGameLevel;
extern long m_iUserTotalScore;
extern int m_Refresh;
extern int m_UserID;

extern int m_iBGMFlag;
extern int m_iShowRegister;

//extern int m_ThemeID;
//extern int m_RoundID;
//extern int m_StepID;

static bool m_DebugFlag = false;

static int TIMER_LABEL_TAG = 21;
static int thetime = 0;
static int nAniFlag = 1;


//Points increaing after win a level
static int WIN_POINT_LOW = 1;
static int WIN_POINT_MED = 3;
static int WIN_POINT_HIG = 5;
static int WIN_POINT_0 = 0;
static int WIN_POINT_1 = 1;
static int WIN_POINT_2 = 2;
static int WIN_POINT_3 = 3;
static int WIN_POINT_5 = 5;
static int WIN_POINT_7 = 7;
static int WIN_POINT_9 = 9;

static int DAILY_SCORE_LIMIT = 20;
static int PUBLISH_SCORE_LIMIT = 50;
static int SHARE_SCORE_LIMIT = 20;

typedef struct PromoteProduct
{
	std::string m_strProductGUID;
	std::string m_strProductName;
	std::string m_strImage;
	std::string m_strProductURL;
	int m_iIsLoad;
	int m_nCurrid;
};


//static char jigSawFile[100];

//digits
#define LETTER_SPACE 32
#define LETTER_NUM_0 48
#define LETTER_NUM_1 49
#define LETTER_NUM_2 50
#define LETTER_NUM_3 51
#define LETTER_NUM_4 52
#define LETTER_NUM_5 53
#define LETTER_NUM_6 54
#define LETTER_NUM_7 55
#define LETTER_NUM_8 56
#define LETTER_NUM_9 57
//Alphabets
#define LETTER_TAG_A 65
#define LETTER_TAG_B 66
#define LETTER_TAG_C 67
#define LETTER_TAG_D 68
#define LETTER_TAG_E 69
#define LETTER_TAG_F 70
#define LETTER_TAG_G 71
#define LETTER_TAG_H 72
#define LETTER_TAG_I 73
#define LETTER_TAG_J 74
#define LETTER_TAG_K 75
#define LETTER_TAG_L 76
#define LETTER_TAG_M 77
#define LETTER_TAG_N 78
#define LETTER_TAG_O 79
#define LETTER_TAG_P 80
#define LETTER_TAG_Q 81
#define LETTER_TAG_R 82
#define LETTER_TAG_S 83
#define LETTER_TAG_T 84
#define LETTER_TAG_U 85
#define LETTER_TAG_V 86
#define LETTER_TAG_W 87
#define LETTER_TAG_X 88
#define LETTER_TAG_Y 89
#define LETTER_TAG_Z 90



//#define BGM_GAME "res/Media/Game-background-2-edited.mp3"
//#define BGM_MAIN "res/Media/MainBackground.mp3"
//For demo purpose, test 2 picked bgm
#define BGM_GAME "res/Media/Game_Background.mp3"
#define BGM_MAIN "res/Media/Main_Background.mp3"
#define BGM_THEME "res/Media/MerryChristmas.mp3"
#define BGM_COLLAGE "res/Media/Collage_Background.mp3"
#define SOUND_BUTTON_CLICK "res/Media/coral-island.mp3"
#define SOUND_TOOL_CLICK "res/Media/tool_button.mp3"
#define SOUND_GAME_PASS "res/Media/pass_level.mp3"
#define SOUND_DAILYSPIN "res/Media/daily_spin.mp3"
#define SOUND_DAILYSPIN_LONG "res/Media/daily_spin_long.mp3"
#define SOUND_TRY_AGAIN "res/Media/Try Again.mp3"
#define SOUND_ANIMATION "res/Media/Animation sound.mp3"
#define SOUND_WIN "res/Media/LevelCompleted.mp3"

#define Cache_Path		"Cache/"
#define DB_File		"Collage.db3"

#define MSG_NETWORK_NONETWORK "Collage Mode will not load until you connect to internet/Wifi.In the meantime, you can still play games and earn!"
#define MSG_NETWORK_3G "You are not connected to Wi-Fi.  If you proceed, data rates may apply.  Do you want to proceed?"
#define MSG_QUIT_TITLE "QUIT GAME"
#define MSG_QUIT_MSG "Are you sure to quit the game? You will lose your current progressing."
#define MSG_REGISTER_MSG "Create an account NOW and receive 50 bonus points! Checkout more awesome features at www.tweebaa.com."
#define MSG_PUBLISHED_MSG " This design was PUBLISHED already!"
#define MSG_SAVED_MSG " This design was SAVED already!"
#define MSG_SAVE_DRAFT_MSG " This design will be saved as draft!\n You may load it from [MY Profile]"
#define MSG_LOAD_DRAFT_MSG " This design will be loaded to edit!"
#define MSG_ERROR_GEN "Oops, it looks like you found an Error."
#define MSG_WARN_CLR "Are you sure to delete all the items?"
#define MSG_WARN_Del "Are you sure to delete this item?"
#define MSG_WARN_ITEM	"Too many items have been added!"
#define MSG_WARN_QUIT	"Are you sure you want to quit the Collage?\nYou will lose any un-published design."
#define MSG_WARN_NORESULT	"No item for %s, Please try another words."
#define MSG_ERROR_NETWORK	"Check your network connection and please try again later.\n"
#define MSGBOX_TAG 9000
#define MSGBOX_TITLE "Tweebaa App"

#define VIDEO_KITTYLITTY "product_videos/KittyLitty.mp4"

#define VIDEO_INTRO "product_videos/Logo video1.mp4"

#define VERSION "Ver 1.5.0"



#endif
