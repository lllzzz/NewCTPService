#include "../MessageProcesser.h"
#include "../../trade/TdSpi.h"

/**
 * {
 *     "id": "12311223123",
 *     "from": "test",
 *     "iid": "xxx",
 *     "tradeId": "12333123123",
 * }
 */
bool MessageCancelProcesser::process(Json::Value data)
{
    LOG(INFO) << "PROCESS START" << "|" << _id;
    TdSpi* tdSpi = TdSpi::getInstance();
    int code = tdSpi->cancel(data["tradeId"].asString());
    LOG(INFO) << "PROCESS CANCEL" << "|" << code;

    // if (!code) {
        // Json::Value req;
        // req["code"] = code;
        // MessageService::getInstance()->fire(RESPONSE_CANCEL + _from, req);
        // return false;
    // }

    // 异步保存交易数据
    data["tdReqId"] = tdReqId;
    data["action"] = "CANCEL";
    Cache::getInstance()->push(data);
    return true;
}
