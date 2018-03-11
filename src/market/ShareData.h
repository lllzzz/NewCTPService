#ifndef SHAREDATA_H
#define SHAREDATA_H


#include "../common/Config.h"
#include "../common/Cache.h"
#include "../common/Tool.h"

using namespace std;

class ShareData
{
public:
    ShareData() {};
    ~ShareData() {};

    static void setLastPrice(string, double);
    static double getLastPrice(string);

    static void setUpperPrice(string, double);
    static double getUpperPrice(string);

    static void setLowerPrice(string, double);
    static double getLowerPrice(string);
    
};

#endif