#include "../MessageHandler.h"
#include "../../trade/TdSpi.h"

bool MessageIOCHandler::process(string data)
{
    LOG(INFO) << "PROCESS" << "|" << data;

    Json::Reader reader;
    Json::Value jsonData;

    if(!reader.parse(data, jsonData)) {
        LOG(INFO) << "JSON PARSE FAILED";
        exit(1);
    }
    LOG(INFO) << jsonData["from"] << jsonData["iid"];
    MessageIOCProcesser* processer = new MessageIOCProcesser(jsonData["id"].asString(),
        jsonData["from"].asString(), jsonData["iid"].asString());

    TdSpi* tdSpi = TdSpi::getInstance();
    tdSpi->addProcesser(processer);
    return processer->process(jsonData);

}

