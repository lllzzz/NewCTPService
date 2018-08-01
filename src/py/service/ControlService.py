#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
# from common.Locker import Locker
from common.Tool import Tool
from common.Config import Config
from common.Redis import Redis
import datetime
import os
import subprocess
import time
import demjson as JSON

class ControlService():

    def __init__(self, cmd, runConfig, globalId):
        self.cmd = cmd
        self.runConfig = JSON.decode(runConfig)
        self.globalId = globalId
        self.sender = Redis.get()
        self.build()

    def build(self):
        raise Exception("rewrite build cmd")

    def start(self):
        fd = open('/tmp/cmd.log', 'w')
        subprocess.Popen(self.cmd, shell=True, stdout=fd)


    def stop(self):
        raise Exception("rewrite stop")

    def status(self):
        raise Exception("rewrite status")

    def restart(self):
        if self.status():
            self.stop()
            while self.status():
                time.sleep(1)
            self.start()

        else:
            self.start()


    def run(self):
        now = datetime.datetime.now()
        dateStr = now.strftime("%Y%m%d")

        isRunTime = False
        for i in xrange(0, len(self.runConfig), 2):
            startTime = datetime.datetime.strptime("%s %s" % (dateStr, self.runConfig[i]),'%Y%m%d %H:%M')
            endTime = datetime.datetime.strptime("%s %s" % (dateStr, self.runConfig[i+1]),'%Y%m%d %H:%M')
            if endTime < startTime:
                endTime = endTime + datetime.timedelta(days=1)
            if startTime <= now <= endTime:
                isRunTime = True

        if isRunTime:
            if not self.status():
                self.start()
        else:
            if self.status():
                self.stop()


class CTPService(ControlService):

    def build(self):
        config = Config.get()
        appRoot = config['appRoot']
        ctpConfig = config['ctpConfig']

        self.cmd = "%s/%s %s/%s" % (appRoot, self.cmd, appRoot, ctpConfig)

    def stop(self):
        self.sender.publish(self.globalId, "2")

    def status(self):
        if self.sender.publish(self.globalId, "1"):
            return True
        return False


class ModelService(ControlService):

    def build(self):
        config = Config.get()
        appRoot = config['appRoot']

        self.cmd = "CTP_CONFIG_PATH=%s python %s/%s %s" % (os.environ.get('CTP_CONFIG_PATH'), appRoot, self.cmd, self.globalId)

    def stop(self):
        self.sender.publish(self.globalId, "STOP")


    def status(self):
        if self.sender.publish(self.globalId, "STATUS"):
            return True
        return False
