#include <string>
#include <iostream>
#include <cstring>
#include "../common/Config.h"
#include <glog/logging.h>

using namespace std;

string ENV;

int main(int argc, char const *argv[])
{
    if (argc <= 1) {
        cout << "请指定环境参数" << endl;
        return 1;
    }
    ENV = argv[1];
    cout << ENV << endl;

    google::InitGoogleLogging("TEST");
    google::SetLogDestination(google::GLOG_INFO, Config::get("path", "log").c_str());
    LOG(INFO) << "TEST";

    cout << Config::get("mdFront") << endl;
    cout << Config::get("path", "log") << endl;
    cout << Config::get("abc") << endl;
    std::vector<string> arr = Config::getV("iids");
    cout << arr[0] << endl;
    cout << arr.size() << endl;

    google::ShutdownGoogleLogging();
    return 0;
}
