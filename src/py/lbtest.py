#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import sys
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis
import datetime

from service.TestService import TestService
from service.LBTestDataService import LBTestDataService

appForm = sys.argv[1]
config = Config.get()
apps = config['app']

iids = apps[appForm]['iids']
moduleName = apps[appForm]['module']
className = apps[appForm]['class']
startDate = apps[appForm]['lbtest']['startDate']
endDate = apps[appForm]['lbtest']['endDate']

dataSrv = LBTestDataService(iids, datetime.datetime.strptime(startDate,'%Y-%m-%d'), datetime.datetime.strptime(endDate,'%Y-%m-%d'))

srv = TestService(appForm, moduleName, className, dataSrv, True, 5)
srv.run()
