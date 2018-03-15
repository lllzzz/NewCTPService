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
        if len(self.iids) == 1:
            iid = self.iids[0]
            data = self.getDataBatch(filter(lambda x:x not in '0123456789', iid), date)
        # for iid in self.iids:
        self.historyDate = self.historyDate + datetime.timedelta(days=1)
        return data


    def getDataBatch(self, iid, date):
        sql = '''
            SELECT * FROM `tick_%s` WHERE `time` LIKE '%s%%'  ORDER BY `time`, `msec`
        ''' % (iid, date)
        print sql
        isOK, data = self.db.getAll(sql)
        return data
