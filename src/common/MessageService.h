#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <hiredis/hiredis.h>
#include <glog/logging.h>
#include "MessageHandler.h"
#include "Tool.h"
#include "Config.h"

/**
 *
 * MessageService msgSrv = new MessageService();
 * msgSrv.fire("TICK", tickJson);
 *
 * MessageService msgSrv = new MessageService();
 * MessageHandler msgHandler = new MessageHandler("TRADE");
 * msgSrv.addHandler(msgHandler);
 * msgSrv.run();
 *
 */

using namespace std;

class MessageService
{
private:

    std::map<string, MessageHandler*> _handlerMap;

    redisContext* redisHandler;
    redisReply* redisRet;


public:
    MessageService();
    ~MessageService();

    void fire(string, Json::Value);
    void run();
    void addHandler(MessageHandler*);
};


#endif
