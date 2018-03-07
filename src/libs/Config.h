#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <jsoncpp/json/json.h>

using namespace std;

extern string ENV;

class Config
{
private:

    static void readJsonFile();

public:
    Config();
    ~Config();

    static string get(string);
    static vector<string> getV(string);
    static string get(string, string);
};


#endif
