#!/usr/bin/env python
# -*- encoding:utf-8 -*-

from redis import Redis as Rds
from Config import Config

class Redis():
    """Redis封装"""
    def __init__(self):
        pass

    @staticmethod
    def get():
        config = Config.get()
        rds = Rds(host = config['redis']['host'],
            port = config['redis']['port'],
            db = config['redis']['db'])
        return rds


