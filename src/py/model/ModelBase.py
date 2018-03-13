#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Config import Config

class ModelBase(object):
    """docstring for ModelBase"""
    def __init__(self, appFrom, tradeSrv):
        self.appFrom = appFrom
        self.tradeSrv = tradeSrv
        self.appConfig = Config.get()['app'][appFrom]

    def onTick(self, tick):
        raise Exception("on tick mast rewrite")

    def tradeSuccess(self, tradeId, dealPrice, dealVolume):
        raise Exception("trade success mast rewrite")

    def tradeCancel(self, tradeId):
        raise Exception("trade success mast rewrite")

    def tradeNormal(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.tradeSrv.trade(iid, price, volume, isOpen, isBuy, isToday)

    def tradeFAK(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.tradeSrv.fak(iid, price, volume, isOpen, isBuy, isToday)

    def tradeFOK(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.tradeSrv.fok(iid, price, volume, isOpen, isBuy, isToday)

    def tradeIOC(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.tradeSrv.ioc(iid, price, volume, isOpen, isBuy, isToday)

    def cancel(self, iid, tradeId):
        return self.tradeSrv.cancel(iid, tradeId)


