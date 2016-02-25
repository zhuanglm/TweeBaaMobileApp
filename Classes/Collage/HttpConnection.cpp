//HttpConnection.c	--Jun 30 2015	by Raymond Zhuang
// last modified at

#include "HttpConnection.h"

vector<string> HttpConnection::v_template_cache_filename;
vector<sctPicList> HttpConnection::v_design_time_info;
vector<sctPicList> HttpConnection::v_design_like_info;
vector<sctPicList> HttpConnection::v_design_temp_info;
vector<sctPicList> HttpConnection::v_product_info;
//vector<sctPicList> HttpConnection::v_decoration_info;
//vector<sctPicList> HttpConnection::v_article_info;
vector<sctColorList>	HttpConnection::v_color_list;
unsigned int	HttpConnection::numberOfLoadSprites;
bool HttpConnection::bTutorialQuitFlag=false;
bool HttpConnection::bTutorialFlag = true;

char * HttpConnection::res_buf = NULL;
int  HttpConnection::nShift;

const string HttpConnection::MainCatList[Main_Categories][2] = {{"Electronic_List","1608AC1B-786A-4596-9DEE-04FC45131332"}
,{"Health_List","C781386B-DBD9-4257-9EB6-8D74ED61A9C1"},{"Apparel_List","3EE6A6A4-933A-42BA-9C53-07F6AEE27ED2"}
,{"Garden_List","4036D9BF-F178-45BB-94F0-1693923AE9B5"},{"Toy_List","C9590D3C-F179-413A-AC5B-24DB7EEB2A1A"}
,{"Tools_List","5D01EB4D-7E33-4381-B993-45988262CB92"},{"Arts_List","B984BDA6-A0A7-4CD2-9EDD-6D1B33EFC3AD"}
,{"Household_List","1382D416-6E2D-4708-8557-7F226EDBDC13"},{"Automotive_List","F5527740-ED0E-4F9B-B7B9-A8222E5EA31C"}
,{"Pets_List","1F728092-0771-431D-81B2-A9B7D3DDE7FE"},{"Sports_List","EA786CE8-9664-48CC-A22D-B1BB5957F0E2"}
,{"Office_List","1E12E85A-F86C-4243-8446-C5F33B19E454"},{"GreenEco_List","FDAC6A4C-86D4-4DBB-857C-F52831F846B1"}
,{"Clean_List","7DEE69E4-6103-4E16-A0CC-FB065473183D"},{"Children_List","F66C02B2-3441-4D71-8328-5A5129E73D2A"}};


class vector_finder
{
 	public:
 	    vector_finder(string a):m_i_a(a){}
 	    bool operator ()(const std::vector<sctPicList>::value_type &value)
 	    {
 	        return value._filename == m_i_a;
 	    }
 	private:
 	    string m_i_a;                    
};

HttpConnection::HttpConnection(void)
{
	numberOfLoadSprites=0;
}

HttpConnection::~HttpConnection(void)
{

}

//get file from website and save as cache file
void HttpConnection::HttpReqToCache(const char* url,const char* tag/*,const ccHttpRequestCallback& callback*/,const char* cache_filename)
{
	
	HttpClient* httpClient	=	HttpClient::getInstance();
	HttpRequest* pHttpReq	=	new HttpRequest();

	v_temp_filename.push_back(cache_filename);  //save filename for onget

	pHttpReq->setRequestType(HttpRequest::Type::GET);
	pHttpReq->setUrl(url);
	
	//pHttpReq->setResponseCallback(this,httpresponse_selector(HttpConnection::onGetFinished));
	//pHttpReq->setResponseCallback(callback);
	pHttpReq->setResponseCallback(CC_CALLBACK_2(HttpConnection::onGetFinished,this));
	pHttpReq->setTag(tag);
	
	httpClient->setTimeoutForConnect(TIME_OUT);
    httpClient->send(pHttpReq);
    
	pHttpReq->release();
    pHttpReq=NULL;
//log("1,buffer file: %s",cache_filename);
	
}


