#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
#
import logging
import datetime
from common.Config import Config
import demjson as JSON

logging.basicConfig(format="%(asctime)s - %(levelname)s - %(message)s", level=logging.INFO)

class Logger():

    def __init__(self, name, isToFile = False):

        self.logger = logging.getLogger(name)
        self.isToFile = isToFile
        if self.isToFile:
            config = Config.get()
            today = datetime.datetime.now().strftime("%Y%m%d")
            path = "%s/src/py/web/static/logs/online/%s_%s.log" % (config['appRoot'], name, today)
            print path, name
            self.logger.addHandler(logging.FileHandler(path))


    def info(self, *args):
        if self.isToFile:
            self.logger.info(JSON.encode(args))
        else:
            print args
