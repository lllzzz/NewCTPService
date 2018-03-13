#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import sys
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis
import datetime

from service.LookingBackTestService import LookingBackTestService

appForm = sys.argv[1]
config = Config.get()
apps = config['app']

srv = LookingBackTestService(appForm, 
    datetime.datetime.strptime('2018-03-12','%Y-%m-%d'), 
    datetime.datetime.strptime('2018-03-13','%Y-%m-%d'), 
    True, 5)
srv.run()
