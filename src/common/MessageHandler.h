#ifndef MESSAGEHANDLE_H
#define MESSAGEHANDLE_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <jsoncpp/json/json.h>

using namespace std;

class MessageHandler
{
private:

    string _messageName;

public:
    
    MessageHandler(): _messageName("")
    {};

    ~MessageHandler() 
    {};

    string getName() 
    {
        return _messageName;
    }

    virtual void fire();
};


#endif
