#include "../MessageProcesser.h"
#include "../../trade/TdSpi.h"
/**
 * {
 *     "id": "12311223123",
 *     "from": "test",
 *     "iid": "xxx",
 * }
 */
bool MessageQueryProcesser::process(Json::Value data)
{
    LOG(INFO) << "PROCESS START" << "|" << _id;
    TdSpi* tdSpi = TdSpi::getInstance();
    int code = tdSpi->query(tdReqId, data["iid"].asString());
    LOG(INFO) << "PROCESS QUERY" << "|" << code;

    // if (!code) {
    //     Json::Value req;
    //     req["code"] = code;
    //     MessageService::getInstance()->fire(RESPONSE_NORMAL_TRADE + _from, req);
    //     return false;
    // }

    return true;
}


void MessageQueryProcesser::callback(Json::Value data)
{
    LOG(INFO) << "CALLBACK QUERY";
}

