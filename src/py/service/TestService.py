#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Locker import Locker
from common.Tool import Tool
import demjson as JSON
import random
import datetime

class TestService():
    """docstring for TradeService"""

    def __init__(self, testVersion, modelName, moduleName, className, dataSrv, isRandom = False, randomBorder = 2):

        self.modelName = modelName
        self.testVersion = testVersion
        self.dataSrv = dataSrv
        self.isRandom = isRandom
        self.randomBorder = randomBorder

        moduleMeta = __import__(moduleName, globals(), locals(), [className])
        classMeta = getattr(moduleMeta, className)
        self.model = classMeta(modelName, self)

        self.tradeInfo = {}
        self.tradeCallbackTick = {}
        self.tradeIds = []


    def trade(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.modelName)

        r = random.randint(1, 10) if self.isRandom else 10
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'trade', 'r': r}
        if r <= self.randomBorder:
            self.tradeCallbackTick[tradeId] = random.randint(0, 10)
        self.tradeIds.append(tradeId)
        return tradeId



    def fak(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.modelName)
        r = random.randint(1, 10) if self.isRandom else 10
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'fak', 'r': r}
        self.tradeIds.append(tradeId)
        return tradeId


    def fok(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.modelName)
        r = random.randint(1, 10) if self.isRandom else 10
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'fok', 'r': r}
        self.tradeIds.append(tradeId)
        return tradeId



    def ioc(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.modelName)
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'ioc', 'isBuy': isBuy}
        self.tradeIds.append(tradeId)
        return tradeId


    def cancel(self, iid, tradeId):
        self.model.tradeCancel(tradeId)
        self.tradeIds.remove(tradeId)


    def run(self):

        locker = Locker('TEST_SERVICE_RUNNING_' + self.testVersion)
        if locker.isLocking(): return

        while True:

            if (self.dataSrv.isOver()): break

            ticks = self.dataSrv.getData()

            for tick in ticks:
                self.model.onTick(tick)

                for tid in self.tradeIds:
                    if self.tradeCallbackTick[tid] == 0:

                        data = self.tradeInfo[tid]

                        if data['type'] == 'trade':
                            self.model.tradeSuccess(data['tradeId'], data['price'], data['volume'])

                        if data['type'] in ['fak', 'fok']:
                            if data['r'] <= self.randomBorder:
                                self.model.tradeCancel(data['tradeId'])
                            else:
                                self.model.tradeSuccess(data['tradeId'], data['price'], data['volume'])

                        if data['type'] == 'ioc':
                            price = data['price']
                            if self.isRandom:
                                r = random.randint(0, 10)
                                price = price + r if data['isBuy'] else price - r
                                self.model.tradeSuccess(data['tradeId'], price, data['volume'])
                            else:
                                self.model.tradeSuccess(data['tradeId'], price, data['volume'])
                        self.tradeIds.remove(tid)

                    else:
                        self.tradeCallbackTick[tid] -= 1






