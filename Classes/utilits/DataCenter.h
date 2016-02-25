#include "cocos2d.h"

USING_NS_CC;

class DataCenter
{
public:
    static DataCenter *getInstance();
    void setValueVectorForKey(ValueVector &vector, const char *key);
    ValueVector getValueVectorByKey(const char *key);
private:
    static DataCenter *_dataCenter;
};
