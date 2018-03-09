#include "../MessageProcesser.h"

bool MessageNormalTradeProcesser::process(Json::Value data)
{
    LOG(INFO) << "PROCESS START" << "|" << _id;
    return true;
}

void MessageNormalTradeProcesser::request(Json::Value data)
{
    LOG(INFO) << "REQUEST" << "|" << _id;
}
