#include "GlobalInfo.h"


GlobalInfo* GlobalInfo::m_instance = NULL;
GlobalInfo* GlobalInfo::getInstance()
{
    if(!m_instance)
    {
        m_instance = new GlobalInfo();
    }
    return m_instance;
}
GlobalInfo::GlobalInfo()
{
}


vector<int> GlobalInfo::get_gizilist(){ return gizilist;}
void GlobalInfo::set_gizilist(vector<int> n_gizilist){gizilist = n_gizilist;}

GlobalInfo::~GlobalInfo()
{
   
}
