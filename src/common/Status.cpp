#include "Status.h"

Status::Status(string key)
{
    _key = "STATUS_KEY_" + key;

    // 建立Redis链接
    struct timeval timeout = {2, 0}; // 2s超时
    string host = Config::get("redis", "host");
    string port = Config::get("redis", "port");

    LOG(INFO) << "STATUS CONNECT START|" << host << "|" << port;
    redisHandler = (redisContext*)redisConnectWithTimeout(host.c_str(), Tool::s2i(port), timeout);
    if ((NULL == redisHandler) || (redisHandler->err)) {
        if (redisHandler) {
            cout << "STATUS链接错误: " << redisHandler->errstr << endl;
        } else {
            cout << "STATUS链接失败" << endl;
        }
        LOG(INFO) << "STATUS CONNECT FAILED";
        exit(1);
    }
    LOG(INFO) << "STATUS CONNECT SUCCESS";

    string db = Config::get("redis", "db");
    redisRet = (redisReply*)redisCommand(redisHandler, "SELECT %s", db.c_str());
    LOG(INFO) << "STATUS SELECT DB";
    freeReplyObject(redisRet);

    redisRet = (redisReply*)redisCommand(redisHandler, "INCR %s", _key.c_str());
    if (!redisRet) {
        LOG(INFO) << "INCR ERROR";
        exit(1);
    }
    freeReplyObject(redisRet);
}

Status::~Status()
{
    LOG(INFO) << "STATUS::~STATUS";
    redisRet = (redisReply*)redisCommand(redisHandler, "DECR %s", _key.c_str());
    if (!redisRet) {
        LOG(INFO) << "DECR ERROR";
        exit(1);
    }
    string val = "";
    if (redisRet->len > 0) val = redisRet->str;
    freeReplyObject(redisRet);
    int cnt = Tool::s2i(val);
    if (cnt <= 0) {
        redisRet = (redisReply*)redisCommand(redisHandler, "DEL %s", _key.c_str());
        if (!redisRet) {
            LOG(INFO) << "DEL ERROR";
            exit(1);
        }
        LOG(INFO) << "STATUS::~RELEASED";
        freeReplyObject(redisRet);
    }

    delete redisHandler;
    delete redisRet;
}

bool Status::isRunning() 
{
    redisRet = (redisReply*)redisCommand(redisHandler, "GET %s", _key.c_str());
    if (!redisRet) {
        LOG(INFO) << "GET ERROR";
        exit(1);
    }
    string val = "";
    if (redisRet->len > 0) val = redisRet->str;
    freeReplyObject(redisRet);
    int cnt = Tool::s2i(val);
    if (cnt > 1) return true;
    return false;
}
