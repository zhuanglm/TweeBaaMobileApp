#pragma once

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class PriceMatchQuestion:public  CCNode

{
public:
	PriceMatchQuestion(void);
	~PriceMatchQuestion(void);
	//int m_iQuestionTotal;
	int m_iIndex;
	std::string m_strImgName;
	std::string m_strPrdName;
	std::string m_strOptionA;
	std::string m_strOptionB;
	std::string m_strOptionC;
	std::string m_strOptionD;
	int m_iAnswer;

};

