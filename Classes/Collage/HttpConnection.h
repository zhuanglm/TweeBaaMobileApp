//HttpConnection.h	--Jun 30 2015	by Raymond Zhuang
// last modified at 

#pragma once

#include <cstdlib>
#include "cocos2d.h"
#include "network/HttpClient.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../../cocos2d/external/curl/include/ios/curl/curl.h"
#else
#include "curl/curl.h"
#endif
#include "define.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::network;

typedef struct {
	unsigned int	id;
	PicType			_Cat;
	char	uid[64];
	string	_filename;
	string	cache_filename;
	string	list_filename;
	string	_objecttitle;
	string	_object4th;
	string	_object5th;
	string	_object6th;
	string	_object7th;
	string	_object8th;
}sctPicList;

typedef struct {
	unsigned int	id;
	GLubyte			R;
	GLubyte			G;
	GLubyte			B;
}sctColorList;

class HttpConnection
{
	public:
		HttpConnection(void);
		~HttpConnection(void);
		//void HttpReq(const char* url,const char* tag,const ccHttpRequestCallback& callback);
		void HttpReqToCache(const char* url,const char* tag,const char * cache_filename);
		void onGetFinished(cocos2d::network::HttpClient *sender,cocos2d::network::HttpResponse *response);

		CURLcode CurlReq(const char* url,const char * sFileName);
		static size_t onCurlFinished(void *pData, size_t n, size_t nDataSize, FILE *stream);

		bool CurlPost(const char*,const char *,bool);
		static size_t onCurlPosted(void *pData, size_t n, size_t nDataSize, void *stream);
		
		vector<string> v_temp_filename;

		static	const string MainCatList[Main_Categories][2];
		//static vector<string> v_design_time_cache_filename;
		static vector<sctPicList> v_design_time_info;
		static vector<sctPicList> v_design_like_info;
		static vector<sctPicList> v_design_temp_info;
		static vector<sctPicList> v_product_info;
		static vector<string> v_template_cache_filename;
		
		//static vector<sctPicList> v_decoration_info;
		//static vector<sctPicList> v_article_info;
		static vector<sctColorList>	v_color_list;

		static unsigned int	numberOfLoadSprites;

		static char *res_buf;
		static int nShift;
		static bool bTutorialQuitFlag;
		static bool	bTutorialFlag;

		char strImageBuffer[1024*100];

};