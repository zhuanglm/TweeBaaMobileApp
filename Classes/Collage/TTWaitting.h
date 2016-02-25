#include <iostream>
#include "cocos2d.h"
//#include "cocos-ext.h"

USING_NS_CC;
//USING_NS_CC_EXT;

using namespace std;

class   TTWaitting:public   Layer
{
public:
    TTWaitting();
    ~TTWaitting();
    
    bool   initWithBgImage(std::string  strImageName);
    
    void   startAnimation();
    
    void   stopAnimation();
    
    static   TTWaitting *  createWithBgImage(std::string  strImageName);
    
public:
    Sprite  *  m_bgSprite;
};