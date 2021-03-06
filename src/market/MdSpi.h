#ifndef MDSPI_H
#define MDSPI_H

#include <glog/logging.h>
#include <vector>
#include "../ctp/ThostFtdcMdApi.h"
#include "../message/MessageSender.h"
#include "../message/MessageTunnel.h"
#include "../common/Config.h"
#include "../common/Cache.h"
#include "ShareData.h"

using namespace std;

class MdSpi: public CThostFtdcMdSpi
{
private:

    CThostFtdcMdApi * _mdApi;

    int _reqID;

public:

    MdSpi(CThostFtdcMdApi *);
    ~MdSpi();

    void OnFrontConnected();
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};


#endif
