#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from .ModelBase import ModelBase
import time

class DemoModel(ModelBase):
    """docstring for DemoModel"""
    def __init__(self, appFrom, tradeSrv):
        super(DemoModel, self).__init__(appFrom, tradeSrv)
        self.num = 0;


    def onTick(self, tick):

        # if self.num % 30 == 0:
        #     self.tradeIOC(tick['iid'], tick['price'], 1, True, True, True)

        tid = self.tradeFOK(tick['iid'], tick['price'], 1, True, True, True)

        # self.tradeFOK("hc1805", 3900, 1, True, True, True)
        # if self.num % 10 == 0:
        #     self.tradeIOC("hc1805", 3900, 1, True, True, True)
        self.num += 1

    def tradeSuccess(self, tradeId, dealPrice, dealVolume):
        print tradeId, dealPrice, dealVolume

    def tradeCancel(self, tradeId):
        print tradeId



