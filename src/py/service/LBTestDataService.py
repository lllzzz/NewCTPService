#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Config import Config
from common.Tool import Tool
from common.MySQL import MySQL
import demjson as JSON
import random
import datetime

class LBTestDataService():
    """docstring for TradeService"""

    def __init__(self, iids, startDate, endDate):
        self.iids = iids
        self.historyDate = startDate
        self.endDate = endDate
        self.db = MySQL()


    def isOver(self):
        return (self.endDate - self.historyDate).total_seconds() < 86400


    def getData(self):
        date = self.historyDate.strftime("%Y%m%d")
        tickGroup = []
        for iid in self.iids:
            data = self.getDataBatch(iid, date)
            tickGroup += list(data)
        # for one in tickGroup:
        #     print one['iid'], one['time'], one['msec']

        tickGroup = sorted(tickGroup, key=lambda x: (x['time'], x['msec']))
        # print '>>>>>>>>>>>>'
        # for one in tickGroup:
        #     print one['iid'], one['time'], one['msec']
        self.historyDate = self.historyDate + datetime.timedelta(days=1)
        return tickGroup


    def getDataBatch(self, iid, date):
        sql = '''
            SELECT * FROM `tick_%s` WHERE `time` LIKE '%s%%'  ORDER BY `time`, `msec`
        ''' % (iid, date)
        # print sql
        isOK, data = self.db.getAll(sql)
        return data
