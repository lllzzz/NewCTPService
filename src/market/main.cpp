#include "MdSpi.h"
#include <signal.h>
#include "../common/Config.h"

using namespace std;

string CONFIG_PATH;
CThostFtdcMdApi * mApi;
// string pidPath;

// void shutdown(int sig)
// {
//     mApi->Release();
//     remove(pidPath.c_str());
//     cout << "MarketSrv stop success!" << endl;
// }

int main(int argc, char const *argv[])
{
    // signal(30, shutdown);
    // ofstream pid;
    // pidPath = C::get("pid_path");
    // pid.open(pidPath.c_str(), ios::out);
    // pid << getpid();
    // pid.close();

    if (argc <= 1) {
        cout << "请指定配置文件路径" << endl;
        return 1;
    }
    CONFIG_PATH = argv[1];

    google::InitGoogleLogging("MARKET");
    google::SetLogDestination(google::GLOG_INFO, Config::get("path", "log").c_str());

    // 初始化交易接口
    string flowPath = Config::get("path", "mdFlow");
    string mdFront = Config::get("mdFront");
    mApi = CThostFtdcMdApi::CreateFtdcMdApi(flowPath.c_str());
    MdSpi mSpi(mApi); // 初始化回调实例
    mApi->RegisterSpi(&mSpi);
    mApi->RegisterFront(const_cast<char*>(mdFront.c_str()));
    mApi->Init();
    cout << "Market服务启动..." << endl;
    mApi->Join();

    return 0;
}
