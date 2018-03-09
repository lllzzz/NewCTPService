#include "Tool.h"

int Tool::s2i(string str) {
    int num;
    stringstream stream(str);
    stream >> num;
    return num;
}

string Tool::d2s(double num)
{
    std::ostringstream strs;
    strs << num;
    std::string str = strs.str();
    return str;
};

string Tool::getDateTime(string format)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t t;
    t = tv.tv_sec;
    char tmp[20];
    strftime(tmp, sizeof(tmp), format.c_str(), localtime(&t));
    string s(tmp);
    return s;
};
