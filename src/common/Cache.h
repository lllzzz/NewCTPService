#ifndef CACHE_H
#define CACHE_H

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

class Cache
{
private:

    Cache();

    static Cache* instance;

    redisContext* redisHandler;
    redisReply* redisRet;


public:
    ~Cache();

    static Cache* getInstance();

    void set(string, string);
    void incr(string);
    string get(string);
    void push(Json::Value);

};

#endif
