#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Redis import Redis
from common.Config import Config
from common.Tool import Tool
import demjson as JSON
from web.models.Model import Model
from common.Locker import Locker

class TradeService():
    """docstring for TradeService"""

    TUNNEL_TICK = 'TICK_%s'
    TUNNEL_CANCEL = 'CANCEL'
    TUNNEL_TRADE = 'NORMAL_TRADE'
    TUNNEL_RESPONSE_TRADE = 'RESPONSE_NORMAL_TRADE_%s'
    TUNNEL_FAK = 'FAK'
    TUNNEL_RESPONSE_FAK = 'RESPONSE_FAK_%s'
    TUNNEL_FOK = 'FOK'
    TUNNEL_RESPONSE_FOK = 'RESPONSE_FOK_%s'
    TUNNEL_IOC = 'IOC'
    TUNNEL_RESPONSE_IOC = 'RESPONSE_IOC_%s'

    def __init__(self, modelName, version, iids):
        self.server = Redis.get()
        self.sender = Redis.get()
        self.cache = Redis.get()
        self.config = Config.get()

        self.modelName = modelName
        self.version = version
        self.globalKey = modelName + version
        self.appConfig = self.config['app'][modelName]
        self.iids = iids

        # 获取模型
        m = Model.query.filter(Model.nick_name==modelName).first()
        if not m:
            print 'Error: model not exist'
            exit()

        className = m.class_name
        moduleName = 'model.' + className

        moduleMeta = __import__(moduleName, globals(), locals(), [className])
        classMeta = getattr(moduleMeta, className)
        self.model = classMeta(modelName, self)

    def trade(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.__trade(self.TUNNEL_TRADE, iid, price, volume, isOpen, isBuy, isToday)

    def fak(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.__trade(self.TUNNEL_FAK, iid, price, volume, isOpen, isBuy, isToday)

    def fok(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.__trade(self.TUNNEL_FOK, iid, price, volume, isOpen, isBuy, isToday)

    def ioc(self, iid, price, volume, isOpen, isBuy, isToday):
        return self.__trade(self.TUNNEL_IOC, iid, 0, volume, isOpen, isBuy, isToday)

    def cancel(self, iid, tradeId):
        data = {}
        data['id'] = Tool.getTradeId(self.modelName)
        data['from'] = self.modelName
        data['iid'] = iid
        data['tradeId'] = tradeId
        jsonData = JSON.encode(data)
        print jsonData
        self.sender.publish(self.TUNNEL_CANCEL, jsonData)


    def __trade(self, tunnel, iid, price, volume, isOpen, isBuy, isToday):
        data = {}
        data['id'] = Tool.getTradeId(self.globalKey)
        data['from'] = self.modelName
        data['iid'] = iid
        data['price'] = price
        data['total'] = volume
        data['isOpen'] = int(isOpen)
        data['isBuy'] = int(isBuy)
        data['isToday'] = int(isToday)
        jsonData = JSON.encode(data)
        print jsonData
        self.sender.publish(tunnel, jsonData)
        return data['id']

    def run(self):

        locker = Locker('ONLINE_SERVICE_RUNNING_' + self.globalKey + '_' + ','.join(self.iids))
        if locker.isLocking(): return

        srv = self.server.pubsub()
        tunnel_tick = []
        for iid in self.iids:
            tunnel_tick.append(self.TUNNEL_TICK % iid)

        tunnel_rsp = [
            self.TUNNEL_RESPONSE_TRADE % self.globalKey,
            self.TUNNEL_RESPONSE_FAK % self.globalKey,
            self.TUNNEL_RESPONSE_FOK % self.globalKey,
            self.TUNNEL_RESPONSE_IOC % self.globalKey,
        ]

        srv.subscribe(tunnel_tick + tunnel_rsp)
        for msg in srv.listen():
            print msg
            if msg['type'] != 'message': continue
            tunnel = msg['channel']
            data = JSON.decode(msg['data'])

            if tunnel in tunnel_tick:
                self.model.onTick(data)

            if tunnel in tunnel_rsp:
                code = data['code']
                data = data['data']
                action = data['action']
                tradeId = data['id']

                if action == 'TRADED':
                    dealPrice = data['dealPrice']
                    dealVolume = data['dealVolume']
                    self.model.tradeSuccess(tradeId, dealPrice, dealVolume)

                if action == 'CANCELED':
                    self.model.tradeCancel(tradeId)






