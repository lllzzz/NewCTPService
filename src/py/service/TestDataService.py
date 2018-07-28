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

    def __init__(self, modelName, dataPath = ''):
        self.over = False
        self.modelName = modelName
        self.dataPath = dataPath


    def isOver(self):
        return self.over


    def getData(self):

        if not os.path.exists(self.dataPath):
            config = Config.get()
            self.dataPath = config['appRoot'] + '/src/py/web/static/test_data/test.csv'

        data = csv.reader(open(self.dataPath, 'r'))
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

