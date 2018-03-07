#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <jsoncpp/json/json.h>

using namespace std;

class MessageService
{
private:

public:
    MessageService();
    ~MessageService();

    void publish(string, Json::Value);
    void run();
    void listen(string, ACTIONCALLBACK)
};


#endif
