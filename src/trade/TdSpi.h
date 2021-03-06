#ifndef TDSPI_H
#define TDSPI_H

#include <algorithm>
#include "../ctp/ThostFtdcTraderApi.h"
#include "../common/Config.h"
#include "../common/Tool.h"
#include "../message/MessageService.h"
#include "../message/MessageProcesser.h"

#define CODE_SUCCESS 0
#define CODE_ERR_ORDER_EXIST 1001
#define CODE_ERR_ORDER_NOT_EXIST 1002

#define SAVE_KEY_TRADE "TRADE";


class TdSpi : public CThostFtdcTraderSpi
{
private:

    TdSpi();

    CThostFtdcTraderApi * _tApi;

    static TdSpi* instance;

    std::map<int, MessageTradeProcesser*> _tradeProcesserMap; // 交易processer
    std::map<int, MessageProcesser*> _processerMap; // 其他processer
    std::map<string, int> _tdReqIdMap;

    int _reqId;

    string _brokerId;
    string _userId;
    vector<string> _iids;

    int _frontId;
    int _sessionId;

    int _maxOrderRef;

    // 清理MessageProcesser对象
    void _clearProcesser(int);


    // 构造订单
    CThostFtdcInputOrderField _createOrder(int, string, bool, int, double,
        TThostFtdcOffsetFlagEnType, // 开平标志
        TThostFtdcHedgeFlagEnType = THOST_FTDC_HFEN_Speculation, // 投机套保标志
        TThostFtdcOrderPriceTypeType = THOST_FTDC_OPT_LimitPrice, // 报单价格条件
        TThostFtdcTimeConditionType = THOST_FTDC_TC_IOC, // 有效期类型
        TThostFtdcVolumeConditionType = THOST_FTDC_VC_CV, //成交量类型
        TThostFtdcContingentConditionType = THOST_FTDC_CC_Immediately// 触发条件
    );


public:

    ~TdSpi();

    static TdSpi* getInstance();

    void addProcesser(MessageTradeProcesser*);
    void addProcesser(MessageProcesser*);

    // 初始化回调接口
    void OnFrontConnected();
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    // // 下单/撤单
    int trade(int tdReqId, string iid,
        bool isOpen, bool isBuy, int total, double price, bool isToday,
        TThostFtdcTimeConditionType timeCondition,
        TThostFtdcVolumeConditionType volumeCondition); // ReqOrderInsert
    int cancel(int tdReqId, string tradeId); // ReqOrderAction

    void OnRtnOrder(CThostFtdcOrderField *pOrder);
    void OnRtnTrade(CThostFtdcTradeField *pTrade);

    // // 手续费查询
    // void qryCommissionRate(int, string); // ReqQryInstrumentCommissionRate?ReqQryOptionInstrCommRate?
    // void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    // 持仓查询
    int query(int tdReqId, string iid); // ReqQryInvestorPosition
    void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    // // 异常
    void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
    void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnFrontDisconnected(int nReason);
    void OnHeartBeatWarning(int nTimeLapse);

};

#endif
