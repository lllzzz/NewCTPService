#!/usr/bin/env python
# -*- encoding:utf-8 -*-

import os
import sys
sys.path.append('../src/libs/')
from Config import Config as C
import demjson as JSON
from redis import Redis
import re
import time
from Logger import Logger

class Q():
    """消化队列"""
    def __init__(self, key, obj):
        self.key = key
        self.obj = obj
        env = C.get('env')
        host = C.get('rds_host_' + env)
        localDB = C.get('rds_db_local')
        self.rds = Redis(host = host, port = 6379, db = localDB)
        self.logger = Logger()

    def run(self):
        while True:
            data = self.rds.rpop(self.key)
            if data:
                try:
                    dataObj = JSON.decode(data)
                    self.obj.processQ(dataObj)
                except Exception, e:
                    self.logger.write('q', Logger.INFO, 'Q[exceptData]', {'data': data})
                    self.logger.write('q', Logger.INFO, 'Q[error]', {'error': e})
            else:
                time.sleep(1)
