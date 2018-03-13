#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import sys
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis

from service.TradeService import TradeService

appForm = sys.argv[1]
config = Config.get()
apps = config['app']

srv = TradeService(appForm)
srv.run()
