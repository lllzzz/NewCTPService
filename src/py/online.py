#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import sys
import argparse
from common.MySQL import MySQL
from common.Config import Config
from common.Redis import Redis

from service.TradeService import TradeService

parser = argparse.ArgumentParser()
parser.add_argument('model_name', help='model name')
parser.add_argument('version', help='running version')
parser.add_argument('iids', help='iids name, use "," to split')
args = parser.parse_args()

modelName   = args.model_name
version = args.version
iids        = args.iids.split(',')

srv = TradeService(modelName, version, iids)
srv.run()