void HttpConnection::onGetFinished(HttpClient *sender,HttpResponse *response)
{
	const char * sTag=response->getHttpRequest()->getTag();
	numberOfLoadSprites++;
	if(v_temp_filename.size()<=0)
	{
		log("no filename in vector");
		return;
	}
	if (!response) {
        return;
    }
	if (0 != strlen(response->getHttpRequest()->getTag())) {
        log("%s compeled",response->getHttpRequest()->getTag());
    }
//log("head:[%ld] flag: %d",response->getResponseCode(), response->isSucceed());
	if (!response->isSucceed() || response->getResponseCode()!=200)		//add getResponseCode for android
    {
		log("error[%ld] buffer: %s",response->getResponseCode(), response->getErrorBuffer());
		//if failed delete this from cache file list
		v_temp_filename.erase(v_temp_filename.begin());
		//delete from v_product_info , v_decoration_info by Tag, Tag=_filename
		std::vector<sctPicList>::iterator it;
		it = find_if(HttpConnection::v_product_info.begin(), HttpConnection::v_product_info.end(), vector_finder(sTag));
		if(it!=HttpConnection::v_product_info.end())	
			HttpConnection::v_product_info.erase(it);
		else
		{
			//non-product processing: design(trend & favorite)
			it = find_if(HttpConnection::v_design_time_info.begin(), HttpConnection::v_design_time_info.end(), vector_finder(sTag));
			if(it!=HttpConnection::v_design_time_info.end())
			{
//log("del %s from Trends",sTag);
				HttpConnection::v_design_time_info.erase(it);
			}
			
			it = find_if(HttpConnection::v_design_like_info.begin(), HttpConnection::v_design_like_info.end(), vector_finder(sTag));
			if(it!=HttpConnection::v_design_like_info.end())
			{
//log("del %s from Likes",sTag);
				HttpConnection::v_design_like_info.erase(it);
			}

		}
		
		return;
    }

	std::vector<char> *buffer = response->getResponseData();
	std::string str;  
    str.insert(str.begin(), buffer->begin(), buffer->end()); 
	
	//save to cache
	FILE *pFile;
	pFile = fopen(v_temp_filename.begin()->c_str(), "wb");
	size_t nWritten = fwrite(str.c_str(),1,buffer->size(),pFile);  
	fflush(pFile);
	fclose(pFile);
//log("buffer size: %d -> file:%s",buffer->size(),v_temp_filename.begin()->c_str());
	v_temp_filename.erase(v_temp_filename.begin());
	
}

size_t HttpConnection::onCurlFinished(void *pData, size_t n, size_t nDataSize, FILE *stream)  
{  
	//char * sHead="<?xml version=\"1.0\" encoding=\"UTF-8\"?>"; 
	if(!n || !nDataSize)
		return 0;
	
	size_t nWritten = fwrite(pData,n,nDataSize,(FILE *)stream);  
	return nWritten;  
}

