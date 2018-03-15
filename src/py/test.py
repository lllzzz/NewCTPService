#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import sys
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis
import datetime

from service.TestService import TestService
from service.TestDataService import TestDataService

appForm = sys.argv[1]
moduleName = sys.argv[2]
className = sys.argv[3]

dataSrv = TestDataService()

srv = TestService(appForm, moduleName, className, dataSrv, False)
srv.run()
