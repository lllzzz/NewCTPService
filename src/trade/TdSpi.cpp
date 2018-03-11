#include "TdSpi.h"

TdSpi* TdSpi::instance = NULL;

TdSpi* TdSpi::getInstance()
{
    if (NULL == instance)
        instance = new TdSpi();
    return instance;
}

void TdSpi::addProcesser(MessageProcesser* processer)
{
    processer->tdReqId = _maxOrderRef++;
    _processerMap[processer->tdReqId] = processer;
    _tdReqIdMap[processer->getId()] = processer->tdReqId;
}

void TdSpi::_clearProcesser(int reqId)
{
    _processerMap.erase(reqId);
}


TdSpi::TdSpi()
{
    _reqId = 1;
    _processerMap = map<int, MessageProcesser*>();
    _tdReqIdMap = map<string, int>();

    string tdFlow = Config::get("path", "tdFlow");
    string tdFront = Config::get("tdFront");

    // 初始化交易接口
    _tApi = CThostFtdcTraderApi::CreateFtdcTraderApi(tdFlow.c_str());
    _tApi->RegisterSpi(this);
    _tApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    _tApi->SubscribePublicTopic(THOST_TERT_QUICK);

    _tApi->RegisterFront(const_cast<char*>(tdFront.c_str()));
    _tApi->Init();

}

void TdSpi::OnFrontConnected()
{
    LOG(INFO) << "On Connected";

    _brokerId = Config::get("brokerId");
    _userId = Config::get("userId");
    string passwd = Config::get("passwd");

    // 登录
    CThostFtdcReqUserLoginField req = {0};
    strcpy(req.BrokerID, _brokerId.c_str());
    strcpy(req.UserID, _userId.c_str());
    strcpy(req.Password, passwd.c_str());

    int res = _tApi->ReqUserLogin(&req, _reqId++);

    LOG(INFO) << "Request Login" << "|" << _reqId << "|" << res;
}


void TdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG(INFO) << "On Login";

    if (pRspInfo && pRspInfo->ErrorID != 0) {
        LOG(INFO) << "ERROR" << "|"
            << pRspInfo->ErrorID << "|"
            << pRspInfo->ErrorMsg << "|"
            << nRequestID << "|"
            << bIsLast;
        exit(1);
    }

    _frontId     = pRspUserLogin->FrontID;
    _sessionId   = pRspUserLogin->SessionID;
    _maxOrderRef = atoi(pRspUserLogin->MaxOrderRef);

    LOG(INFO) << "User Info" << "|"
        << _frontId << "|"
        << _sessionId << "|"
        << _maxOrderRef;

    // 确认
    CThostFtdcSettlementInfoConfirmField req = {0};
    strcpy(req.BrokerID, _brokerId.c_str());
    strcpy(req.InvestorID, _userId.c_str());
    strcpy(req.ConfirmDate, Tool::getDateTime("%Y%m%d").c_str());
    strcpy(req.ConfirmTime, Tool::getDateTime("%H:%M:%S").c_str());

    int res = _tApi->ReqSettlementInfoConfirm(&req, _reqId++);

    LOG(INFO) << "Request Confirm" << "|" << _reqId << "|" << res;
}


void TdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG(INFO) << "On Confirm";

    if (pRspInfo && pRspInfo->ErrorID != 0) {
        LOG(INFO) << "ERROR" << "|"
            << pRspInfo->ErrorID << "|"
            << pRspInfo->ErrorMsg << "|"
            << nRequestID << "|"
            << bIsLast;
        exit(1);
    }

    LOG(INFO) << "Confirm Info" << "|"
        << pSettlementInfoConfirm->ConfirmDate << "|"
        << pSettlementInfoConfirm->ConfirmTime;

    // _confirmDate = string(pSettlementInfoConfirm->ConfirmDate);
}

// bool TdSpi::_checkReqExist(int tdReqId)
// {
//     std::vector<int>::iterator it = find(_reqIdVec.begin(), _reqIdVec.end(), tdReqId);
//     if (it != _reqIdVec.end()) {
//         return true;
//     }
//     return false;

// }

