#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from common.Config import Config
from common.Tool import Tool
from common.MySQL import MySQL
import demjson as JSON
import random
import datetime
import os
import csv

class TestDataService():
    """docstring for TradeService"""

    def __init__(self, className):
        self.over = False
        self.className = className


    def isOver(self):
        return self.over


    def getData(self):
        dataPathDir = 'src/py/web/static/test_data/'
        dataPath = dataPathDir + self.className + '.csv'
        if not os.path.exists(dataPath):
            dataPath = dataPathDir + 'test.csv'

        data = csv.reader(open(dataPath, 'r'))
        isTitle = True
        title = []
        tickData = []
        for line in data:
            if isTitle:
                for t in line:
                    title.append(t)
                isTitle = False
                continue
            tick = {}
            for i, t in enumerate(line):
                tick[title[i]] = t

            tickData.append(tick)

        self.over = True
        return tickData

