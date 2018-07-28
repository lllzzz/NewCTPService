#include "TdSpi.h"
#include "../common/Config.h"
#include "../message/MessageHandler.h"
#include "../message/MessageTunnel.h"

using namespace std;

string CONFIG_PATH;

int main(int argc, char const *argv[])
{
    if (argc <= 1) {
        cout << "请指定配置文件路径" << endl;
        return 1;
    }
    CONFIG_PATH = argv[1];

    google::InitGoogleLogging("TRADE");
    if (Tool::s2i(Config::get("is_log_std"))) {
        google::SetStderrLogging(google::GLOG_INFO);
    }
    google::SetLogDestination(google::GLOG_INFO, Config::get("path", "log").c_str());

    MessageService* msgSrv = MessageService::getInstance(SRV_TRADE);
    msgSrv->addHandler(new MessageNormalTradeHandler(NORMAL_TRADE)); // 下单
    msgSrv->addHandler(new MessageCancelHandler(CANCEL)); // 撤单
    msgSrv->addHandler(new MessageFAKHandler(FAK)); // 撤单
    msgSrv->addHandler(new MessageFOKHandler(FOK)); // 撤单
    msgSrv->addHandler(new MessageIOCHandler(IOC)); // 撤单
    msgSrv->addHandler(new MessageQueryHandler(QUERY)); // 查询
    TdSpi::getInstance();
    cout << "Trade服务启动..." << endl;
    msgSrv->run();

    return 0;
}

