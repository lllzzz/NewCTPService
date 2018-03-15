#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Config import Config
from common.Tool import Tool
from common.MySQL import MySQL
import demjson as JSON
import random
import datetime

class TestDataService():
    """docstring for TradeService"""

    def __init__(self):
        self.over = False


    def isOver(self):
        return self.over


    def getData(self):
        ticks = [
            {
                'iid': 'hc1805',
                'price': 3600,
                'volume': 200,
            },
            {
                'iid': 'hc1805',
                'price': 3700,
                'volume': 300,
            },
        ]
        self.over = True
        return ticks

