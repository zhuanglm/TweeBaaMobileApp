//===========Tweebaa==============
//Collage_Template.h	--Jun 23 2015	
//Version 1.1 by Raymond Zhuang at Toronto
// last modified at Nov 17
#pragma once
#include <string>
using namespace std;

#include "cocos2d.h"
using namespace cocos2d;
#include "ui/CocosGUI.h"

#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

extern "C"
{
	#include "sqlite/sqlite3.h"
};

#include <vector>
//#include <codecvt>
#include "define.h"
#include "global_const.h"

extern string	TEMP_filePath,G_UNM,G_UID,G_Score,G_Level,G_Percentage;
extern float	G_fP;

typedef struct TemplateList
{
	int		id;
	char	uid[64];
	char strListImage[128];
	char sTitle[64];			
	char sFourthEle[32];		//product : price ; design : inspiration
	char sFifthEle[256];		//product : description ; design : username
	char sSixthEle[32];		//product :  ; design : likecount
	char sSeventhEle[16];		//product :  ; design : country
	char sEighthEle[8];		//product :  ; design : sharecount
	//int	 nImageSize;
}sctTempList;

class CollageTemplateList : public Object
{
	private:
		

	public:
		vector <sctTempList>	vTemplate;
		CollageTemplateList(void);
		~CollageTemplateList(void);

		static string GetElementFromXML(const char *fileName,const string element);
		static CollageTemplateList * ParseXMLWithfile(const char *fileName);
		static CollageTemplateList * ParseXMLWithstring(const char *content);
		static CollageTemplateList * LoadXMLFromDB(const char *db_file,const char *table,const char * sort);
		static CollageTemplateList * LoadXMLFromDB(const char *db_file,const char *table,const char *field,const char *key);
		//char* getString(const char *key);

		static void WStrToUTF8(std::string& dest, const std::wstring& src);
		static void Str2SQL(char *);
		static std::string WStrToUTF8(const std::wstring& str);
		static std::wstring s2ws(const std::string& s);
		static sqlite3* initDB(const char*);
};

class BrowsePage : public cocos2d::ui::PageView
{
	public:
		void setTouchDirection(PageView::TouchDirection);
		void setCurPageIndex(size_t);

		virtual void interceptTouchEvent(TouchEventType event, Widget* sender,Touch *touch) override;
};

