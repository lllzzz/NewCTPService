#!/usr/bin/env python
# -*- encoding:utf-8 -*-
import MySQLdb
from Config import Config

class MySQL():


    """数据库管理类"""
    def __init__(self):

        config = Config.get()

        self.db = MySQLdb.connect(
            host = config['mysql']['host'],
            port = config['mysql']['port'],
            user = config['mysql']['user'],
            passwd = config['mysql']['passwd'],
            db = config['mysql']['name'],
            charset='utf8')
        self.cursor = self.db.cursor()


    def getAll(self, sql):
        self.cursor.execute(sql)
        res = self.cursor.fetchall()
        return len(res), res

    def getOne(self, sql):
        self.cursor.execute(sql)
        res = self.cursor.fetchone()
        flg = False if not res or len(res) == 0 else True
        return flg, res

    def insert(self, sql):
        self.cursor.execute(sql)
        self.db.commit()

    def update(self, sql):
        self.cursor.execute(sql)
        self.db.commit()