int TdSpi::trade(int tdReqId, string iid, 
    bool isOpen, bool isBuy, int total, double price, bool isToday, 
    TThostFtdcTimeConditionType timeCondition,
    TThostFtdcVolumeConditionType volumeCondition)
{

    LOG(INFO) << "TRADE" << "|"
        << tdReqId << "|"
        << iid << "|"
        << isOpen << "|"
        << isBuy << "|"
        << total << "|"
        << price << "|"
        << isToday << "|"
        << timeCondition << "|"
        << volumeCondition;

    // if (_isExistOrder(appKey, orderID)) {
    //     _rspMsg(appKey, CODE_ERR_ORDER_EXIST, "不要重复提交订单", orderID);
    //     return;
    // }
    // if (type == ORDER_TYPE_NORMAL) { // 普通单有可能撤单，所以要判断撤单限制
    //     int cancelTimes = _getCancelTimes();
    //     if (cancelTimes >= _cancelTimesMax) {
    //         Redis::getRds("t")->set("CAN_CANCEL", "0");
    //         _rspMsg(appKey, CODE_ERR_ORDER_EXIST, "撤单已达上限，不能再下单", orderID);
    //         return;
    //     }
    // }
    // _initOrder(appKey, orderID, iid);

    TThostFtdcOffsetFlagEnType closeFlag = isToday ? THOST_FTDC_OFEN_CloseToday : THOST_FTDC_OFEN_Close;
    TThostFtdcOffsetFlagEnType flag = isOpen ? THOST_FTDC_OFEN_Open : closeFlag;
    TThostFtdcContingentConditionType condition = THOST_FTDC_CC_Immediately;
    // TThostFtdcTimeConditionType timeCondition = THOST_FTDC_TC_GFD;
    // TThostFtdcVolumeConditionType volumeCondition = THOST_FTDC_VC_AV;
    // TThostFtdcOrderPriceTypeType priceType = THOST_FTDC_OPT_LimitPrice;

    // if (type == ORDER_TYPE_FOK) {
    //     timeCondition = THOST_FTDC_TC_IOC;
    //     volumeCondition = THOST_FTDC_VC_CV;
    // }

    // if (type == ORDER_TYPE_FAK) {
    //     timeCondition = THOST_FTDC_TC_IOC;
    //     volumeCondition = THOST_FTDC_VC_AV;
    // }

    // if (type == ORDER_TYPE_IOC) {
    //     if (isBuy) {
    //         string upper = Redis::getRds("tl")->get("UPPERLIMITPRICE_" + iid);
    //         price = Lib::stod(upper);
    //     } else {
    //         string lower = Redis::getRds("tl")->get("LOWERLIMITPRICE_" + iid);
    //         price = Lib::stod(lower);
    //     }
    // }


    CThostFtdcInputOrderField order = _createOrder(tdReqId, iid, isBuy, total, price, flag,
            THOST_FTDC_HFEN_Speculation, THOST_FTDC_OPT_LimitPrice, timeCondition, volumeCondition, condition);

    int res = _tApi->ReqOrderInsert(&order, tdReqId);
    LOG(INFO) << "API TRADE" << "|" << tdReqId << "|" << res;
    if (res != 0) return res;

}


void TdSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    LOG(INFO) << "ON ORDER";
    if (!pOrder) {
        LOG(INFO) << "ORDER EMPTY";
        exit(1);
    }
    if (pOrder->FrontID != _frontId || pOrder->SessionID != _sessionId) return;

    LOG(INFO) << "ORDER INFO" << "|"
        << pOrder->OrderRef << "|"
        << pOrder->InstrumentID << "|"
        << pOrder->OrderStatus << "|"
        << pOrder->VolumeTotalOriginal << "|"
        << pOrder->VolumeTraded << "|"
        << pOrder->VolumeTotal << "|"
        << pOrder->ZCETotalTradedVolume << "|"
        << pOrder->OrderSysID;

    MessageProcesser* processer = _processerMap[Tool::s2i(string(pOrder->OrderRef))];
    processer->setOrderInfo(pOrder);
    if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled) {
        processer->response((Json::Value*)NULL);
    }

    // // log
    // _logger->push("appKey", Lib::itos(info.appKey));
    // _logger->push("iid", string(pOrder->InstrumentID));
    // _logger->push("orderID", Lib::itos(info.orderID));
    // _logger->push("orderRef", string(pOrder->OrderRef));
    // _logger->push("orderStatus", Lib::ctos(pOrder->OrderStatus));
    // _logger->push("VolumeTotalOriginal", Lib::itos(pOrder->VolumeTotalOriginal));
    // _logger->push("VolumeTraded", Lib::itos(pOrder->VolumeTraded));
    // _logger->push("VolumeTotal", Lib::itos(pOrder->VolumeTotal));
    // _logger->push("ZCETotalTradedVolume", Lib::itos(pOrder->ZCETotalTradedVolume));
    // _logger->push("OrderSysID", string(pOrder->OrderSysID));
    // _logger->info("TdSpi[OnRtnOrder]");

    // _updateOrder(orderRef, pOrder);

    // if (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) {
    //     _onOrder(pOrder);
    // } else {
    //     _onCancel(pOrder);
    // }
}


