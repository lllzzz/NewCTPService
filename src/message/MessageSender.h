#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

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
#include "../common/Tool.h"
#include "../common/Config.h"

/**
 *
 * MessageSender msgSrv = new MessageSender();
 * msgSrv.fire("TICK", tickJson);
 *
 * MessageSender msgSrv = new MessageSender();
 * MessageHandler msgHandler = new MessageHandler("TRADE");
 * msgSrv.addHandler(msgHandler);
 * msgSrv.run();
 *
 */

using namespace std;

class MessageSender
{
private:

    MessageSender();

    static MessageSender* instance;

    redisContext* redisHandler;
    redisReply* redisRet;


public:
    ~MessageSender();

    static MessageSender* getInstance();

    void fire(string, Json::Value);
};

#endif
