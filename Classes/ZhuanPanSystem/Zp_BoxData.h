#ifndef _Zp_BoxData_
#define _Zp_BoxData_

#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

static char BoxReward[9][32] = { "Powerup\0", "1 Tweebaa Point\0", "10 Tweebaa Points\0", "50 Tweebaa Points\0", "100 Tweebaa Points\0", "5 TweeBucks\0", "10 TweeBucks\0", "a Wonder Cup\0", "a TweeBot\0" };

class Zp_BoxData : public CCObject
{
private :
    
	int boxid;

	int boxReward;

	bool xuanzhong;

	int tag;//not used at all

	CCPoint point;
public :
	CCPoint get_point(){return point;};
	void set_point(CCPoint n_point){point = n_point;  };

    int get_tag(){return tag;};
	void set_tag(int n_tag){tag = n_tag;  };

	int get_boxid(){return boxid;};
	void set_boxid(int n_boxid){boxid = n_boxid;  };
   
	int get_boxReward(){return boxReward;};
	void set_boxReward(int n_boxReward){boxReward = n_boxReward;  };

	int get_xuanzhong(){return xuanzhong;};
	void set_xuanzhong(int n_xuanzhong){xuanzhong = n_xuanzhong;  };
};



#endif//_ZP_IMGPATH_
