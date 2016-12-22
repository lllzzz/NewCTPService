#include "MarketSpi.h"

using namespace std;

MarketSpi::MarketSpi(CThostFtdcMdApi * mdApi)
{
    _mdApi = mdApi;
    _logger = new Logger("market");

    string env = C::get("env");

    _userID = C::get("market_user_id_" + env);
    _password = C::get("market_password_" + env);
    _brokerID = C::get("market_broker_id_" + env);
    _iIDs = Lib::split(C::get("iids"), "/");
    _channel = C::getCh("channel_tick");

    int db = Lib::stoi(C::get("rds_db_" + env));
    int dbLocal = Lib::stoi(C::get("rds_db_local"));
    string host = C::get("rds_host_" + env);
    int pool = Lib::stoi(C::get("rds_pool_num"));
    Redis::initRds("m", pool, host, 6379, db);
    Redis::initRds("ml", pool, host, 6379, dbLocal);
    _reqID = 1;
}

MarketSpi::~MarketSpi()
{
    _mdApi = NULL;
    _logger->info("MarketSpi[~]");
}

void MarketSpi::OnFrontConnected()
{

    CThostFtdcReqUserLoginField reqUserLogin;

    memset(&reqUserLogin, 0, sizeof(reqUserLogin));

    strcpy(reqUserLogin.BrokerID, _brokerID.c_str());
    strcpy(reqUserLogin.UserID, _userID.c_str());
    strcpy(reqUserLogin.Password, _password.c_str());

    // 发出登陆请求
    int res = _mdApi->ReqUserLogin(&reqUserLogin, _reqID);
    _logger->request("MarketSrv[ReqUserLogin]", _reqID++, res);
}


void MarketSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID != 0) {
        _logger->error("MarketSrv[OnRspUserLogin]", pRspInfo, nRequestID, bIsLast);
    }

    _logger->push("SessionID", Lib::itos(pRspUserLogin->SessionID));
    _logger->push("TradingDay", string(pRspUserLogin->TradingDay));
    _logger->info("MarketSrv[LoginSuccess]");

    int cnt = _iIDs.size();
    char ** Instrumnet;
    Instrumnet = (char**)malloc((sizeof(char*)) * cnt);
    for (int i = 0; i < cnt; i++) {
        char * tmp = Lib::stoc(_iIDs[i]);
        Instrumnet[i] = tmp;
    }

    int res = _mdApi->SubscribeMarketData(Instrumnet, cnt);
    free(Instrumnet);
    _logger->request("MarketSrv[SubscribeMarketData]", _reqID, res);
}

void MarketSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo && pRspInfo->ErrorID != 0) {
        _logger->error("MarketSrv[OnRspSubMarketData]", pRspInfo, nRequestID, bIsLast);
    }
}

/**
 * 接收市场数据
 * 绘制K线图
 * 执行策略进行买卖
 * @param pDepthMarketData [description]
 */
void MarketSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    _saveMarketData(pDepthMarketData);
}

void MarketSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    _logger->error("MarketSrv[OnRspError]", pRspInfo, nRequestID, bIsLast);
}

void MarketSpi::_saveMarketData(CThostFtdcDepthMarketDataField *data)
{
    if (!data) return;

    Json::FastWriter writer;
    Json::Value tick;

    string iid = string(data->InstrumentID);
    tick["iid"] = iid;
    tick["price"] = data->LastPrice;
    tick["vol"] = data->Volume;
    tick["time"] = string(data->ActionDay) + "_" + string(data->UpdateTime);
    tick["msec"] = data->UpdateMillisec;
    tick["bid1"] = data->BidPrice1;
    tick["bidvol1"] = data->BidVolume1;
    tick["ask1"] = data->AskPrice1;
    tick["askvol1"] = data->AskVolume1;

    string jsonStr = writer.write(tick);
    Redis::getRds("m")->pub(_channel + iid, jsonStr);
    Redis::getRds("ml")->set("CURRENT_TICK_" + iid, Lib::dtos(data->LastPrice));
    Redis::getRds("ml")->set("UPPERLIMITPRICE_" + iid, Lib::dtos(data->UpperLimitPrice));
    Redis::getRds("ml")->set("LOWERLIMITPRICE_" + iid, Lib::dtos(data->LowerLimitPrice));
    Redis::getRds("ml")->push("Q_TICK", jsonStr);
}