// void TdSpi::_onOrder(CThostFtdcOrderField *pOrder)
// {
//     int orderRef = Lib::stoi(string(pOrder->OrderRef));
//     OrderInfo info = _getOrderByRef(orderRef);

//     Json::FastWriter writer;
//     Json::Value data;

//     string time = Lib::getDate("%Y/%m/%d-%H:%M:%S", true);
//     data["type"] = "order";
//     data["appKey"] = info.appKey;
//     data["iid"] = pOrder->InstrumentID;
//     data["orderID"] = info.orderID;
//     data["frontID"] = _frontId;
//     data["sessionID"] = _sessionId;
//     data["orderRef"] = orderRef;
//     data["insertDate"] = pOrder->InsertDate;
//     data["insertTime"] = pOrder->InsertTime;
//     data["localTime"] = time;
//     data["orderStatus"] = pOrder->OrderStatus;
//     data["todoVol"] = pOrder->VolumeTotal;
//     data["orderSysID"] = pOrder->OrderSysID;

//     std::string qStr = writer.write(data);
//     Redis::getRds("tl")->push("Q_TRADE", qStr); // 记录本地数据
// }


void TdSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    LOG(INFO) << "ON TRADE";
    if (!pTrade) {
        LOG(INFO) << "TRADE EMPTY";
        exit(1);
    }

    MessageProcesser* processer = _processerMap[Tool::s2i(string(pTrade->OrderRef))];
    if (!processer->checkOrder(pTrade)) {
        return;
    }
    // if (strcmp(info.eID, pTrade->ExchangeID) != 0 ||
    //     strcmp(info.oID, pTrade->OrderSysID) != 0)
    // { // 不是我的订单
    //     return;
    // }
    LOG(INFO) << "TRADE INFO" << "|"
        << pTrade->OrderRef << "|"
        << pTrade->InstrumentID << "|"
        << pTrade->Price << "|"
        << pTrade->TradeID << "|"
        << pTrade->TradeDate << "|"
        << pTrade->TradeTime << "|"
        << pTrade->ExchangeID << "|"
        << pTrade->Volume;

    Json::Value data;
    data["dealPrice"] = pTrade->Price;
    data["dealVolumn"] = pTrade->Volume;
    data["tdReqId"] = pTrade->OrderRef;
    data["tradeId"] = pTrade->TradeID;
    data["tradeDate"] = pTrade->TradeDate;
    data["tradeTime"] = pTrade->TradeTime;
    // data["exchangeID"] = pTrade->ExchangeID;
    processer->response(data);

    // data["type"] = "traded";
    // data["iid"] = pTrade->InstrumentID;
    // data["orderID"] = info.orderID;
    // data["realPrice"] = pTrade->Price;
    // data["successVol"] = pTrade->Volume;
    // _rspMsg(info.appKey, CODE_SUCCESS, "成功", info.orderID, &data);

    // string time = Lib::getDate("%Y/%m/%d-%H:%M:%S", true);
    // data["appKey"] = info.appKey;
    // data["orderRef"] = pTrade->OrderRef;
    // data["frontID"] = _frontId;
    // data["sessionID"] = _sessionId;
    // data["tradeDate"] = pTrade->TradeDate;
    // data["tradeTime"] = pTrade->TradeTime;
    // data["localTime"] = time;
    // data["successVol"] = pTrade->Volume;
    // data["tradeID"] = pTrade->TradeID;

    // Json::FastWriter writer;
    // std::string qStr = writer.write(data);
    // Redis::getRds("tl")->push("Q_TRADE", qStr); // 记录本地数据
}


