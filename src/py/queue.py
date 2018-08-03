#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis
import time as timeer
import hashlib
import demjson as JSON
import datetime

def format_price(num):
    num = float('{:.2f}'.format(num))
    return 0 if num > 10000000.0 else num


db = MySQL()
rds = Redis.get()
config = Config.get()

iids = config['iids']
runningKey = 'CTP.QUEUE.RUNNING.CNT'
rds.incr(runningKey)

for i in xrange(1,10000):

    runningCnt = int(rds.get(runningKey))
    if runningCnt > 5:
        break

    data = rds.rpop(config['queueKey'])
    if not data:
        timeer.sleep(1)
        continue

    data = JSON.decode(data)
    # print data
    action = data['action']

    if action == 'TICK':
        iid = data['iid']
        time = data['time']
        msec = data['msec']
        price = format_price(data['price'])
        volume = data['volume']
        bidPrice1 = format_price(data['bidPrice1'])
        bidVolume1 = data['bidVolume1']
        askPrice1 = format_price(data['askPrice1'])
        askVolume1 = data['askVolume1']
        m2 = hashlib.md5()
        m2.update(iid + time + str(msec))
        token = m2.hexdigest()

        sql = '''
            INSERT INTO `tick_%s` (`token`, `iid`, `time`, `msec`, `price`, `volume`, `bid_price1`, `bid_volume1`, `ask_price1`, `ask_volume1`)
            VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')
        ''' % (filter(lambda x:x not in '0123456789', iid), token, iid, time, msec, price, volume, bidPrice1, bidVolume1, askPrice1, askVolume1)
        # print sql
        try:
            db.insert(sql)
        except Exception as e:
            pass


    if action in ['TRADE', 'FOK', 'FAK', 'IOC']:
        iid = data['iid']
        app_from = data['from']
        tradeId = data['id']
        ctpTdReqId = data['tdReqId']
        isOpen = data['isOpen']
        isBuy = data['isBuy']
        total = data['total']
        price = format_price(data['price'])
        isToday = data['isToday']
        sendTime = datetime.datetime.now().strftime("%Y%m%d %H:%M:%S")

        sql = '''
            INSERT INTO `trade` (`iid`, `from`, `trade_id`, `ctp_td_req_id`, `is_open`, `is_buy`, `volume`, `price`, `is_today`, `action`, `send_time`)
            VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')
        ''' % (iid, app_from, tradeId, ctpTdReqId, int(isOpen), int(isBuy), total, price, int(isToday), action, sendTime)
        # print sql

        db.insert(sql)


    if action == 'CANCEL':
        tradeId = data['tradeId']
        sql = '''UPDATE `trade` SET `status` = `status` + 5 WHERE `trade_id` = '%s'; ''' % (tradeId)
        db.update(sql)


    if action == 'TRADED':
        print data
        tradeId = data['id']
        ctpFrontId = data['frontId']
        ctpSessionId = data['sessionId']
        ctpExchangeId = data['exchangeId']
        ctpOrderSysId = data['orderSysId'].strip()
        ctpTradeId = data['tradeId'].strip()
        dealPrice = format_price(data['dealPrice'])
        dealVolume = data['dealVolume']
        dealTime = data['tradeDate'] + ' ' + data['tradeTime']

        sql = '''
            UPDATE `trade` SET
            `ctp_front_id` = '%s',
            `ctp_session_id` = '%s',
            `ctp_exchange_id` = '%s',
            `ctp_sys_order_id` = '%s',
            `ctp_trade_id` = '%s',
            `deal_price` = '%s',
            `deal_volume` = '%s',
            `deal_time` = '%s',
            `status` = `status` + 1
            WHERE `trade_id` = '%s'
        ''' % (ctpFrontId, ctpSessionId, ctpExchangeId, \
            ctpOrderSysId, ctpTradeId, dealPrice, dealVolume, \
            dealTime, tradeId)
        # print sql
        db.update(sql)

    if action == 'CANCELED':
        tradeId = data['id']
        sql = '''UPDATE `trade` SET `status` = `status` + 2 WHERE `trade_id` = '%s'; ''' % (tradeId)
        db.update(sql)



rds.decr(runningKey)







