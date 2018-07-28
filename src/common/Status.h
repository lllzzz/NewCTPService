#ifndef STATUS_H
#define STATUS_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <hiredis/hiredis.h>
#include <glog/logging.h>
#include "../common/Tool.h"
#include "../common/Config.h"

using namespace std;

class Status
{
private:

    redisContext* redisHandler;
    redisReply* redisRet;
    string _key;

public:
    Status(string key);
    ~Status();

    bool isRunning();

};

#endif
