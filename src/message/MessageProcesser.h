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
    string _from;
    string _iid;

public:

    int tdReqId;

    MessageProcesser(string id, string from, string iid) {
        _id = id;
        _from = from;
        _iid = iid;
    };

    ~MessageProcesser() {};

    string getId() {
        return _id;
    }

    string getIid() {
        return _iid;
    }

    virtual bool process(Json::Value)=0;
};

class MessageCancelProcesser: public MessageProcesser
{
public:
    MessageCancelProcesser(string id, string from, string iid):MessageProcesser(id, from, iid) {};
    bool process(Json::Value);
};

class MessageTradeProcesser: public MessageProcesser
{
protected:

    string _exchangeId;
    string _orderSysId;
    string _tunnelName;

public:
    MessageTradeProcesser(string id, string from, string iid):MessageProcesser(id, from, iid) {};
    virtual bool process(Json::Value)=0;
    void traded(Json::Value);
    void canceled();
    void setOrderInfo(CThostFtdcOrderField*);
    bool checkOrder(CThostFtdcTradeField*);
};


class MessageNormalTradeProcesser: public MessageTradeProcesser
{
public:
    MessageNormalTradeProcesser(string id, string from, string iid):MessageTradeProcesser(id, from, iid) {
        _tunnelName = RESPONSE_NORMAL_TRADE;
    };
    bool process(Json::Value);
};


class MessageFAKProcesser: public MessageTradeProcesser
{
public:
    MessageFAKProcesser(string id, string from, string iid):MessageTradeProcesser(id, from, iid) {
        _tunnelName = RESPONSE_FAK;
    };
    bool process(Json::Value);
};

class MessageFOKProcesser: public MessageTradeProcesser
{
public:
    MessageFOKProcesser(string id, string from, string iid):MessageTradeProcesser(id, from, iid) {
        _tunnelName = RESPONSE_FOK;
    };
    bool process(Json::Value);
};

class MessageIOCProcesser: public MessageTradeProcesser
{
public:
    MessageIOCProcesser(string id, string from, string iid):MessageTradeProcesser(id, from, iid) {
        _tunnelName = RESPONSE_IOC;
    };
    bool process(Json::Value);
};

#endif
