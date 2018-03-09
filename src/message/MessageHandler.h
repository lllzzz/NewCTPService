#ifndef MESSAGEHANDLE_H
#define MESSAGEHANDLE_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <glog/logging.h>

using namespace std;

class MessageHandler
{
private:

    string _messageName;

public:

    MessageHandler(string name)
    {
        _messageName = name;
    };

    ~MessageHandler() {};

    string getName()
    {
        return _messageName;
    }

    virtual bool process(string)=0;
    virtual void request(Json::Value)=0;
};

class MessageNormalTradeHandler: public MessageHandler
{
private:

    string _orderId;
    string _iid;
    double _price;
    int _total;
    bool _isBuy;
    bool _isOpen;
    bool _isToday;

public:
    MessageNormalTradeHandler(string name):MessageHandler(name) {};
    bool process(string);
    void request(Json::Value);
    string getOrderId();
};

#endif
