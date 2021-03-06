#include "MdSpi.h"

MdSpi::MdSpi(CThostFtdcMdApi * mdApi)
{
    _mdApi = mdApi;
    _reqID = 1;
}

MdSpi::~MdSpi()
{
    _mdApi = NULL;
    LOG(INFO) << "BYE~";
}

void MdSpi::OnFrontConnected()
{
    LOG(INFO) << "On Connected";

    CThostFtdcReqUserLoginField reqUserLogin;

    memset(&reqUserLogin, 0, sizeof(reqUserLogin));

    string brokerId = Config::get("brokerId");
    string userId = Config::get("userId");
    string passwd = Config::get("passwd");

    strcpy(reqUserLogin.BrokerID, brokerId.c_str());
    strcpy(reqUserLogin.UserID, userId.c_str());
    strcpy(reqUserLogin.Password, passwd.c_str());

    // 发出登陆请求
    int res = _mdApi->ReqUserLogin(&reqUserLogin, _reqID);

    LOG(INFO) << "Request Login" << "|" << _reqID++ << "|" << res;
}


void MdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG(INFO) << "ON LOGIN";
    if (pRspInfo && pRspInfo->ErrorID != 0) {
        LOG(INFO) << "ERROR" << "|"
            << pRspInfo->ErrorID << "|"
            << pRspInfo->ErrorMsg << "|"
            << nRequestID << "|"
            << bIsLast;
        exit(1);
    }

    vector<string> iids = Config::getV("iids");

    int cnt = iids.size();
    char ** Instrumnet;
    Instrumnet = (char**)malloc((sizeof(char*)) * cnt);
    for (int i = 0; i < cnt; i++) {
        char * tmp = const_cast<char*>(iids[i].c_str());
        Instrumnet[i] = tmp;
    }

    int res = _mdApi->SubscribeMarketData(Instrumnet, cnt);
    free(Instrumnet);
    LOG(INFO) << "SUBSCRIBE DATA" << "|" << _reqID++ << "|" << res;
}


void MdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG(INFO) << "ON SUB DATA";
    if (pRspInfo && pRspInfo->ErrorID != 0) {
        LOG(INFO) << "ERROR" << "|"
            << pRspInfo->ErrorID << "|"
            << pRspInfo->ErrorMsg << "|"
            << nRequestID << "|"
            << bIsLast;
        exit(1);
    }
}

void MdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG(INFO) << "ON ERROR" << "|"
        << pRspInfo->ErrorID << "|"
        << pRspInfo->ErrorMsg << "|"
        << nRequestID << "|"
        << bIsLast;
    exit(1);
}

/**
 * 接收市场数据
 * 绘制K线图
 * 执行策略进行买卖
 * @param pDepthMarketData [description]
 */
void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    LOG(INFO) << "ON TICK";
    if (!pDepthMarketData) {
        LOG(INFO) << "EMPTY DATA";
        return;
    }

    MessageSender* msgSender = MessageSender::getInstance();

    Json::Value tick;

    string iid = string(pDepthMarketData->InstrumentID);
    tick["iid"] = iid;
    tick["price"] = pDepthMarketData->LastPrice;
    tick["volume"] = pDepthMarketData->Volume;
    tick["time"] = string(pDepthMarketData->ActionDay) + " " + string(pDepthMarketData->UpdateTime);
    tick["msec"] = pDepthMarketData->UpdateMillisec;
    tick["bidPrice1"] = pDepthMarketData->BidPrice1;
    tick["bidVolume1"] = pDepthMarketData->BidVolume1;
    tick["askPrice1"] = pDepthMarketData->AskPrice1;
    tick["askVolume1"] = pDepthMarketData->AskVolume1;

    LOG(INFO) << "TICK INFO" << "|"
        << tick["iid"] << "|"
        << tick["price"];

    msgSender->fire(TICK + iid, tick);

    ShareData::setLastPrice(iid, pDepthMarketData->LastPrice);
    ShareData::setUpperPrice(iid, pDepthMarketData->UpperLimitPrice);
    ShareData::setLowerPrice(iid, pDepthMarketData->LowerLimitPrice);

    tick["action"] = "TICK";
    Cache::getInstance()->push(tick);

}