int TdSpi::cancel(std::string id)
{
    LOG(INFO) << "CANCEL" << "|" << id;
    int tdReqId = _tdReqIdMap[id];
    MessageProcesser* tradeProcesser = _processerMap[tdReqId];

    CThostFtdcInputOrderActionField req = {0};

    ///投资者代码
    strncpy(req.InvestorID, _userId.c_str(), sizeof(TThostFtdcInvestorIDType));
    ///报单引用
    strncpy(req.OrderRef, Tool::i2s(tdReqId).c_str(), sizeof(TThostFtdcOrderRefType));
    ///前置编号
    req.FrontID = _frontId;
    ///会话编号
    req.SessionID = _sessionId;
    ///合约代码
    strncpy(req.InstrumentID, tradeProcesser->getIid().c_str(), sizeof(TThostFtdcInstrumentIDType));
    ///操作标志
    req.ActionFlag = THOST_FTDC_AF_Delete;


    int res = _tApi->ReqOrderAction(&req, tdReqId);
    LOG(INFO) << "API CANCEL" << "|" << tdReqId << "|" << res;
    if (res != 0) return res;

    return 0;
}


// void TdSpi::_onCancel(CThostFtdcOrderField *pOrder)
// {
//     int orderRef = Lib::stoi(string(pOrder->OrderRef));
//     OrderInfo info = _getOrderByRef(orderRef);

//     // log
//     _logger->push("appKey", Lib::itos(info.appKey));
//     _logger->push("iid", string(pOrder->InstrumentID));
//     _logger->push("orderID", Lib::itos(info.orderID));
//     _logger->push("orderRef", string(pOrder->OrderRef));
//     _logger->push("limitPrice", Lib::dtos(pOrder->LimitPrice));
//     _logger->info("TdSpi[onCancel]");

//     Json::Value data;

//     data["type"] = "canceled";
//     data["iid"] = pOrder->InstrumentID;
//     data["orderID"] = info.orderID;
//     data["price"] = pOrder->LimitPrice;
//     data["cancelVol"] = pOrder->VolumeTotal;

//     _rspMsg(info.appKey, CODE_SUCCESS, "成功", info.orderID, &data);

//     string time = Lib::getDate("%Y/%m/%d-%H:%M:%S", true);
//     data["appKey"] = info.appKey;
//     data["orderRef"] = pOrder->OrderRef;
//     data["frontID"] = _frontId;
//     data["sessionID"] = _sessionId;
//     data["insertDate"] = pOrder->InsertDate;
//     data["insertTime"] = pOrder->InsertTime;
//     data["localTime"] = time;
//     data["orderStatus"] = pOrder->OrderStatus;
//     data["currentTick"] = Redis::getRds("tl")->get("CURRENT_TICK_" + string(pOrder->InstrumentID));
//     data["cancelVol"] = pOrder->VolumeTotal;

//     Json::FastWriter writer;
//     std::string qStr = writer.write(data);
//     Redis::getRds("tl")->push("Q_TRADE", qStr); // 记录本地数据
// }


// void TdSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
// {
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnRspOrderInsert]", pRspInfo, nRequestID, bIsLast);
//     }
//     int orderRef = Lib::stoi(string(pInputOrder->OrderRef));
//     OrderInfo info = _getOrderByRef(orderRef);
//     if (!info.orderID) return;

//     // log
//     _logger->push("appKey", Lib::itos(info.appKey));
//     _logger->push("iid", string(pInputOrder->InstrumentID));
//     _logger->push("orderID", Lib::itos(info.orderID));
//     _logger->push("orderRef", string(pInputOrder->OrderRef));
//     _logger->push("errNo", Lib::itos(pRspInfo->ErrorID));
//     _logger->info("TdSpi[OnRspOrderInsert]");
// }

// void TdSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
// {
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnErrRtnOrderInsert]", pRspInfo, 0, 1);
//     }
//     int orderRef = Lib::stoi(string(pInputOrder->OrderRef));
//     OrderInfo info = _getOrderByRef(orderRef);
//     if (!info.orderID) return;