CURLcode HttpConnection::CurlReq(const char* url,const char * sFileName)
{
	CURL *pCurl;
	CURLcode nResCode;
	CURLcode cURLcode;  

	//cURLcode = curl_global_init(CURL_GLOBAL_SSL); 
	//cURLcode = curl_global_init(CURL_GLOBAL_DEFAULT);
	/*if(CURLE_OK != cURLcode)    
    {    
        curl_global_cleanup();    
		log("curl global init error:%d",cURLcode);
        return false;    
    }*/
	pCurl = curl_easy_init();//初始化CURL取得初始化成功后的CURL指针

	if (pCurl != NULL) 
	 {
		  FILE *pFile;
		  pFile = fopen(sFileName, "wb");
		  curl_easy_setopt(pCurl,CURLOPT_URL,url);
		  if(pFile != NULL)
			curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //指定写入的文件指针

		  curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, onCurlFinished);//设置写数据的回调函数
		  curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //让CURL报告每一件意外的事情
		  curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 5);                  //设置超时时间
		  curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
		  curl_easy_setopt(pCurl, CURLOPT_FAILONERROR, true);
		  curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		  //curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, CollageLayer::::DownProgresss);//指定显示进度的回调函数
		  nResCode = curl_easy_perform(pCurl);//执行上面设定的动作并返回状态码
		  
		  long http_code = 0;
		  if(nResCode==0){ //Add by Long
			curl_easy_getinfo (pCurl, CURLINFO_RESPONSE_CODE, &http_code);
		  }
		  curl_easy_cleanup(pCurl);           //释放相关资源

		  //curl_global_cleanup(); 
	  
		  fflush(pFile);
		  fclose(pFile);
		  //nResCode == CURLE_OK ? CCLOG("DownLoad Success") : CCLOG("CODE: %d",nResCode);
	 }
	if(nResCode != CURLE_OK) 
	{
		//log("Error CODE: %d",nResCode);
		remove(sFileName);
	}
	return nResCode;
}


size_t HttpConnection::onCurlPosted(void *pData, size_t n, size_t nDataSize, void *stream)   
{  
	
	int res_size;

    res_size = nDataSize;
    res_buf = (char *)realloc(res_buf, nShift+res_size + 1);
    memcpy(res_buf + nShift, pData, res_size);
	res_buf[nShift+res_size]='\0';
    nShift += res_size;
    return nDataSize;
}

//bFile	如果上传的是文件:true
bool HttpConnection::CurlPost(const char* url,const char * buffer,bool bFile)
{
	//string pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(sFileName);
	//string filePath = FileUtils::getInstance()->getWritablePath();
	//filePath+=buffer;


	CURLcode cURLcode;    
    CURL *cURL;    
        
    //Init    
    cURLcode = curl_global_init(CURL_GLOBAL_SSL);    
    if(CURLE_OK != cURLcode)    
    {    
        curl_global_cleanup();    
		log("curl global init error:%d",cURLcode);
        return false;    
    }    
        
    cURL = curl_easy_init();    
    if(!cURL)    
    {    
        curl_easy_cleanup(cURL);    
        curl_global_cleanup();   
		log("curl easy init error:%d",cURL);
        return false;    
    }    
    curl_httppost *post = NULL;    
    curl_httppost *last = NULL;    
        
	if(bFile)
	{
		//上传的文件的全路径并指定类型    
		curl_formadd(&post, &last,CURLFORM_COPYNAME,"photo",    
					 CURLFORM_FILE,buffer,    
					 CURLFORM_CONTENTTYPE,"Image/png",CURLFORM_END);    
	}
	else
	{
		curl_formadd(&post, &last,CURLFORM_COPYNAME,"collage",
			CURLFORM_COPYCONTENTS, buffer, CURLFORM_END);
	}
        
    ////设置多个参数    
    //curl_formadd(&post, &last,CURLFORM_COPYNAME,"username",    
    //            CURLFORM_COPYCONTENTS,"你好",CURLFORM_END);    
    
    curl_easy_setopt(cURL, CURLOPT_URL,url);
	curl_easy_setopt(cURL, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(cURL, CURLOPT_TIMEOUT,10);//超时时间10秒    
    curl_easy_setopt(cURL, CURLOPT_HTTPPOST,post);//请求方式POST    
    curl_easy_setopt(cURL, CURLOPT_WRITEFUNCTION, onCurlPosted); //设置回调    
    curl_easy_setopt(cURL, CURLOPT_VERBOSE,true); //非零值表示你想CURL报告每一件意外的事情    
        
    curl_easy_perform(cURL);    
        
    curl_easy_cleanup(cURL);    
        
    curl_global_cleanup(); 

	return true;
}