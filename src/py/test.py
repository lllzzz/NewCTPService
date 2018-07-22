#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import argparse

from web.models.Model import Model
from service.TestService import TestService
from service.TestDataService import TestDataService

parser = argparse.ArgumentParser()
parser.add_argument('model_name', help='model name')
parser.add_argument('-p', '--data_path', help='test data path')
args = parser.parse_args()

modelName = args.model_name
dataPath = args.data_path or ''

# 获取模型
m = Model.query.filter(Model.nick_name==modelName).first()
if not m:
    print 'Error: model not exist'
    exit()

className = m.class_name
moduleName = 'model.' + className

dataSrv = TestDataService(modelName, dataPath)

srv = TestService(modelName, modelName, moduleName, className, dataSrv)
srv.run()
