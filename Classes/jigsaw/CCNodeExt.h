#include "cocos2d.h"
using namespace cocos2d;

 class CCNodeExt : public Node{
	  
	
	public:
	 std::string nodeText;
	 CCNodeExt();
	
	 void setNodeText(std::string nText);

	std::string getNodeText();
};
 