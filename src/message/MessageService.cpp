#include "MessageService.h"

MessageService* MessageService::instance = NULL;

MessageService::MessageService()
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

    string db = Config::get("redis", "db");
    redisRet = (redisReply*)redisCommand(redisHandler, "SELECT %s", db.c_str());
    LOG(INFO) << "REDIS SELECT DB";
    freeReplyObject(redisRet);

    _handlerMap = map<string, MessageHandler*>();
}

MessageService::~MessageService()
{
    delete redisHandler;
    delete redisRet;
}

MessageService* MessageService::getInstance()
{
    if (NULL == instance)
        instance = new MessageService();
    return instance;
}

void MessageService::fire(string msgName, Json::Value data)
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

void MessageService::addHandler(MessageHandler* handler)
{
    _handlerMap[handler->getName()] = handler;
}

void MessageService::run()
{
    map<string, MessageHandler*>::iterator it;
    string name;
    string data;

    // 构造命令参数
    int argc = _handlerMap.size() + 1;
    char** argv = new char*[argc];
    size_t* argvlen = new size_t[argc];
    int i = 0;
    argv[i] = new char[10];
    memcpy(argv[i], "SUBSCRIBE", 9);
    argvlen[i] = 9;
    i++;
    for (it = _handlerMap.begin(); it != _handlerMap.end(); ++it) {
        name = it->first;
        LOG(INFO) << "SUBSCRIBE " << name;
        argvlen[i] = name.length();
        argv[i] = new char[argvlen[i]];
        memset((void*)argv[i], 0, argvlen[i]);
        memcpy((void*)argv[i], name.c_str(), name.length());
        i++;
    }

    // REDIS订阅
    redisRet = (redisReply*)redisCommandArgv(redisHandler, argc, const_cast<const char**>(argv), argvlen);
    if (!redisRet) {
        LOG(INFO) << "SUBSCRIBE ERROR";
        return;
    }
    freeReplyObject(redisRet);

    // 释放内存
    for (int i = 0; i < argc; i++) {
        delete [] argv[i];
        argv[i] = NULL;
    }
    delete [] argv;
    delete [] argvlen;
    argv = NULL;

    // 循环监听
    while (true) {
        int code = redisGetReply(redisHandler, (void**)&redisRet);
        if (!redisRet) {
            LOG(INFO) << "LISTEN ERROR";
            return;
        }
        if (code != REDIS_OK) {
            LOG(INFO) << "LISTEN CODE IS NOT OK";
            break;
        }
        if (redisRet->elements >= 3) {
            name = string(redisRet->element[1]->str);
            data = string(redisRet->element[2]->str);
            LOG(INFO) << "LISTEN DATA|" << name << "|" << data;
            freeReplyObject(redisRet);
            if (!_handlerMap[name]->process(data)) {
                LOG(INFO) << "HANDLER FAILED";
            }
        } else {
            freeReplyObject(redisRet);
        }
    }
}
