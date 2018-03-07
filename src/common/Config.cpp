#include "Config.h"

Config::Config(){}
Config::~Config(){}

Json::Value json;

void Config::readJsonFile()
{
    string path = "../etc/config." + ENV + ".json";
    cout << "配置地址:" << path << endl;

    Json::Reader reader;

    std::ifstream in;
    in.open(path.c_str(), std::ios::binary);

    bool parsingSuccessful = reader.parse(in, json);
    in.close();
    if (!parsingSuccessful) {
        cout << "解析配置文件错误: " << reader.getFormattedErrorMessages() << endl;
        exit(1);
    }
}

vector<string> Config::getV(string key)
{
    if (!json) readJsonFile();
    int itemSize = json[key].size();
    if (itemSize >= 1) {
        vector<string> vec;
        for (int i = 0; i < itemSize; i++) {
            vec.push_back(json[key][i].asString());
        }
        return vec;
    } else {
        cout << "获取失败" << endl;
        exit(1);
    }
}

string Config::get(string key)
{
    if (!json) readJsonFile();
    return json[key].asString();
}

string Config::get(string section, string key)
{
    if (!json) readJsonFile();
    return json[section][key].asString();
}
