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
bool MessageFOKProcesser::process(Json::Value data)
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
        THOST_FTDC_TC_IOC,
        THOST_FTDC_VC_CV);
    LOG(INFO) << "PROCESS TRADE" << "|" << code;

    // if (!code) {
    //     Json::Value req;
    //     req["code"] = code;
    //     MessageService::getInstance()->fire(_tunnelName + _from, req);
    //     return false;
    // }

    // 异步保存交易数据
    data["tdReqId"] = tdReqId;
    data["action"] = "FOK";
    Cache::getInstance()->push(data);
    return true;
}
