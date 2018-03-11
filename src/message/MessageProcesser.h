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
    string _exchangeId;
    string _orderSysId;

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
    virtual void response(Json::Value)=0;
    virtual void setOrderInfo(CThostFtdcOrderField *pOrder)=0;
    virtual bool checkOrder(CThostFtdcTradeField *pTrade)=0;
};

class MessageNormalTradeProcesser: public MessageProcesser
{
public:
    MessageNormalTradeProcesser(string id, string from, string iid):MessageProcesser(id, from, iid) {};
    bool process(Json::Value);
    void response(Json::Value);
    void setOrderInfo(CThostFtdcOrderField *pOrder);
    bool checkOrder(CThostFtdcTradeField *pTrade);
};

class MessageCancelProcesser: public MessageProcesser
{
public:
    MessageCancelProcesser(string id, string from, string iid):MessageProcesser(id, from, iid) {};
    bool process(Json::Value);
    void response(Json::Value);
    void setOrderInfo(CThostFtdcOrderField *pOrder) {return;};
    bool checkOrder(CThostFtdcTradeField *pTrade) {return true;};
};


#endif
