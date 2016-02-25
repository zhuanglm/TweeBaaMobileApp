#pragma once

#include <string>
using namespace std;

#include "cocos2d.h"
using namespace cocos2d;
#include "ui/CocosGUI.h"
#include "AppDelegate.h" 

extern "C"
{
#include "sqlite/sqlite3.h"
}; 

#define THEME_INIT_FILE "res/theme/initTheme.txt"
#define PRODUCT_INFO_FILE "res/productInfo_v3.sql"

//-------------------------------TABLE STRUCT--------------------------------------------
/*
//----------USERINFO----------
CREATE TABLE  IF NOT EXISTS `USERINFO` 
(
	`USERID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`USERGUID` TEXT NOT NULL UNIQUE,
	`USERNAME` TEXT NOT NULL UNIQUE,
	`PASSWD` TEXT NOT NULL
); 
//----------PARAM-------------
CREATE TABLE IF NOT EXISTS `PARAM` 
(
	`ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	`KEYNAME` TEXT NOT NULL UNIQUE
);
//----------PARAMVALUE--------
CREATE TABLE IF NOT EXISTS `PARAMVALUE` 
(
	`USERID` INTEGER NOT NULL,
	`ID` INTEGER NOT NULL,
	`VALUE` TEXT NOT NULL,
	PRIMARY KEY(USERID, ID),
	FOREIGN KEY(`USERID`) REFERENCES USERINFO(`USERID`) ON DELETE CASCADE,
	FOREIGN KEY(`ID`) REFERENCES PARAM(`ID`) ON DELETE CASCADE
);
*/
//---------------------------------------------------------------------------------------

typedef struct _UserInfo
{
	int nUserID;//auto increase
	string strUserGuid;
	string strUserName;
	string strPassWd;
};//tableUSERINFO;

/********************
Data Structure for _ProductInfo
ID          INTEGER  NOT NULL PRIMARY KEY 
,NAME        VARCHAR(48) NOT NULL
,SKU         VARCHAR(12) NOT NULL
,IMAGE       VARCHAR(64) NOT NULL
,URL         VARCHAR(128) NOT NULL
,PRICE       VARCHAR(8) NOT NULL
,DESCRIPTION VARCHAR(512) NOT NULL
,SHORT_NAME  VARCHAR(16)
,ZOOM        BIT 
,SHUFFLE     BIT 
,FLIP        BIT 
,MATCH       BIT 
,LINK        BIT 
*/
typedef struct _ProductInfo
{
	string ID;
	string NAME;
	string SKU;
	string IMAGE;
	string URL;
	string PRICE;
	string DESCRIPTION;
	string SHORTNAME;
	string ZOOM;
	string SHUFFLE;
	string FLIP;
	string MATCH;
	string LINK;
};//GameProductInfo;


class DataAccess
{
public:
	DataAccess();
	~DataAccess();
	static void initDB();
	static void addUser(string strUserGuid, string strUserName, string strPassWd);
	static _UserInfo getUserInfoByID(int nUserID);
	static _UserInfo getUserInfoByName(string strUserName);
	static void setParamValue(string strKeyName, string strValue);

	static string getParamValue(string strKeyName, string strDefault = "\0");
	static void initTheme();
	static void execSql(std::string strFile); 

	//Product Info
	static void loadProductList(); //load all product from db
	static std::vector<_ProductInfo>* getProductList(); //get the pointer to m_PrdInfLst vector
	static void clearProductlist(); 

	static void loadProductListbyGame(int GameID); //load all product from db
	static std::vector<_ProductInfo>* getProductListbyGame(int GameID);	//get the pointer to m_GmPrdInfLst vector, use when entering the game
	static void clearGameProductlist(); //clean m_GmPrdInfLst vector, use when exit game

	static _ProductInfo getProductInfobyID(string ID); //get product by ID from db, when no product is found, empty _ProductInfo will be returned.
	static _ProductInfo getProductInfobySKU(string SKU); //get product by ID from SKU, when no product is found, empty _ProductInfo will be returned.

	//static 
private:
	static sqlite3* openDB();
	static void insertParam(sqlite3* pDB, string strKeyName);
	static int getParamID(sqlite3* pDB, string strKeyName);
	static void dropTable(sqlite3* db, string strTableName);

};


/*
typedef struct Param
{
int nParamID;//auto increase
string strKeyName;
};

typedef struct ParamValue
{
int nUserID;
int nParamID;
string strValue;
};
typedef struct KeyValue
{
string strKeyName;
string strValue;
};*/