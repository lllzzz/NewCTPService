#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from .ModelBase import ModelBase

class TestModel(ModelBase):
    """docstring for DemoModel"""
    def __init__(self, appFrom, tradeSrv):
        super(TestModel, self).__init__(appFrom, tradeSrv)


    def onTick(self, tick):
        print tick
        pass


    def tradeSuccess(self, tradeId, dealPrice, dealVolume):
        pass

    def tradeCancel(self, tradeId):
        pass
