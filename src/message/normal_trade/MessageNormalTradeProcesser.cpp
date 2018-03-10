#include "../MessageProcesser.h"
#include "../../trade/TdSpi.h"

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
        MessageService::getInstance()->fire(REQUEST_NORMAL_TRADE, req);
        return false;
    }

    // 异步保存交易数据
    data["tdReqId"] = tdReqId;
    Cache::getInstance()->push(data);
    return true;
}

void MessageNormalTradeProcesser::request(Json::Value data)
{
    LOG(INFO) << "REQUEST" << "|" << _id;
}
