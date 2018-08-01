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

class ControlService():

    def __init__(self, cmd, runConfig, globalId):
        self.cmd = cmd
        self.runConfig = runConfig
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
