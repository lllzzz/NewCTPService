#ifndef REDIS_H
#define REDIS_H

#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Config.h"
#include "Lib.h"

using namespace std;

typedef bool (*ACTIONCALLBACK)(string);

class Redis
{
public:
    redisContext *pRedisContext;
    redisReply *pRedisReply;

    ACTIONCALLBACK _callback;
    string _channel;

    string _getResultAsString();
    long long _getResultAsInt();
    void _freeReply();

public:
    Redis(string host, int port, int db);
    ~Redis();
    // string execCmd(string cmd, bool = false);
    string pop(string key);
    void set(string key, string data);
    void setnx(string key, string data);
    long long incr(string key);
    string get(string key);
    void push(string key, string data);

    void pub(string key, string data);

    void asService(ACTIONCALLBACK, string);
    void run();
};

#endif