//     // log
//     _logger->push("appKey", Lib::itos(info.appKey));
//     _logger->push("iid", string(pInputOrder->InstrumentID));
//     _logger->push("orderID", Lib::itos(info.orderID));
//     _logger->push("orderRef", string(pInputOrder->OrderRef));
//     _logger->push("errNo", Lib::itos(pRspInfo->ErrorID));
//     _logger->info("TdSpi[OnErrRtnOrderInsert]");

//     _rspMsg(info.appKey, pRspInfo->ErrorID, Lib::g2u(string(pRspInfo->ErrorMsg)), info.orderID);
// }

// void TdSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
// {
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnRspOrderAction]", pRspInfo, nRequestID, bIsLast);
//     }
//     int orderRef = Lib::stoi(string(pInputOrderAction->OrderRef));
//     OrderInfo info = _getOrderByRef(orderRef);
//     if (!info.orderID) return;
//     if (pInputOrderAction->SessionID != _sessionId || pInputOrderAction->FrontID != _frontId) return;

//     // log
//     _logger->push("appKey", Lib::itos(info.appKey));
//     _logger->push("iid", string(pInputOrderAction->InstrumentID));
//     _logger->push("orderID", Lib::itos(info.orderID));
//     _logger->push("orderRef", string(pInputOrderAction->OrderRef));
//     _logger->push("errNo", Lib::itos(pRspInfo->ErrorID));
//     _logger->info("TdSpi[OnRspOrderAction]");

//     _rspMsg(info.appKey, pRspInfo->ErrorID, Lib::g2u(string(pRspInfo->ErrorMsg)), info.orderID);

// }

// void TdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
// {
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnRspError]", pRspInfo, nRequestID, bIsLast);
//     }
// }

// void TdSpi::OnFrontDisconnected(int nReason)
// {
//     _logger->push("nReason", Lib::itos(nReason));
//     _logger->info("TdSpi[OnFrontDisconnected]");
// }

// void TdSpi::OnHeartBeatWarning(int nTimeLapse)
// {
//     _logger->push("nTimeLapse", Lib::itos(nTimeLapse));
//     _logger->info("TdSpi[OnHeartBeatWarning]");
// }

// void TdSpi::_initOrder(int appKey, int orderID, string iid)
// {
//     _maxOrderRef++;
//     OrderInfo info = {0};
//     info.appKey = appKey;
//     info.orderID = orderID;
//     info.orderRef = _maxOrderRef;
//     info.iid = iid;

//     _orderInfoViaAO[appKey][orderID] = info;
//     _orderInfoViaR[_maxOrderRef] = info;
// }

// bool TdSpi::_isExistOrder(int appKey, int orderID)
// {
//     std::map<int, std::map<int, OrderInfo> >::iterator it = _orderInfoViaAO.find(appKey);
//     if (it != _orderInfoViaAO.end()) {
//         std::map<int, OrderInfo>::iterator i = it->second.find(orderID);
//         if (i != it->second.end()) return true;
//     }
//     return false;
// }

// OrderInfo TdSpi::_getOrderByRef(int orderRef)
// {
//     std::map<int, OrderInfo>::iterator it = _orderInfoViaR.find(orderRef);
//     if (it != _orderInfoViaR.end()) return it->second;
//     OrderInfo empty = {0};
//     return empty;
// }

// void TdSpi::_updateOrder(int orderRef, CThostFtdcOrderField *pOrder)
// {
//     OrderInfo info = _getOrderByRef(orderRef);
//     strcpy(info.eID, pOrder->ExchangeID);
//     strcpy(info.oID, pOrder->OrderSysID);
//     strcpy(info.iID, pOrder->InstrumentID);
//     strcpy(info.oRef, pOrder->OrderRef);
//     _orderInfoViaR[orderRef] = info;
//     _orderInfoViaAO[info.appKey][info.orderID] = info;
// }

// void TdSpi::_rspMsg(int appKey, int err, string msg, int orderID, Json::Value * data)
// {
//     Json::Value rsp;
//     rsp["err"] = err;
//     rsp["msg"] = msg;
//     rsp["orderID"] = orderID;
//     if (data) rsp["data"] = *data;

//     Json::FastWriter writer;
//     string jsonStr = writer.write(rsp);
//     Redis::getRds("t")->pub(_channelRsp + Lib::itos(appKey), jsonStr);
// }

