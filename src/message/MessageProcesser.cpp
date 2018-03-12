#include "MessageProcesser.h"
#include "../message/MessageSender.h"

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
void MessageTradeProcesser::traded(Json::Value data)
{
    LOG(INFO) << "RESPONSE START" << "|" << _id;
    data["iid"] = _iid;
    data["from"] = _from;
    data["id"] = _id;
    data["action"] = "TRADED";
    Json::Value req;
    req["code"] = 0;
    req["data"] = data;
    MessageSender::getInstance()->fire(_tunnelName + _from, req);
    Cache::getInstance()->push(data);
}

/**
 * {
 *     "code": 0,
 *     "data": {
 *         "action": "CANCEL",
 *         "id": "12311223123",
 *         "from": "test",
 *         "iid": "xxx",
 *     }
 * }
 */
void MessageTradeProcesser::canceled()
{
    LOG(INFO) << "RESPONSE START" << "|" << _id;
    Json::Value data;
    data["iid"] = _iid;
    data["from"] = _from;
    data["id"] = _id;
    data["action"] = "CANCELED";
    Json::Value req;
    req["code"] = 0;
    req["data"] = data;
    MessageSender::getInstance()->fire(_tunnelName + _from, req);
    Cache::getInstance()->push(data);
}

void MessageTradeProcesser::setOrderInfo(CThostFtdcOrderField *pOrder)
{
    _exchangeId = string(pOrder->ExchangeID);
    _orderSysId = string(pOrder->OrderSysID);
}

bool MessageTradeProcesser::checkOrder(CThostFtdcTradeField *pTrade)
{
    string exchangeId = string(pTrade->ExchangeID);
    string orderSysId = string(pTrade->OrderSysID);
    if (_exchangeId.compare(exchangeId) || _orderSysId.compare(orderSysId)) {
        return false;
    }
    return true;
}
