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
#include "MessageProcesser.h"

using namespace std;

class MessageHandler
{
protected:

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
};

class MessageNormalTradeHandler: public MessageHandler
{
public:
    MessageNormalTradeHandler(string name):MessageHandler(name) {};
    bool process(string);
};

class MessageCancelHandler: public MessageHandler
{
public:
    MessageCancelHandler(string name):MessageHandler(name) {};
    bool process(string);
};

class MessageFAKHandler: public MessageHandler
{
public:
    MessageFAKHandler(string name):MessageHandler(name) {};
    bool process(string);
};

class MessageFOKHandler: public MessageHandler
{
public:
    MessageFOKHandler(string name):MessageHandler(name) {};
    bool process(string);
};

class MessageIOCHandler: public MessageHandler
{
public:
    MessageIOCHandler(string name):MessageHandler(name) {};
    bool process(string);
};

class MessageQueryHandler: public MessageHandler
{
public:
    MessageQueryHandler(string name):MessageHandler(name) {};
    bool process(string);
};

#endif
