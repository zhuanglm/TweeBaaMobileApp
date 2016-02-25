#ifndef _GLOBALINFO_
#define _GLOBALINFO_
#include "cocos2d.h"
#include <string>

using namespace std;
USING_NS_CC;

class GlobalInfo
{
private :
     GlobalInfo();
    ~GlobalInfo();
    static GlobalInfo* m_instance;
    
	vector<int> gizilist;
public :
   static GlobalInfo* getInstance();
    
    vector<int> get_gizilist();
    void set_gizilist(vector<int> gizilist);    
};

#endif /*_GLOBALINFO_*/
