#include "../MessageProcesser.h"
#include "../../trade/TdSpi.h"
#include "../../market/ShareData.h"
/**
 * {
 *     "id": "12311223123",
 *     "from": "test",
 *     "iid": "xxx",
 *     "isOpen": true,
 *     "isBuy": false,
 *     "total": 1,
 *     "isToday": true,
 * }
 */
bool MessageIOCProcesser::process(Json::Value data)
{
    LOG(INFO) << "PROCESS START" << "|" << _id;

    double price = 0;
    if (data["isBuy"].asBool()) {
        price = ShareData::getUpperPrice(_iid);
    } else {
        price = ShareData::getLowerPrice(_iid);
    }

    TdSpi* tdSpi = TdSpi::getInstance();
    int code = tdSpi->trade(tdReqId,
        data["iid"].asString(),
        data["isOpen"].asBool(),
        data["isBuy"].asBool(),
        data["total"].asInt(),
        price,
        data["isToday"].asBool(),
        THOST_FTDC_TC_GFD,
        THOST_FTDC_VC_AV);
    LOG(INFO) << "PROCESS TRADE" << "|" << code;

    // 异步保存交易数据
    data["tdReqId"] = tdReqId;
    data["action"] = "TRADE";
    Cache::getInstance()->push(data);
    return true;
}
