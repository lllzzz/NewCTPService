#include "Config.h"

C::C(){}
C::~C(){}

string C::get(string key)
{
    string path = "../etc/config.ini";
    parseIniFile(path);
    string val = getOptionToString(key);
    return val;
}

string C::getCh(string key)
{
    string env = C::get("env");
    string db = C::get("rds_db_" + env);
    string ch = C::get(key);
    return db + ":" + ch;
}
