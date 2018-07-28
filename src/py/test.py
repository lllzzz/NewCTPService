#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
import argparse

from service.TestService import TestService
from service.TestDataService import TestDataService

parser = argparse.ArgumentParser()
parser.add_argument('model_name', help='model name')
parser.add_argument('-p', '--data_path', help='test data path')
args = parser.parse_args()

modelName = args.model_name
dataPath = args.data_path or ''

dataSrv = TestDataService(modelName, dataPath)

srv = TestService(modelName, modelName, dataSrv)
srv.run()
