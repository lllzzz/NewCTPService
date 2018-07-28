#!/usr/bin/env python
# -*- encoding:utf-8 -*-

from .Redis import Redis

class Locker():

    prefix = 'CTP_LOCKER_'

    """Locker"""
    def __init__(self, key):
        key = self.prefix + key
        self.rds = Redis.get()
        self.counter = self.rds.incr(key)
        self.key = key


    @staticmethod
    def getLocking(key):
        key = Locker.prefix + key
        rds = Redis.get()
        keys = rds.keys(key + '*')
        res = []
        for one in keys:
            res.append(one.replace(key, ''))
        return res



    def isLocking(self):
        if self.counter > 1: return True
        return False


    def __del__(self):
        count = self.rds.decr(self.key)
        if count <= 0:
            self.rds.delete(self.key)


