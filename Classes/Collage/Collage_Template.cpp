//===========Tweebaa==============
//Collage_Template.cpp	--Jun 23 2015	
//Version 1.1 by Raymond Zhuang at Toronto
// last modified at Nov 17 2015

#include "Collage_Template.h"

string G_UNM,G_UID,TEMP_filePath,G_Score,G_Level,G_Percentage;
float	G_fP;

CollageTemplateList::CollageTemplateList(void)
{
}

CollageTemplateList::~CollageTemplateList(void)
{
}

void CollageTemplateList::Str2SQL(char * source)
{
	char sTmp[256];
	int j=0;
	for(int i=0;i<strlen(source);i++)
	{
		sTmp[j]=source[i];
		if(source[i]=='\'')
		{
			j++;
			sTmp[j]=source[i];
		}
		j++;
	}
	sTmp[j]='\0';
	strcpy(source,sTmp);
}

sqlite3* CollageTemplateList::initDB(const char *db_file)
{
    auto sharedFileUtils = FileUtils::getInstance();
 
	string filePath = FileUtils::getInstance()->getWritablePath();
    string path = filePath+db_file;
 
 
    /*bool isExist = false;
    isExist = sharedFileUtils->isFileExist(path);
    if (!isExist) {
        log("Collage.db3 doesn't exist.");
        return -1;
    }*/
 
 
    sqlite3* db= NULL;
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) 
	{
        sqlite3_close(db);
        log("DB open failure.");
		return NULL;
    } 
	else 
	{
        char *err;
        string create_Design = 
	"CREATE TABLE IF NOT EXISTS CollageDesigns (id TEXT PRIMARY KEY,cdate TEXT,image TEXT,title TEXT,inspiration TEXT,user TEXT,like_count TEXT,personal_flag INTEGER,share_count TEXT)";
		string create_Product = 
	"CREATE TABLE IF NOT EXISTS CollageProducts (id TEXT PRIMARY KEY, image TEXT,name TEXT,price TEXT,description TEXT)";
		string create_Draft_List = 
	"CREATE TABLE IF NOT EXISTS DraftList (filename TEXT PRIMARY KEY, UUID TEXT,cdate TEXT)";
		string create_Draft_Info = 
	"CREATE TABLE IF NOT EXISTS DraftInfo (uid TEXT,filename TEXT , id INTEGER REFERENCES DraftList(rowid) ON DELETE CASCADE\
	,type INTEGER,string TEXT,pos_x REAL,pos_y REAL,pos_z INTEGER,scale REAL,angle REAL,flip INTEGER,color INTEGER,font TEXT,\
	cat INTEGER,sn INTEGER PRIMARY KEY AUTOINCREMENT)";
 
 
        if (sqlite3_exec(db,create_Design.c_str(),NULL,NULL,&err) != SQLITE_OK) 
		{
            sqlite3_close(db);
            log("Create table[designs] failure.");
			return NULL;
        }

		if (sqlite3_exec(db,create_Product.c_str(),NULL,NULL,&err) != SQLITE_OK) 
		{
            sqlite3_close(db);
            log("Create table[products] failure.");
			return NULL;
        }

		if (sqlite3_exec(db,create_Draft_List.c_str(),NULL,NULL,&err) != SQLITE_OK) 
		{
            sqlite3_close(db);
            log("Create table[draft_list] failure.");
			return NULL;
        }

		if (sqlite3_exec(db,create_Draft_Info.c_str(),NULL,NULL,&err) != SQLITE_OK) 
		{
            sqlite3_close(db);
            log("Create table[draft_info] failure.");
			return NULL;
        }
        //sqlite3_close(db);
    }
 
    return db;
}

