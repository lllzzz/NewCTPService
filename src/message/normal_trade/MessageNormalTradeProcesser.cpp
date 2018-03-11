#include "../MessageProcesser.h"
#include "../../trade/TdSpi.h"
/**
 * {
 *     "id": "12311223123",
 *     "from": "test",
 *     "iid": "xxx",
 *     "isOpen": true,
 *     "isBuy": false,
 *     "total": 1,
 *     "price": 2031,
 *     "isToday": true,
 * }
 */
bool MessageNormalTradeProcesser::process(Json::Value data)
{
    LOG(INFO) << "PROCESS START" << "|" << _id;
    TdSpi* tdSpi = TdSpi::getInstance();
    int code = tdSpi->trade(tdReqId, 
        data["iid"].asString(), 
        data["isOpen"].asBool(),
        data["isBuy"].asBool(),
        data["total"].asInt(),
        data["price"].asDouble(), 
        data["isToday"].asBool(),
        THOST_FTDC_TC_GFD,
        THOST_FTDC_VC_AV);
    LOG(INFO) << "PROCESS TRADE" << "|" << code;
    
    if (!code) {
        Json::Value req;
        req["code"] = code;
        MessageService::getInstance()->fire(RESPONSE_NORMAL_TRADE + _from, req);
        return false;
    }

    // 异步保存交易数据
    data["tdReqId"] = tdReqId;
    data["action"] = "TRADE";
    Cache::getInstance()->push(data);
    return true;
}
/**
 * {
 *     "code": 0,
 *     "data": {
 *         "id": "12311223123",
 *         "from": "test",
 *         "iid": "xxx",
 *         "dealPrice": 2391.1,
 *         "dealVolumn": 1,
 *         "tdReqId": 3,
 *         "tradeId": "123df",
 *         "tradeDate": "2018xxx",
 *         "tradeTime": "201232",
 *     }
 * }
 */
void MessageNormalTradeProcesser::response(Json::Value data)
{
    LOG(INFO) << "RESPONSE START" << "|" << _id;
    data["iid"] = _iid;
    data["from"] = _from;
    data["id"] = _id;
    Json::Value req;
    req["code"] = 0;
    req["data"] = data;
    MessageService::getInstance()->fire(RESPONSE_NORMAL_TRADE + _from, req);
    data["action"] = "TRADED";
    Cache::getInstance()->push(data);
}


void MessageNormalTradeProcesser::setOrderInfo(CThostFtdcOrderField *pOrder)
{
    _exchangeId = string(pOrder->ExchangeID);
    _orderSysId = string(pOrder->OrderSysID);
}


bool MessageNormalTradeProcesser::checkOrder(CThostFtdcTradeField *pTrade)
{
    string exchangeId = string(pTrade->ExchangeID);
    string orderSysId = string(pTrade->OrderSysID);
    if (_exchangeId.compare(exchangeId) || _orderSysId.compare(orderSysId)) {
        return false;
    }
    return true;
}


