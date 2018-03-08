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

    ~MessageHandler()
    {};

    string getName()
    {
        return _messageName;
    }

    virtual void process()=0;
};

class MessageTestHandler: public MessageHandler
{
public:
    MessageTestHandler(string name):MessageHandler(name) {};
    void process();
};

#endif
