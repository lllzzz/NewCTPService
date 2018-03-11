#include "../MessageHandler.h"
#include "../../trade/TdSpi.h"

bool MessageFAKHandler::process(string data)
{
    LOG(INFO) << "PROCESS" << "|" << data;

    Json::Reader reader;
    Json::Value jsonData;

    if(!reader.parse(data, jsonData)) {
        LOG(INFO) << "JSON PARSE FAILED";
        exit(1);
    }
    MessageFAKProcesser* processer = new MessageFAKProcesser(jsonData["id"].asString(),
        jsonData["from"].asString(), jsonData["iid"].asString());

    TdSpi* tdSpi = TdSpi::getInstance();
    tdSpi->addProcesser(processer);
    return processer->process(jsonData);

}

