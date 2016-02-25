#ifndef __C_CSV_PARSE__
#define __C_CSV_PARSE__

#include "cocos2d.h"
#include <vector>
using namespace std;

class CCSVParse
{
public:
    CCSVParse(void);
    ~CCSVParse(void);

    CCSVParse(istream& fin, string sep=","):
        fieldsep(sep),
        cols(0)
    {

    }

    //用以存储数据
    std::vector<std::vector<std::string>> data;

private:
    string        fieldsep;
    int            cols;

    void StringSplit(const string& str, vector<string>& tokens, const char& delimiters);
    void split(vector<string>& field, string line);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);

public:
    bool openFile(const char* fileName);
    const char* getData(unsigned int rows, unsigned int cols);
    int findColsData(int cols, const char* value);

    inline int getCols(){return cols;}
    inline int getRows(){return data.size();};
};

#endif //__C_CSV_PARSE__