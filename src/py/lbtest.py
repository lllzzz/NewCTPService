#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import argparse
import datetime

from web.models.Model import Model
from service.TestService import TestService
from service.LBTestDataService import LBTestDataService

parser = argparse.ArgumentParser()
parser.add_argument('model_name', help='model name')
parser.add_argument('test_version', help='test version')
parser.add_argument('iids', help='iids name, use "," to split')
parser.add_argument('start_date', help='history data start at this date')
parser.add_argument('end_date', help='history date stop at this date')
parser.add_argument('-r', '--is_random', action="store_true", help='use slippage point or not, default is False')
parser.add_argument('-s', '--max_slippage', help='max slippage', type=int, default=5)
args = parser.parse_args()

modelName   = args.model_name
testVersion = args.test_version
iids        = args.iids.split(',')
startDate   = args.start_date
endDate     = args.end_date
isRandom    = args.is_random
border      = args.max_slippage

# 获取模型
m = Model.query.filter(Model.nick_name==modelName).first()
if not m:
    print 'Error: model not exist'
    exit()

className = m.class_name
moduleName = 'model.' + className

dataSrv = LBTestDataService(iids,
    datetime.datetime.strptime(startDate,'%Y-%m-%d'),
    datetime.datetime.strptime(endDate,'%Y-%m-%d'))

srv = TestService(testVersion, modelName, moduleName, className, dataSrv, isRandom, border)
srv.run()
