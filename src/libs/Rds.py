#!/usr/bin/env python
# -*- encoding:utf-8 -*-

from redis import Redis
from Config import Config as C

class Rds():
    """Redis封装"""
    def __init__(self, host, port, db):
        self.rds = Redis(host = host, port = port, db = db)

    @staticmethod
    def getLocal():
        env = C.get('env')
        host = C.get('rds_host_' + env)
        localDB = C.get('rds_db_local')
        obj = Rds(host, 6379, localDB)
        return obj.rds

    @staticmethod
    def getRds():
        env = C.get('env')
        host = C.get('rds_host_' + env)
        db = C.get('rds_db_' + env)
        obj = Rds(host, 6379, db)
        return obj.rds

    @staticmethod
    def getSender():
        return Rds.getRds()

    @staticmethod
    def getService():
        rds = Rds.getRds()
        return rds.pubsub()
