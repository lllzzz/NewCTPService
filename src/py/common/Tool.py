#!/usr/bin/env python
# -*- encoding:utf-8 -*-

import datetime
import time

class Tool():

    tidNum = 0

    @staticmethod
    def getTradeId(appFrom):
        now = datetime.datetime.now()
        timestamp = int(time.mktime(now.timetuple()))
        Tool.tidNum = Tool.tidNum + 1
        return appFrom + str(timestamp) + str(now.microsecond) + str(Tool.tidNum)


