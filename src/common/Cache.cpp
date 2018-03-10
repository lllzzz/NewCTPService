#include "Cache.h"

Cache* Cache::instance = NULL;

Cache::Cache()
{
    // 建立Redis链接
    struct timeval timeout = {2, 0}; // 2s超时
    string host = Config::get("redis", "host");
    string port = Config::get("redis", "port");

    LOG(INFO) << "CACHE CONNECT START|" << host << "|" << port;
    redisHandler = (redisContext*)redisConnectWithTimeout(host.c_str(), Tool::s2i(port), timeout);
    if ((NULL == redisHandler) || (redisHandler->err)) {
        if (redisHandler) {
            cout << "Cache链接错误: " << redisHandler->errstr << endl;
        } else {
            cout << "Cache链接失败" << endl;
        }
        LOG(INFO) << "CACHE CONNECT FAILED";
        exit(1);
    }
    LOG(INFO) << "CACHE CONNECT SUCCESS";

    string db = Config::get("redis", "db");
    redisRet = (redisReply*)redisCommand(redisHandler, "SELECT %s", db.c_str());
    LOG(INFO) << "CACHE SELECT DB";
    freeReplyObject(redisRet);

}

Cache::~Cache()
{
    delete redisHandler;
    delete redisRet;
}

Cache* Cache::getInstance()
{
    if (NULL == instance)
        instance = new Cache();
    return instance;
}

void Cache::set(string key, string val)
{
    redisRet = (redisReply*)redisCommand(redisHandler, "SET %s %s", key.c_str(), val.c_str());
    if (!redisRet) {
        LOG(INFO) << "SET ERROR";
        exit(1);
    }
    freeReplyObject(redisRet);
}


string Cache::get(string key)
{
    redisRet = (redisReply*)redisCommand(redisHandler, "GET %s", key.c_str());
    if (!redisRet) {
        LOG(INFO) << "GET ERROR";
        exit(1);
    }
    string val = "";
    if (redisRet->len > 0) val = redisRet->str;
    freeReplyObject(redisRet);
    return val;
}


void Cache::incr(string key)
{
    redisRet = (redisReply*)redisCommand(redisHandler, "INCR %s", key.c_str());
    if (!redisRet) {
        LOG(INFO) << "INCR ERROR";
        exit(1);
    }
    freeReplyObject(redisRet);
}

void Cache::push(Json::Value data)
{
    Json::FastWriter writer;
    string dataStr = writer.write(data);
    redisRet = (redisReply*)redisCommand(redisHandler, "LPUSH %s %s", Config::get("queueKey").c_str(), dataStr.c_str());
    freeReplyObject(redisRet);
}