CThostFtdcInputOrderField TdSpi::_createOrder(int tdReqId, string instrumnetId, bool isBuy, int total, double price,
    // double stopPrice,
    TThostFtdcOffsetFlagEnType offsetFlag, // 开平标志
    TThostFtdcHedgeFlagEnType hedgeFlag, // 投机套保标志
    TThostFtdcOrderPriceTypeType priceType, // 报单价格条件
    TThostFtdcTimeConditionType timeCondition, // 有效期类型
    TThostFtdcVolumeConditionType volumeCondition, //成交量类型
    TThostFtdcContingentConditionType contingentCondition// 触发条件
    )
{
    CThostFtdcInputOrderField order = {0};

    strcpy(order.BrokerID, _brokerId.c_str()); ///经纪公司代码
    strcpy(order.InvestorID, _userId.c_str()); ///投资者代码
    strcpy(order.InstrumentID, instrumnetId.c_str()); ///合约代码
    strcpy(order.UserID, _userId.c_str()); ///用户代码
    // strcpy(order.ExchangeID, "SHFE"); ///交易所代码

    order.MinVolume = 1;///最小成交量
    order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;///强平原因
    order.IsAutoSuspend = 0;///自动挂起标志
    order.UserForceClose = 0;///用户强评标志
    order.IsSwapOrder = 0;///互换单标志

    order.Direction = isBuy ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell; ///买卖方向
    order.VolumeTotalOriginal = total;///数量
    order.LimitPrice = price;///价格
    order.StopPrice = 0;///止损价
    if (contingentCondition != THOST_FTDC_CC_Immediately) {
        order.StopPrice = price;///止损价
    }

    ///组合开平标志
    //THOST_FTDC_OFEN_Open 开仓
    //THOST_FTDC_OFEN_Close 平仓
    //THOST_FTDC_OFEN_ForceClose 强平
    //THOST_FTDC_OFEN_CloseToday 平今
    //THOST_FTDC_OFEN_CloseYesterday 平昨
    //THOST_FTDC_OFEN_ForceOff 强减
    //THOST_FTDC_OFEN_LocalForceClose 本地强平
    order.CombOffsetFlag[0] = offsetFlag;
    if (THOST_FTDC_OFEN_ForceClose == offsetFlag) {
        order.ForceCloseReason = THOST_FTDC_FCC_Other; // 其他
        order.UserForceClose = 1;
    }

    ///组合投机套保标志
    // THOST_FTDC_HFEN_Speculation 投机
    // THOST_FTDC_HFEN_Arbitrage 套利
    // THOST_FTDC_HFEN_Hedge 套保
    order.CombHedgeFlag[0] = hedgeFlag;

    ///报单价格条件
    // THOST_FTDC_OPT_AnyPrice 任意价
    // THOST_FTDC_OPT_LimitPrice 限价
    // THOST_FTDC_OPT_BestPrice 最优价
    // THOST_FTDC_OPT_LastPrice 最新价
    // THOST_FTDC_OPT_LastPricePlusOneTicks 最新价浮动上浮1个ticks
    // THOST_FTDC_OPT_LastPricePlusTwoTicks 最新价浮动上浮2个ticks
    // THOST_FTDC_OPT_LastPricePlusThreeTicks 最新价浮动上浮3个ticks
    // THOST_FTDC_OPT_AskPrice1 卖一价
    // THOST_FTDC_OPT_AskPrice1PlusOneTicks 卖一价浮动上浮1个ticks
    // THOST_FTDC_OPT_AskPrice1PlusTwoTicks 卖一价浮动上浮2个ticks
    // THOST_FTDC_OPT_AskPrice1PlusThreeTicks 卖一价浮动上浮3个ticks
    // THOST_FTDC_OPT_BidPrice1 买一价
    // THOST_FTDC_OPT_BidPrice1PlusOneTicks 买一价浮动上浮1个ticks
    // THOST_FTDC_OPT_BidPrice1PlusTwoTicks 买一价浮动上浮2个ticks
    // THOST_FTDC_OPT_BidPrice1PlusThreeTicks 买一价浮动上浮3个ticks
    // THOST_FTDC_OPT_FiveLevelPrice 五档价
    order.OrderPriceType = priceType;

    ///有效期类型
    // THOST_FTDC_TC_IOC 立即完成，否则撤销
    // THOST_FTDC_TC_GFS 本节有效
    // THOST_FTDC_TC_GFD 当日有效
    // THOST_FTDC_TC_GTD 指定日期前有效
    // THOST_FTDC_TC_GTC 撤销前有效
    // THOST_FTDC_TC_GFA 集合竞价有效
    order.TimeCondition = timeCondition;

    ///成交量类型
    // THOST_FTDC_VC_AV 任何数量
    // THOST_FTDC_VC_MV 最小数量
    // THOST_FTDC_VC_CV 全部数量
    order.VolumeCondition = volumeCondition;

    ///触发条件
    // THOST_FTDC_CC_Immediately 立即
    // THOST_FTDC_CC_Touch 止损
    // THOST_FTDC_CC_TouchProfit 止赢
    // THOST_FTDC_CC_ParkedOrder 预埋单
    // THOST_FTDC_CC_LastPriceGreaterThanStopPrice 最新价大于条件价
    // THOST_FTDC_CC_LastPriceGreaterEqualStopPrice 最新价大于等于条件价
    // THOST_FTDC_CC_LastPriceLesserThanStopPrice 最新价小于条件价
    // THOST_FTDC_CC_LastPriceLesserEqualStopPrice 最新价小于等于条件价
    // THOST_FTDC_CC_AskPriceGreaterThanStopPrice 卖一价大于条件价
    // THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 卖一价大于等于条件价
    // THOST_FTDC_CC_AskPriceLesserThanStopPrice 卖一价小于条件价
    // THOST_FTDC_CC_AskPriceLesserEqualStopPrice 卖一价小于等于条件价
    // THOST_FTDC_CC_BidPriceGreaterThanStopPrice 买一价大于条件价
    // THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 买一价大于等于条件价
    // THOST_FTDC_CC_BidPriceLesserThanStopPrice 买一价小于条件价
    // THOST_FTDC_CC_BidPriceLesserEqualStopPrice 买一价小于等于条件价
    order.ContingentCondition = contingentCondition;

    ///报单引用
    sprintf(order.OrderRef, "%d", tdReqId);

    ///请求编号
    // _reqId++;
    order.RequestID = tdReqId;

    // order.GTDDate = ;///GTD日期
    // order.BusinessUnit = ;///业务单元
    // order.InvestUnitID = ;///投资单元代码
    // order.AccountID = ;///资金账号
    // order.CurrencyID = ;///币种代码
    // order.ClientID = ;///交易编码
    // order.IPAddress = ;///IP地址
    // order.MacAddress = ;///Mac地址

    return order;
}

