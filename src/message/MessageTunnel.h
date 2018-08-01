#ifndef MESSAGETUNNEL_H
#define MESSAGETUNNEL_H

#include <string>

using namespace std;


const string TICK = "TICK_"; // TICK_hc1809
const string CANCEL = "CANCEL";
const string NORMAL_TRADE = "NORMAL_TRADE"; // 普通下单
const string RESPONSE_NORMAL_TRADE = "RESPONSE_NORMAL_TRADE_"; // 消息反馈key_from
const string FAK = "FAK";
const string RESPONSE_FAK = "RESPONSE_FAK_";
const string FOK = "FOK";
const string RESPONSE_FOK = "RESPONSE_FOK_";
const string IOC = "IOC";
const string RESPONSE_IOC = "RESPONSE_IOC_";
const string QUERY = "QUERY";
const string RESPONSE_QUERY = "RESPONSE_QUERY_";
const string SRV_TRADE = "SRV_TRADE";
const string SRV_MARKET = "SRV_MARKET";
#endif
