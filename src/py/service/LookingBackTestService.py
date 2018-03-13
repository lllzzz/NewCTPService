#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Config import Config
from common.Tool import Tool
from common.MySQL import MySQL
import demjson as JSON
import random
import datetime

class LookingBackTestService():
    """docstring for TradeService"""

    def __init__(self, appFrom, startDate, endDate, isRandom = False, randomBorder = 2):
        self.config = Config.get()
        self.db = MySQL()

        self.appFrom = appFrom
        self.appConfig = self.config['app'][appFrom]
        self.iids = self.appConfig['iids']
        self.isRandom = isRandom
        self.randomBorder = randomBorder
        self.historyDate = startDate
        self.endDate = endDate

        moduleName = self.appConfig['module']
        className = self.appConfig['class']
        moduleMeta = __import__(moduleName, globals(), locals(), [className])
        classMeta = getattr(moduleMeta, className)
        self.model = classMeta(appFrom, self)

        self.tradeInfo = {}
        self.tradeCallbackTick = {}
        self.tradeIds = []


    def trade(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.appFrom)

        r = random.randint(1, 10) if self.isRandom else 10
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'trade', 'r': r}
        if r <= self.randomBorder:
            self.tradeCallbackTick[tradeId] = random.randint(0, 10)
        self.tradeIds.append(tradeId)
        return tradeId



    def fak(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.appFrom)
        r = random.randint(1, 10) if self.isRandom else 10
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'fak', 'r': r}
        self.tradeIds.append(tradeId)
        return tradeId


    def fok(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.appFrom)
        r = random.randint(1, 10) if self.isRandom else 10
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'fok', 'r': r}
        self.tradeIds.append(tradeId)
        return tradeId



    def ioc(self, iid, price, volume, isOpen, isBuy, isToday):
        tradeId = Tool.getTradeId(self.appFrom)
        self.tradeCallbackTick[tradeId] = 0
        self.tradeInfo[tradeId] = {'tradeId': tradeId, 'price': price, 'volume': volume, 'type': 'ioc', 'isBuy': isBuy}
        self.tradeIds.append(tradeId)
        return tradeId


    def cancel(self, iid, tradeId):
        self.model.tradeCancel(tradeId)
        self.tradeIds.remove(tradeId)

    def getDataBatch(self, iid, date):
        sql = '''
            SELECT * FROM `tick_%s` WHERE `time` LIKE '%s%%'  ORDER BY `time`, `msec`
        ''' % (iid, date)
        isOK, data = self.db.getAll(sql)
        return data

    def getData(self, date):
        if len(self.iids) == 1:
            iid = self.iids[0]
            data = self.getDataBatch(filter(lambda x:x not in '0123456789', iid), date)
        # for iid in self.iids:
        return data

    def run(self):

        while True:

            if (self.endDate - self.historyDate).total_seconds() < 86400: break

            ticks = self.getData(self.historyDate.strftime("%Y%m%d"))
            self.historyDate = self.historyDate + datetime.timedelta(days=1)

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