// TdSpi::~TdSpi()
// {
//     _tApi->RegisterSpi(NULL);
//     _tApi->Release();
//     _tApi = NULL;
//     _logger->info("TdSpi[~]");
// }



// void TdSpi::qryCommissionRate(int appKey, string iid)
// {
//     // 查询合约
//     CThostFtdcQryInstrumentCommissionRateField req = {0};
//     // CThostFtdcQryOptionInstrCommRateField req = {0};
//     strcpy(req.BrokerID, Lib::stoc(_brokerID));
//     strcpy(req.InvestorID, Lib::stoc(_userID));
//     strcpy(req.InstrumentID, iid.c_str());

//     int res = _tApi->ReqQryInstrumentCommissionRate(&req, _reqId);
//     // int res = _tApi->ReqQryOptionInstrCommRate(&req, _reqId);
//     _qryReq2App[_reqId] = appKey;
//     _logger->request("TdSpi[ReqQryInstrumentCommissionRate]", _reqId++, res);
//     // _logger->request("TdSpi[ReqQryOptionInstrCommRate]", _reqId++, res);
//     if (res < 0) {
//         _rspMsg(appKey, res, "请求失败");
//     }
// }

// void TdSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
//     CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
// {
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnRspQryInstrumentCommissionRate]", pRspInfo, nRequestID, bIsLast);
//         return;
//     }

//     int appKey = _qryReq2App[nRequestID];

//     Json::Value data;

