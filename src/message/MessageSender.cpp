#include "MessageSender.h"

MessageSender* MessageSender::instance = NULL;

MessageSender::MessageSender()
{
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
}

MessageSender::~MessageSender()
{
    delete redisHandler;
    delete redisRet;
}

MessageSender* MessageSender::getInstance()
{
    if (NULL == instance)
        instance = new MessageSender();
    return instance;
}

void MessageSender::fire(string msgName, Json::Value data)
{
    Json::FastWriter writer;
    string dataStr = writer.write(data);
    LOG(INFO) << "FIRE" << "|" << msgName << "|" << dataStr;
    // 消息发送
    redisRet = (redisReply*)redisCommand(redisHandler, "PUBLISH %s %s", msgName.c_str(), dataStr.c_str());
    if (!redisRet) {
        LOG(INFO) << "FIRE ERROR";
        exit(1);
    }
    freeReplyObject(redisRet);
}
