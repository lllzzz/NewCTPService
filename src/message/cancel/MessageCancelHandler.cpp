#include "../MessageHandler.h"
#include "../../trade/TdSpi.h"

bool MessageCancelHandler::process(string data)
{
    LOG(INFO) << "PROCESS" << "|" << data;

    Json::Reader reader;
    Json::Value jsonData;

    if(!reader.parse(data, jsonData)) {
        LOG(INFO) << "JSON PARSE FAILED";
        exit(1);
    }
    MessageCancelProcesser* processer = new MessageCancelProcesser(jsonData["id"].asString(),
        jsonData["from"].asString(), jsonData["iid"].asString());

    // TdSpi* tdSpi = TdSpi::getInstance();
    // tdSpi->addProcesser(processer);
    return processer->process(jsonData);

}