//     data["type"] = "rate";
//     data["iid"] = pInstrumentCommissionRate->InstrumentID;
//     data["openByMoney"] = pInstrumentCommissionRate->OpenRatioByMoney;
//     data["openByVol"] = pInstrumentCommissionRate->OpenRatioByVolume;
//     data["closeByMoney"] = pInstrumentCommissionRate->CloseRatioByMoney;
//     data["closeByVol"] = pInstrumentCommissionRate->CloseRatioByVolume;
//     data["closeTodayByMoney"] = pInstrumentCommissionRate->CloseTodayRatioByMoney;
//     data["closeTodayByVol"] = pInstrumentCommissionRate->CloseTodayRatioByVolume;
//     _rspMsg(appKey, CODE_SUCCESS, "成功", -1, &data);
// }

// void TdSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate,
//     CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
// {
//     cout << pOptionInstrCommRate->InstrumentID << endl;
//     cout << nRequestID << endl;
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnRspQryOptionInstrCommRate]", pRspInfo, nRequestID, bIsLast);
//         return;
//     }

//     int appKey = _qryReq2App[nRequestID];

//     Json::Value data;

//     data["type"] = "rate";
//     data["iid"] = pOptionInstrCommRate->InstrumentID;
//     data["openByMoney"] = pOptionInstrCommRate->OpenRatioByMoney;
//     data["openByVol"] = pOptionInstrCommRate->OpenRatioByVolume;
//     data["closeByMoney"] = pOptionInstrCommRate->CloseRatioByMoney;
//     data["closeByVol"] = pOptionInstrCommRate->CloseRatioByVolume;
//     data["closeTodayByMoney"] = pOptionInstrCommRate->CloseTodayRatioByMoney;
//     data["closeTodayByVol"] = pOptionInstrCommRate->CloseTodayRatioByVolume;
//     _rspMsg(appKey, CODE_SUCCESS, "成功", -1, &data);
// }


// void TdSpi::qryPosition(int appKey, string iid)
// {
//     // 查询合约
//     CThostFtdcQryInvestorPositionField req = {0};
//     strcpy(req.BrokerID, Lib::stoc(_brokerID));
//     strcpy(req.InvestorID, Lib::stoc(_userID));
//     strcpy(req.InstrumentID, iid.c_str());

//     int res = _tApi->ReqQryInvestorPosition(&req, _reqId);
//     _qryReq2App[_reqId] = appKey;
//     _logger->request("TdSpi[ReqQryInvestorPosition]", _reqId++, res);
//     if (res < 0) {
//         _rspMsg(appKey, res, "请求失败");
//     }
// }

// void TdSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition,
//     CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
// {
//     if (pRspInfo && pRspInfo->ErrorID != 0) {
//         _logger->error("TdSpi[OnRspQryInvestorPosition]", pRspInfo, nRequestID, bIsLast);
//         return;
//     }

//     int appKey = _qryReq2App[nRequestID];

//     Json::Value data;

//     std::string direct = "";
//     switch (pInvestorPosition->PosiDirection) {
//         case THOST_FTDC_PD_Net:
//             direct = "none";
//             break;
//         case THOST_FTDC_PD_Long:
//             direct = "buy";
//             break;
//         case THOST_FTDC_PD_Short:
//             direct = "sell";
//             break;
//         default:
//             break;
//     }

//     data["type"] = "position";
//     data["isLast"] = bIsLast;
//     data["iid"] = pInvestorPosition->InstrumentID;
//     data["direction"] = direct;
//     data["pos"] = pInvestorPosition->Position;
//     data["tPos"] = pInvestorPosition->TodayPosition;
//     data["openVol"] = pInvestorPosition->OpenVolume;
//     data["closeVol"] = pInvestorPosition->CloseVolume;
//     data["openAmnt"] = pInvestorPosition->OpenAmount;
//     data["closeAmnt"] = pInvestorPosition->CloseAmount;
//     _rspMsg(appKey, CODE_SUCCESS, "成功", -1, &data);
// }


// void TdSpi::_incrCancelTimes()
// {
//     Redis::getRds("tl")->incr("CANCEL_TIMES_" + _confirmDate);
// }

// int TdSpi::_getCancelTimes()
// {
//     return Lib::stoi(Redis::getRds("tl")->get("CANCEL_TIMES_" + _confirmDate));
// }
