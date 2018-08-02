#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from .ModelBase import ModelBase
from common.Logger import Logger

class DemoModel(ModelBase):
    """
    模型类：
    编写算法时，需要重写onTick、tradeSuccess以及tradeCancel方法
    onTick：当有Tick信号时触发
    tradeSuccess：下单成功时触发
    tradeCancel：订单撤销时触发

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    可用变量：
    self.appConfig: 应用配置，在Config配置中，appKey所对应的配置项

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    可用方法：
    self.tradeNormal(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下普通单，直到成交，触发tradeSuccess方法，
    可手动撤单，撤单成功后会触发tradeCancel方法

    self.tradeFAK(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下FAK单，触发tradeSuccess或tradeCancel方法

    self.tradeFOK(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下FOK单，触发tradeSuccess或tradeCancel方法

    self.tradeIOC(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下市价单，涨跌停情况外，触发tradeSuccess方法

    以上全部方法均返回tradeId，tradeId为此次交易的唯一订单ID，
    可用于追踪订单

    self.cancel(iid, tradeId)
    该方法用户撤单

    """
    def __init__(self, appKey, tradeSrv):
        super(DemoModel, self).__init__(appKey, tradeSrv)
        self.i = 0
        self.isBuy = True
        self.isOpen = True
        self.logger = Logger('demo', False)
        self.logger.info("INIT", self.appConfig)


    def onTick(self, tick):
        """
        tick：Dict
        {
            'iid': 'hc1808', // 合约
            'price': 3400, // 最新价
            'volume': 340, // 数量
            'bid1Price': 3333,
            'bid1Volume': 300,
            'ask1Price': 3322,
            'ask1Volume': 33,
            'time': '20180312 11:23:44', // 时间
            'msec': 500, // 毫秒
        }
        """
        self.logger.info("TICK", tick)
        self.i += 1
        if self.i % 5 == 0:
            tradeId = self.tradeIOC(tick['iid'], tick['price'],  1, self.isOpen, self.isBuy, True)
            self.isOpen = not self.isOpen
            self.isBuy = not self.isBuy
            self.logger.info("TRADEID", tradeId)
        pass


    def tradeSuccess(self, tradeId, dealPrice, dealVolume):
        """
        tradeId: String 订单号
        dealPrice: Float 成交价
        dealVolume: Int 成交量
        """
        self.logger.info("SUCCESS", tradeId, dealPrice, dealVolume)

    def tradeCancel(self, tradeId):
        self.logger.info("CANCEL", tradeId)