CollageTemplateList * CollageTemplateList::LoadXMLFromDB(const char *db_file,const char *table,const char * sort)
{
	CollageTemplateList *pXMLParser = new CollageTemplateList();
	sctTempList	sctTemp;
	std::stringstream ss;
	
	char sSQL[256];

	sprintf(sSQL,"select * from %s order by %s DESC",table,sort);
	char **re;
	int r,c;
	auto pDB = CollageTemplateList::initDB(db_file);
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK) 
	{
		sqlite3_close(pDB);
        log("load XML from table[designs] failure.");
		return NULL;
    }

	if(r==0)
	{
		sqlite3_free_table(re);
		sqlite3_close(pDB);
		log("no element from %s.",table);
		return NULL;
	}

	for(int i=1;i<r+1;i++)
	{
		sctTemp.id = i-1;
		strcpy(sctTemp.uid,re[i*c]);
		strcpy(sctTemp.strListImage,re[i*c+2]);
		strcpy(sctTemp.sTitle,re[i*c+3]);
		strcpy(sctTemp.sFourthEle,re[i*c+4]);
		strcpy(sctTemp.sFifthEle,re[i*c+5]);
		strcpy(sctTemp.sSixthEle,re[i*c+6]);
		strcpy(sctTemp.sSeventhEle,re[i*c+7]);
		strcpy(sctTemp.sEighthEle,re[i*c+8]);

		pXMLParser->vTemplate.push_back(sctTemp);
	}
	sqlite3_free_table(re);
	sqlite3_close(pDB);
	return pXMLParser;
}

CollageTemplateList * CollageTemplateList::LoadXMLFromDB(const char *db_file,const char *table,const char *field,const char *key)
{
	CollageTemplateList *pXMLParser = new CollageTemplateList();
	sctTempList	sctTemp;
	char sSQL[256];

	sprintf(sSQL,"select * from %s where %s='%s' order by id DESC",table,field,key);
	char **re;
	int r,c;
	auto pDB = CollageTemplateList::initDB(db_file);
	if (sqlite3_get_table(pDB, sSQL, &re, &r, &c, NULL) != SQLITE_OK) 
	{
		sqlite3_close(pDB);
        log("load XML from table[designs] failure.");
		return NULL;
    }

	if(r==0)
	{
		sqlite3_free_table(re);
		sqlite3_close(pDB);
		log("no element from %s.",table);
		return false;
	}

	for(int i=1;i<r+1;i++)
	{
		sctTemp.id = i-1;
		strcpy(sctTemp.uid,re[i*c]);
		strcpy(sctTemp.strListImage,re[i*c+2]);
		strcpy(sctTemp.sTitle,re[i*c+3]);
		strcpy(sctTemp.sFourthEle,re[i*c+4]);
		strcpy(sctTemp.sFifthEle,re[i*c+5]);
		strcpy(sctTemp.sSixthEle,re[i*c+6]);
		strcpy(sctTemp.sEighthEle,re[i*c+7]);

		pXMLParser->vTemplate.push_back(sctTemp);
	}
	sqlite3_free_table(re);
	sqlite3_close(pDB);
	return pXMLParser;
}

string CollageTemplateList::GetElementFromXML(const char *fileName,const string element)
{
	std::string filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
//log("path=%s",filePath.c_str());
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();

	ssize_t size;	 
	auto pBuffer = CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), "rb", &size);
	if(size<50)
	{
		log("%s size error: %d",filePath.c_str(), size);
		return NULL;
	}

	pBuffer[size-1] = '\0';
	XMLError errorId = pDoc->Parse((const char*)pBuffer); 
	
	if (errorId != 0)	//xml格式错误
	{
		log("%s format error: %d",filePath.c_str(), errorId);
		return NULL;
	}

	XMLElement *rootEle = pDoc->RootElement();	//get <CollageTemplates>
	if(!rootEle)
	{
		log("%s error: NULL Element",filePath.c_str());
		return NULL;
	}

	XMLElement *templateEle = rootEle->FirstChildElement();
	string st;
	while ( templateEle )
	{
		st = templateEle->Value();
		if(st == element)
		{
			st = templateEle->GetText();
			break;
		}
		else
		{
			templateEle = templateEle->NextSiblingElement();
			continue;
		}

	}
	return st;
}

