#include "MessageService.h"

MessageService::MessageService()
{
    cout << "消息服务初始化开始..." << endl;

    // 建立Redis链接
    struct timeval timeout = {2, 0}; // 2s超时
    string host = Config::get("redis", "host");
    string port = Config::get("redis", "port");

    LOG(INFO) << "REIDS CONNECT START|" << host << "|" << port;
    redisHandler = (redisContext*)redisConnectWithTimeout(host.c_str(), Tool::s2i(port), timeout);
    if ((NULL == redisHandler) || (redisHandler->err)) {
        if (redisHandler) {
            cout << "Redis链接错误: " << redisHandler->errstr << endl;
        } else {
            cout << "Redis链接失败" << endl;
        }
        LOG(INFO) << "REDIS CONNECT FAILED";
        exit(1);
    }
    LOG(INFO) << "REDIS CONNECT SUCCESS";

    string db = Config::get("redis", "db");
    redisRet = (redisReply*)redisCommand(redisHandler, "SELECT %s", db.c_str());
    LOG(INFO) << "REDIS SELECT DB";
    freeReplyObject(redisRet);

    cout << "消息服务初始化完成..." << endl;
}

MessageService::~MessageService()
{
    delete redisHandler;
    delete redisRet;
}

void MessageService::fire(string msgName, Json::Value data)
{
    Json::FastWriter writer;
    string dataStr = writer.write(data);
    LOG(INFO) << "FIRE" << "|" << msgName << "|" << dataStr;
    redisRet = (redisReply*)redisCommand(redisHandler, "PUBLISH %s %s", msgName.c_str(), dataStr.c_str());
    freeReplyObject(redisRet);
}
