#include "DataAccess.h"
#include <vector>

int m_UserID;
int m_CrrntGameID;
std::vector<_ProductInfo> m_PrdInfLst;
std::vector<_ProductInfo> m_GmPrdInfLst;

DataAccess::DataAccess(void){ m_iCurrentGameID = -1; }
DataAccess::~DataAccess(void){}
void DataAccess::initDB()
{
	string strInitFlag=UserDefault::getInstance()->getStringForKey("DBInitial", "\0");
	if (strInitFlag != VERSION)
	{
		string filePath = FileUtils::getInstance()->getWritablePath();
		string path = filePath + DB_File;

		sqlite3* db = NULL;
		if (sqlite3_open(path.c_str(), &db) != SQLITE_OK)
		{
			sqlite3_close(db);
			log("DB open failure.");
			return;
		}
		else
		{
			char *err;

			dropTable(db,"USERINFO");
			dropTable(db, "PARAM");
			dropTable(db, "PARAMVALUE");

			string create_UserInfo =
				"CREATE TABLE  IF NOT EXISTS `USERINFO` \
				(`USERID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
				`USERGUID` TEXT NOT NULL UNIQUE,\
				`USERNAME` TEXT NOT NULL UNIQUE,\
				`PASSWD` TEXT NOT NULL); ";

			string create_Param =
				"CREATE TABLE IF NOT EXISTS `PARAM` \
				(`ID` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
				`KEYNAME` TEXT NOT NULL UNIQUE); ";

			string create_ParamValue =
				"CREATE TABLE IF NOT EXISTS `PARAMVALUE`\
				 (`USERID` INTEGER NOT NULL,\
				 `ID` INTEGER NOT NULL,\
				 `VALUE` TEXT NOT NULL,\
				 PRIMARY KEY(USERID, ID),\
				 FOREIGN KEY(`USERID`) REFERENCES USERINFO(`USERID`) ON DELETE CASCADE,\
				 FOREIGN KEY(`ID`) REFERENCES PARAM(`ID`) ON DELETE CASCADE); ";


		if (sqlite3_exec(db, create_UserInfo.c_str(), NULL, NULL, &err) !=SQLITE_OK)
		{
			sqlite3_close(db);
			log("Create USERINFO failure.");
			return;
		}

		if (sqlite3_exec(db, create_Param.c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			sqlite3_close(db);
			log("Create PARAM failure.");
			return;
		}

		if (sqlite3_exec(db, create_ParamValue.c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			sqlite3_close(db);
			log("Create PARAMVALUE failure.");
			return;
		}


		string sSQL = "INSERT INTO USERINFO(USERID,USERGUID, USERNAME, PASSWD)  VALUES (-1,'','','')";

		//sprintf(sSQL, "INSERT INTO USERINFO(USERGUID, USERNAME, PASSWD)  VALUES ('%s','%s','%s');", strUserGuid.c_str(), strUserName.c_str(), strPassWd.c_str());//ENCRYPTED USERNAME
		if (sqlite3_exec(db, sSQL.c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			log("initial USERINFO failure .");
		}

			//-------------------------------------------------------------------------------------------
			//for theme mode


		dropTable(db,"THEME");
		dropTable(db, "THEMEINFO");
		dropTable(db, "ROUND");
		dropTable(db, "ROUNDINFO");

			string create_Theme =
				"CREATE TABLE  IF NOT EXISTS `THEME`\
				(`THEMEID`	INTEGER NOT NULL,\
				`CREATEDATE` TEXT NOT NULL,\
				`ISACTIVE` INTEGER NOT NULL,\
				PRIMARY KEY(THEMEID));";


			string create_ThemeInfo =
				"CREATE TABLE  IF NOT EXISTS `THEMEINFO`\
				(`THEMEINFOID` INTEGER NOT NULL,\
				`THEMEID` INTEGER NOT NULL,\
				`LANGUAGEID` INTEGER NOT NULL,\
				`THEMENAME` TEXT NOT NULL,\
				`THEMEIMAGE` TEXT NOT NULL,\
				PRIMARY KEY(THEMEINFOID),\
				FOREIGN KEY(`THEMEID`) REFERENCES THEME(`THEMEID`) );";


			string create_Round =
				"CREATE TABLE  IF NOT EXISTS `ROUND`\
				(`ROUNDID` INTEGER NOT NULL,\
				`THEMEID` INTEGER NOT NULL,\
				PRIMARY KEY(ROUNDID,THEMEID),\
				FOREIGN KEY(`THEMEID`) REFERENCES THEME(`THEMEID`)); ";


			string create_RoundInfo =
				"CREATE TABLE IF NOT EXISTS `ROUNDINFO`\
				(`ROUNDINFOID` INTEGER NOT NULL,\
				`ROUNDID` INTEGER NOT NULL,\
				`LANGUAGEID` INTEGER NOT NULL,\
				`STEP` INTEGER NOT NULL,\
				`TYPE` INTEGER NOT NULL,\
				`INFO` TEXT NOT NULL,\
				`EFFECT` TEXT NOT NULL,\
				PRIMARY KEY(ROUNDINFOID)\
				);";
				//FOREIGN KEY(`ROUNDID`) REFERENCES ROUND(`ROUNDID`)\

			if (sqlite3_exec(db, create_Theme.c_str(), NULL, NULL, &err) != SQLITE_OK)
			{
				sqlite3_close(db);
				log("Create THEME failure.");
				return;
			}

			if (sqlite3_exec(db, create_ThemeInfo.c_str(), NULL, NULL, &err) != SQLITE_OK)
			{
				sqlite3_close(db);
				log("Create ThemeInfo failure.");
				return;
			}

			if (sqlite3_exec(db, create_Round.c_str(), NULL, NULL, &err) != SQLITE_OK)
			{
				sqlite3_close(db);
				log("Create ROUNDINFO failure.");
				return;
			}

			if (sqlite3_exec(db, create_RoundInfo.c_str(), NULL, NULL, &err) != SQLITE_OK)
			{
				sqlite3_close(db);
				log("Create ROUNDINFO failure.");
				return;
			}

			string strSqlFile = THEME_INIT_FILE;
			execSql(strSqlFile);
			//sqlite3_close(db);

			strSqlFile = PRODUCT_INFO_FILE;
			execSql(strSqlFile);

			sqlite3_close(db);

			loadProductList();
			//_ProductInfo p = getProductInfobyID("12");
			//p = getProductInfobySKU("1700012210");

		}
		UserDefault::getInstance()->setStringForKey("DBInitial", VERSION);
	}
}


void DataAccess::dropTable(sqlite3* db, string strTableName)
{
	string strSql = "DROP TABLE " + strTableName;
	char *err;
	if (sqlite3_exec(db, strSql.c_str(), NULL, NULL, &err) != SQLITE_OK)
	{
		log("Drop table %s error %s", strTableName.c_str(), err);
	}
}

sqlite3* DataAccess::openDB()
{
	string filePath = FileUtils::getInstance()->getWritablePath();
	string path = filePath + DB_File;

	sqlite3* pDB = NULL;
	if (sqlite3_open(path.c_str(), &pDB) != SQLITE_OK)
	{
		sqlite3_close(pDB);
		log("DB open failure.");
		return NULL;
	}
	else
	{
		char *err;

		string open_FK =
			"PRAGMA foreign_keys = ON;";

		if (sqlite3_exec(pDB, open_FK.c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			sqlite3_close(pDB);
			log("Open foreign constriant failed.");
			return NULL;
		}
	}
	return pDB;
}

void DataAccess::addUser(string strUserGuid, string strUserName, string strPassWd)
{
	sqlite3* pDB = openDB();
	if (pDB)
	{
		char sSQL[256];
		char *err;
		sprintf(sSQL, "INSERT INTO USERINFO(USERGUID, USERNAME, PASSWD)  VALUES ('%s','%s','%s');", strUserGuid.c_str(), strUserName.c_str(), strPassWd.c_str());//ENCRYPTED USERNAME
		if (sqlite3_exec(pDB, sSQL, NULL, NULL, &err) != SQLITE_OK)
		{
			log("insert into USERINFO failure %s :Username=%s.", err, strUserName.c_str());
		}
		sqlite3_close(pDB);
	}
	else log("Open DB failure.");
}

void DataAccess::setParamValue(string strKeyName, string strValue)
{
	sqlite3* pDB = openDB();

	if (pDB)
	{
		char sSQL[256];
		char *err;
		insertParam(pDB, strKeyName);
		int nParamID = getParamID(pDB, strKeyName);

		char **re;
		int r, c;
		sprintf(sSQL, "SELECT T3.VALUE FROM USERINFO T1, PARAM T2, PARAMVALUE T3 WHERE T1.USERID=T3.USERID AND T2.ID=T3.ID AND T1.USERID=%d AND T2.KEYNAME='%s';", m_UserID, strKeyName.c_str());//ENCRYPTED USERNAME
		if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
		{
			log("select from PARAMVALUE failure.");
			sqlite3_free_table(re);
			sqlite3_close(pDB);
			return;
		}
		if (r > 0)//already exists
		{
			sprintf(sSQL, "UPDATE PARAMVALUE SET VALUE = '%s' WHERE USERID = %d AND ID=%d;", strValue.c_str(), m_UserID, nParamID);//ENCRYPTED USERNAME
			if (sqlite3_exec(pDB, sSQL, NULL, NULL, &err) != SQLITE_OK)
			{
				log("update PARAMVALUE failure %s: UserId=%d,ParamId=%d,Keyname=%s,Value=%s.", err, m_UserID, nParamID, strKeyName.c_str(), strValue.c_str());
				sqlite3_close(pDB);
				return;
			}
		}
		else//not exists
		{
			sprintf(sSQL, "INSERT INTO PARAMVALUE(USERID, ID, VALUE)  VALUES(%d,%d, '%s');", m_UserID, nParamID, strValue.c_str());//ENCRYPTED USERNAME
			if (sqlite3_exec(pDB, sSQL, NULL, NULL, &err) != SQLITE_OK)
			{
				log("insert into PARAMVALUE failure %s: UserId=%d,ParamId=%d,Keyname=%s,Value=%s.", err, m_UserID, nParamID, strKeyName.c_str(), strValue.c_str());
				sqlite3_close(pDB);
				return;
			}
		}
		sqlite3_free_table(re);
		sqlite3_close(pDB);
	}
	else log("Open DB failure.");
}

void DataAccess::insertParam(sqlite3* pDB,string strKeyName)
{
	char sSQL[256];
	char *err;
	sprintf(sSQL, "INSERT INTO PARAM(KEYNAME) VALUES('%s');", strKeyName.c_str());
	if (sqlite3_exec(pDB, sSQL, NULL, NULL, &err) != SQLITE_OK)
	{
		log("insert into PARAM failure %s:%s.", err, strKeyName.c_str());
	}
}

int  DataAccess::getParamID(sqlite3* pDB,string strKeyName)
{
	char sSQL[256];
	char **re;
	int r, c;
	sprintf(sSQL, "SELECT ID FROM PARAM WHERE KEYNAME ='%s'", strKeyName.c_str());
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
	{
		log("select from PARAM failure.");
		sqlite3_free_table(re);
		return -1;
	}
	int nRet=-1;
	if (r >0)
	{
		nRet = atoi(re[1]);
	}
	sqlite3_free_table(re);
	return nRet;
}

_UserInfo DataAccess::getUserInfoByID(int nUserID)
{
	sqlite3* pDB = openDB();
	_UserInfo UserInfo;
	UserInfo.nUserID = -1;
	if (pDB)
	{
		char sSQL[256];
		char **re;
		int r, c;
		sprintf(sSQL, "SELECT * FROM USERINFO WHERE USERID =%d", nUserID);
		if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
		{
			log("select from PARAM failure.");
		}
		else
		{
			if (r > 0)
			{
				UserInfo.nUserID = nUserID;
				UserInfo.strUserGuid = re[c + 1];
				UserInfo.strUserName = re[c + 2];
				UserInfo.strPassWd = re[c + 3];
			}
		}
		sqlite3_free_table(re);
	}
	sqlite3_close(pDB);
	return UserInfo;
}
_UserInfo DataAccess::getUserInfoByName(string strUserName)
{

	sqlite3* pDB = openDB();
	_UserInfo UserInfo;
	UserInfo.nUserID = -1;
	if (pDB)
	{
		char sSQL[256];
		char **re;
		int r, c;
		sprintf(sSQL, "SELECT * FROM USERINFO WHERE USERNAME ='%s'", strUserName.c_str());
		if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
		{
			log("select from PARAM failure.");
		}
		else
		{
			if (r > 0)
			{
				UserInfo.nUserID = atoi(re[c]);
				UserInfo.strUserGuid = re[c + 1];
				UserInfo.strUserName = re[c + 2];
				UserInfo.strPassWd = re[c + 3];
			}
		}
		sqlite3_free_table(re);
	}
	sqlite3_close(pDB);
	return UserInfo;
}
string DataAccess::getParamValue(string strKeyName,string strDefault)
{
	sqlite3* pDB = openDB();
	string strRet = strDefault;
	int nParamID = getParamID(pDB, strKeyName);
	if (nParamID > 0)
	{
		char sSQL[256];
		char **re;
		int r, c;	
		sprintf(sSQL, "SELECT VALUE FROM PARAMVALUE WHERE USERID = %d AND ID =%d", m_UserID, nParamID);
		if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
		{
			log("select from PARAM failure.");
		}
		else
		{
			if (r > 0)
			{
				strRet = re[1];
			}
		}
		sqlite3_free_table(re);
	}
	sqlite3_close(pDB);
	return strRet;
}
//----------------------------------------------------------------------------------------------------
//for theme mode

void DataAccess::initTheme()
{
	AppDelegate* app = (AppDelegate*)Application::getInstance();
	sqlite3* pDB = openDB();
	if (pDB)
	{
		app->sp_GameRound.clear();
		char sSQL[256];
		char **re;
		int r, c;
		sprintf(sSQL, "SELECT DISTINCT A.STEP,A.TYPE,A.INFO,A.EFFECT FROM ROUNDINFO A,ROUND B WHERE B.THEMEID=%d AND A.ROUNDID=%d AND A.ROUNDID=B.ROUNDID ORDER BY STEP",app->iThemeID,app->iRound);

		if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
		{
			sqlite3_close(pDB);
			log("Init theme failure.");
			return ;
		}

		if (r == 0)
		{
			sqlite3_free_table(re);
			sqlite3_close(pDB);
			log("Init theme failure,no elements.");
			return;
		}
		stGameRound round;

		for (int i = 1; i<r + 1; i++)
		{
			round.round_id = app->iRound;//1
			round.round_step = atoi(re[i*c]);
			round.round_type = atoi(re[i*c+1]);
			round.round_info = re[i*c + 2];
			round.round_effect = re[i*c + 3];
			app->sp_GameRound.push_back(round);
		}
		sqlite3_free_table(re);
	}
	sqlite3_close(pDB);
	return ;
}

void DataAccess::execSql(std::string strFile){
	// Initialize variables needed
	//unsigned long fileSize = 0;
	ssize_t fileSize = 0;
	unsigned char * fileContents = NULL;
	string thisLine, result, fullPath, contents;
	sqlite3* pDB = openDB();
	char *err;

	if (FileUtils::getInstance()->isFileExist(strFile) == false){
		log("%s, does not exist",strFile.c_str());
		return;
	}

	fileContents = FileUtils::getInstance()->getFileData(strFile,"r", &fileSize);
	contents.append( ( char * ) fileContents );
   // Create a string stream so that we can use getline( ) on it
   istringstream fileStringStream( contents );

   // Get file contents line by line
   while ( getline( fileStringStream, thisLine ) ) {
      // Put all lines in vector
		if (sqlite3_exec(pDB, thisLine.c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			log("DataAccess: failed inserted to table, sql %s,err=%s",thisLine.c_str(),err);
			//break;
		}
		//else{
		//	log("DataAccess: successfully inserted to table, sql %s",thisLine.c_str());		
		//}
   }

	sqlite3_close(pDB);

	// Delete buffer created by fileContents. This part is required.
	if ( fileContents ) {
		delete[ ] fileContents;
		fileContents = NULL;
	}
	return;

}

/*
CREATE TABLE IF NOT EXISTS GAMEPRODUCTINFO(
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
);
*/
void DataAccess::loadProductList(){
	do{

		sqlite3* pDB = openDB();


		if (pDB)
		{
			char sSQL[256];
			char **re;
			int r, c;
			sprintf(sSQL, "SELECT * FROM GAMEPRODUCTINFO");
			if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
			{
				log("select from PARAM failure.");
			}
			else
			{
				if (r > 0)
				{
					m_PrdInfLst.clear();
					for(int i = 1; i <= r; i++){
							_ProductInfo PdInfo;
							PdInfo.ID = re[i*c];
							PdInfo.NAME = re[i*c + 1];
							PdInfo.SKU = re[i*c + 2];
							PdInfo.IMAGE = re[i*c + 3];
							PdInfo.URL = re[i*c + 4];
							PdInfo.PRICE = re[i*c + 5];
							PdInfo.DESCRIPTION = re[i*c + 6];
							PdInfo.SHORTNAME = re[i*c + 7];
							PdInfo.ZOOM = re[i*c + 8];
							PdInfo.SHUFFLE = re[i*c + 9];
							PdInfo.FLIP = re[i*c + 10];
							PdInfo.MATCH = re[i*c + 11];
							PdInfo.LINK = re[i*c + 12];
							//log("DataAccess: Loading product info: ID %s, SKU %s, Price %s, Zoom %s", PdInfo.ID, PdInfo.SKU.c_str(),PdInfo.PRICE.c_str(),PdInfo.ZOOM);
							m_PrdInfLst.push_back(PdInfo);
					}
				}
			}
			sqlite3_free_table(re);
	}
	sqlite3_close(pDB);

	}while(0);
	return;
}

_ProductInfo DataAccess::getProductInfobyID(string ID){

	_ProductInfo PdInfo;
	PdInfo.ID = "";
	PdInfo.NAME = "";
	PdInfo.SKU = "";
	PdInfo.IMAGE = "";
	PdInfo.URL = "";
	PdInfo.PRICE = "";
	PdInfo.DESCRIPTION = "";
	PdInfo.SHORTNAME = "";
	PdInfo.ZOOM = "";
	PdInfo.SHUFFLE = "";
	PdInfo.FLIP = "";
	PdInfo.MATCH = "";
	PdInfo.LINK = "";

	do{
		sqlite3* pDB = openDB();
	
		if (pDB)
		{
			char sSQL[256];
			char **re;
			int r, c;
			sprintf(sSQL, "SELECT * FROM GAMEPRODUCTINFO WHERE ID = %s;", ID.c_str());
			if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
			{
				log("select from PARAM failure.");
			}
			else
			{
				if (r > 0)
				{
					//_ProductInfo PdInfo;
					PdInfo.ID = re[c];
					PdInfo.NAME = re[c + 1];
					PdInfo.SKU = re[c + 2];
					PdInfo.IMAGE = re[c + 3];
					PdInfo.URL = re[c + 4];
					PdInfo.PRICE = re[c + 5];
					PdInfo.DESCRIPTION = re[c + 6];
					PdInfo.SHORTNAME = re[c + 7];
					PdInfo.ZOOM = re[c + 8];
					PdInfo.SHUFFLE = re[c + 9];
					PdInfo.FLIP = re[c + 10];
					PdInfo.MATCH = re[c + 11];
					PdInfo.LINK = re[c + 12];
					log("DataAccess: getProductInfobyID product info: ID %s, SKU %s, Price %s, Zoom %s", PdInfo.ID.c_str(), PdInfo.SKU.c_str(),PdInfo.PRICE.c_str(),PdInfo.ZOOM.c_str());
				}
			}
			sqlite3_free_table(re);
		}
		sqlite3_close(pDB);

	}while(0);
	return PdInfo;
}

_ProductInfo DataAccess::getProductInfobySKU(string SKU){ //get product from Sqlite DB
	_ProductInfo PdInfo;
	PdInfo.ID = "";
	PdInfo.NAME = "";
	PdInfo.SKU = "";
	PdInfo.IMAGE = "";
	PdInfo.URL = "";
	PdInfo.PRICE = "";
	PdInfo.DESCRIPTION = "";
	PdInfo.SHORTNAME = "";
	PdInfo.ZOOM = "";
	PdInfo.SHUFFLE = "";
	PdInfo.FLIP = "";
	PdInfo.MATCH = "";
	PdInfo.LINK = "";

	do{
		sqlite3* pDB = openDB();
	
		if (pDB)
		{
			char sSQL[256];
			char **re;
			int r, c;
			sprintf(sSQL, "SELECT * FROM GAMEPRODUCTINFO WHERE SKU = %s;", SKU.c_str());
			if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
			{
				log("select from PARAM failure.");
			}
			else
			{
				if (r > 0)
				{
					//_ProductInfo PdInfo;
					PdInfo.ID = re[c];
					PdInfo.NAME = re[c + 1];
					PdInfo.SKU = re[c + 2];
					PdInfo.IMAGE = re[c + 3];
					PdInfo.URL = re[c + 4];
					PdInfo.PRICE = re[c + 5];
					PdInfo.DESCRIPTION = re[c + 6];
					PdInfo.SHORTNAME = re[c + 7];
					PdInfo.ZOOM = re[c + 8];
					PdInfo.SHUFFLE = re[c + 9];
					PdInfo.FLIP = re[c + 10];
					PdInfo.MATCH = re[c + 11];
					PdInfo.LINK = re[c + 12];
					log("DataAccess: getProductInfobyID product info: ID %s, SKU %s, Price %s, Zoom %s", PdInfo.ID.c_str(), PdInfo.SKU.c_str(),PdInfo.PRICE.c_str(),PdInfo.ZOOM.c_str());
				}
			}
			sqlite3_free_table(re);
		}
		sqlite3_close(pDB);

	}while(0);
	return PdInfo;
}

void DataAccess::loadProductListbyGame(int GameID){
	do{
		if (m_CrrntGameID == GameID) break;

		m_GmPrdInfLst.clear();
		sqlite3* pDB = openDB();
		string flag = "";
		switch(GameID){
			case 2: 
				flag = "ZOOM";
				break;
			case 3:	
				flag = "SHUFFLE";
				break;
			case 4:	
				flag = "FLIP";
				break;
			case 5:	
				flag = "MATCH";
				break;
			case 6:	
				flag = "LINK";
				break;
			default:
				return;
		}

		if (pDB)
		{
			char sSQL[256];
			char **re;
			int r, c;
			sprintf(sSQL, "SELECT * FROM GAMEPRODUCTINFO WHERE %s = 1;", flag.c_str());
			if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK)
			{
				log("select from PARAM failure.");
			}
			else
			{
				if (r > 0)
				{
					
					for(int i = 1; i <= r; i++){
							_ProductInfo PdInfo;
							PdInfo.ID = re[i*c];
							PdInfo.NAME = re[i*c + 1];
							PdInfo.SKU = re[i*c + 2];
							PdInfo.IMAGE = re[i*c + 3];
							PdInfo.URL = re[i*c + 4];
							PdInfo.PRICE = re[i*c + 5];
							PdInfo.DESCRIPTION = re[i*c + 6];
							PdInfo.SHORTNAME = re[i*c + 7];
							PdInfo.ZOOM = re[i*c + 8];
							PdInfo.SHUFFLE = re[i*c + 9];
							PdInfo.FLIP = re[i*c + 10];
							PdInfo.MATCH = re[i*c + 11];
							PdInfo.LINK = re[i*c + 12];
							log("DataAccess: Loading product info: ID %s, SKU %s, Price %s, Zoom %s", PdInfo.ID.c_str(), PdInfo.SKU.c_str(),PdInfo.PRICE.c_str(),PdInfo.ZOOM.c_str());
							m_GmPrdInfLst.push_back(PdInfo);
					}
				}
			}
			sqlite3_free_table(re);
	}
	sqlite3_close(pDB);

	}while(0);
	return;
}
std::vector<_ProductInfo> *DataAccess::getProductListbyGame(int GameID){ //get product from Sqlite DB
	if(m_CrrntGameID != GameID){
		loadProductListbyGame(GameID);
	}
	return &m_GmPrdInfLst;
}

void DataAccess::clearGameProductlist(){
	m_CrrntGameID = 0;
	m_GmPrdInfLst.clear();
	return;
}

std::vector<_ProductInfo>* DataAccess::getProductList(){
	return &m_PrdInfLst;
};

void DataAccess::clearProductlist(){
	m_PrdInfLst.clear();
	return;
}