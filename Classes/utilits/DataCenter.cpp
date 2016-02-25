#include "DataCenter.h"
#include "string.h"

#define splite '|'

DataCenter * DataCenter ::_dataCenter = nullptr;

DataCenter* DataCenter::getInstance()
{
    if (! _dataCenter)
    {
        _dataCenter = new DataCenter();
    }
    return _dataCenter;
}

void DataCenter::setValueVectorForKey(ValueVector &vector, const char *key)
{
    std::string valueString = "";
    for (int i = 0; i < vector.size(); i++) {
        valueString = valueString + vector.at(i).asString();
        if (i != vector.size() - 1) {
            valueString += splite;
        }
    }
    UserDefault::getInstance()->setStringForKey(key, valueString);
	return;
}

ValueVector DataCenter::getValueVectorByKey(const char * key)
{
    ValueVector valueVector;
    std::string valueString = UserDefault::getInstance()->getStringForKey(key);
    std::string::iterator i;
    std::string tmp = "";

    for(i = valueString.begin(); i != valueString.end(); ++i) {
        if((const char)*i != splite  && i != valueString.end()) {
            tmp += *i;
        } else {
            valueVector.push_back(Value(tmp));
			log("%s",tmp.c_str());
            tmp = "";
        }
    }
    valueVector.push_back(Value(tmp));

    return valueVector;

}