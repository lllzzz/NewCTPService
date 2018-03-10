#ifndef MESSAGEPROCESSER_H
#define MESSAGEPROCESSER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <glog/logging.h>
#include "../ctp/ThostFtdcTraderApi.h"
#include "../common/Cache.h"
#include "../message/MessageTunnel.h"

using namespace std;

class MessageProcesser
{
protected:

    string _id;

public:

    int tdReqId;

    MessageProcesser(string id) {
        _id = id;
    };

    ~MessageProcesser() {};

    string getId() {
        return _id;
    }

    virtual bool process(Json::Value)=0;
    virtual void request(Json::Value)=0;
};

class MessageNormalTradeProcesser: public MessageProcesser
{
public:
    MessageNormalTradeProcesser(string id):MessageProcesser(id) {};
    bool process(Json::Value);
    void request(Json::Value);
};

#endif
