#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <sstream>
#include <sys/time.h>

using namespace std;

class Tool
{
public:

    static int s2i(string);
    static double s2d(string);
    static string d2s(double);
    static string i2s(int);
    static string getDateTime(string);

    Tool() {};
    ~Tool() {};

};

#endif