CollageTemplateList * CollageTemplateList::ParseXMLWithfile(const char *fileName)
{
	CollageTemplateList *pXMLParser = new CollageTemplateList();
	sctTempList	sctTemp;
	
	//std::string filePath = FileUtils::getInstance()->getWritablePath() +fileName;
	std::string filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
//log("path=%s",filePath.c_str());
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	//XMLError errorId = pDoc->LoadFile(filePath.c_str());

	ssize_t size;	 
	auto pBuffer = CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), "rb", &size);
	if (pBuffer) {
		//Add by Long to verify it	
	pBuffer[size-1] = '\0';
	XMLError errorId = pDoc->Parse((const char*)pBuffer); 
	
	if (errorId != 0)	//xml格式错误
	{
		log("%s error: %d",filePath.c_str(), errorId);
		return NULL;
	}

	XMLElement *rootEle = pDoc->RootElement();	//get <CollageTemplates>
	if(!rootEle)
	{
		log("%s error: NULL Element",filePath.c_str());
		return NULL;
	}
	//log("root_name = %s", rootEle->Value());

	//获取根节点属性
	//const XMLAttribute *attribute = rootEle->FirstAttribute();
	//打印节点属性名和值
	//log("attribute_name = %s,attribute_value = %s", attribute->Name(), attribute->Value());
	
	XMLElement *templateEle = rootEle->FirstChildElement();
string st;
	while ( templateEle ) {
		XMLElement *idEle = templateEle->FirstChildElement();
		if(idEle==NULL)
		{
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		if(idEle->GetText())
			strcpy(sctTemp.uid,idEle->GetText());
		else
			strcpy(sctTemp.uid,"None");

		XMLElement *imageEle = idEle->NextSiblingElement();
		if(imageEle==NULL)
		{
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		if(imageEle->GetText())
			strcpy(sctTemp.strListImage,imageEle->GetText());
		else
			strcpy(sctTemp.strListImage,"None");
		
		if(!strcmp("gif",&sctTemp.strListImage[strlen(sctTemp.strListImage)-3]))
		{
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		
		//==========================以下不是必须字段==================
		XMLElement *TitleEle = imageEle->NextSiblingElement();
		if(TitleEle==NULL)
		{
			if(pXMLParser->vTemplate.empty())
				sctTemp.id=0;
			else
			{
				sctTemp.id=pXMLParser->vTemplate.back().id;
				sctTemp.id++;
			}
			//准备放入模板内容 sctTemp.strContent;
			////////////////////////////////////
			pXMLParser->vTemplate.push_back(sctTemp);
			// next one
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		else
		{
			/*if(TitleEle->GetText())
			{
				std::string	strSrc = TitleEle->GetText();
				std::wstring wstSrc= s2ws(strSrc); 
				std::string strDst = WStrToUTF8(wstSrc);
				strcpy(sctTemp.sTitle,strDst.c_str());
			}*/
			if(TitleEle->GetText())
				strcpy(sctTemp.sTitle,TitleEle->GetText());
			else
				strcpy(sctTemp.sTitle,"None");
		}
		
		XMLElement *FourthEle = TitleEle->NextSiblingElement();
		if(FourthEle==NULL)
		{
			if(pXMLParser->vTemplate.empty())
				sctTemp.id=0;
			else
			{
				sctTemp.id=pXMLParser->vTemplate.back().id;
				sctTemp.id++;
			}
			
			pXMLParser->vTemplate.push_back(sctTemp);
			// next one
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		else
		{
			if(FourthEle->GetText())
				strcpy(sctTemp.sFourthEle,FourthEle->GetText());
			else
				strcpy(sctTemp.sFourthEle,"None");
		}

		XMLElement *FifthEle = FourthEle->NextSiblingElement();
		if(FifthEle==NULL)
		{
			if(pXMLParser->vTemplate.empty())
				sctTemp.id=0;
			else
			{
				sctTemp.id=pXMLParser->vTemplate.back().id;
				sctTemp.id++;
			}
			
			pXMLParser->vTemplate.push_back(sctTemp);
			// next one
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		else
		{
			if(FifthEle->GetText())
				strcpy(sctTemp.sFifthEle,FifthEle->GetText());
			else
				strcpy(sctTemp.sFifthEle,"None");
		}
		
		
		XMLElement *SixthEle = FifthEle->NextSiblingElement();
		if(SixthEle==NULL)
		{
			if(pXMLParser->vTemplate.empty())
				sctTemp.id=0;
			else
			{
				sctTemp.id=pXMLParser->vTemplate.back().id;
				sctTemp.id++;
			}
			
			pXMLParser->vTemplate.push_back(sctTemp);
			// next one
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		else
		{
			if(SixthEle->GetText())
				strcpy(sctTemp.sSixthEle,SixthEle->GetText());
			else
				strcpy(sctTemp.sSixthEle,"None");
		}
		
		XMLElement *SeventhEle = SixthEle->NextSiblingElement();
		if(SeventhEle==NULL)
		{
			if(pXMLParser->vTemplate.empty())
				sctTemp.id=0;
			else
			{
				sctTemp.id=pXMLParser->vTemplate.back().id;
				sctTemp.id++;
			}
			
			pXMLParser->vTemplate.push_back(sctTemp);
			// next one
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		else
		{
			if(SeventhEle->GetText())
				strcpy(sctTemp.sSeventhEle,SeventhEle->GetText());
			else
				strcpy(sctTemp.sSeventhEle,"None");
		}
		
		XMLElement *EighthEle = SeventhEle->NextSiblingElement();
		if(EighthEle==NULL)
		{
			if(pXMLParser->vTemplate.empty())
				sctTemp.id=0;
			else
			{
				sctTemp.id=pXMLParser->vTemplate.back().id;
				sctTemp.id++;
			}
			
			pXMLParser->vTemplate.push_back(sctTemp);
			// next one
			templateEle = templateEle->NextSiblingElement();
			continue;
		}
		else
		{
			if(EighthEle->GetText())
				strcpy(sctTemp.sEighthEle,EighthEle->GetText());
			else
				strcpy(sctTemp.sEighthEle,"0");
		}
		//log("Elements ID= %s IMAGE= %s", idEle->GetText(),imageEle->GetText());
		
		if(pXMLParser->vTemplate.empty())
			sctTemp.id=0;
		else
		{
			sctTemp.id=pXMLParser->vTemplate.back().id;
			sctTemp.id++;
		}
		
		pXMLParser->vTemplate.push_back(sctTemp);
		// next one
		templateEle = templateEle->NextSiblingElement();
	}

	delete pDoc;
	//pXMLParser->autorelease(); 
    return pXMLParser;
	}
	 else{

		 return nullptr;
	 }
}

void CollageTemplateList::WStrToUTF8(std::string& dest, const std::wstring& src)
{  
	dest.clear(); 
	for (size_t i = 0; i < src.size(); i++)
	{  
		wchar_t w = src[i];  
		if (w <= 0x7f)  
		dest.push_back((char)w);  
		else if (w <= 0x7ff){  
		dest.push_back(0xc0 | ((w >> 6)& 0x1f));  
		dest.push_back(0x80| (w & 0x3f));  
	}  
	else if (w <= 0xffff)
	{  
		dest.push_back(0xe0 | ((w >> 12)& 0x0f));  
		dest.push_back(0x80| ((w >> 6) & 0x3f));  
		dest.push_back(0x80| (w & 0x3f));  
	}  
	else if (sizeof(wchar_t) > 2 && w <= 0x10ffff)
	{  
		dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation  
		dest.push_back(0x80| ((w >> 12) & 0x3f));  
		dest.push_back(0x80| ((w >> 6) & 0x3f));  
		dest.push_back(0x80| (w & 0x3f));  
	}  
	else  
		dest.push_back('?');  
	}  
}  
//! simple warpper  
std::string CollageTemplateList::WStrToUTF8(const std::wstring& str)
{  
	std::string result;  
	WStrToUTF8(result, str);  
	return result;  
}  

std::wstring CollageTemplateList::s2ws(const std::string& s)
{
    setlocale(LC_ALL, "chs"); 
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

void BrowsePage::setTouchDirection(PageView::TouchDirection direction)
{
	_touchMoveDirection = direction;
	return;
}

void BrowsePage::interceptTouchEvent(TouchEventType event, Widget* sender,Touch *touch)
{
	log("");
}