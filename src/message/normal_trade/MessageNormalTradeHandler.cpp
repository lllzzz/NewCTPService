#include "../MessageHandler.h"
#include "../../trade/TdSpi.h"

bool MessageNormalTradeHandler::process(string data)
{
    LOG(INFO) << "PROCESS" << "|" << data;

    Json::Reader reader;
    Json::Value jsonData;

    string id = "";
    if(reader.parse(data, jsonData)) {
        id = jsonData["id"].asString();
    } else {
        LOG(INFO) << "JSON PARSE FAILED";
        exit(1);
    }
    MessageNormalTradeProcesser* processer = new MessageNormalTradeProcesser(id);

    TdSpi* tdSpi = TdSpi::getInstance();
    tdSpi->addProcesser(processer);
    return processer->process(jsonData);

}

