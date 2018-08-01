#!/usr/bin/env python
# -*- encoding:utf-8 -*-
import datetime
import time
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis

class Data():

    def __init__(self):
        self.db = MySQL()
        self.rds = Redis.get()
        self.config = Config.get()

    def getTickHistory(self, instrument, iids, startTime, endTime):
        """ 获取历史TIck"""
        sql = "SELECT * FROM `tick_%s` WHERE iid in ('%s') AND `time` >= '%s' AND `time` <= '%s'" \
            % (instrument, "','".join(iids), startTime, endTime)

        return self.db.getAll(sql)






