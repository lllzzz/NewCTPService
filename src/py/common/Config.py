#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
#
import json
import os

class Config():
    """docstring for Config"""
    def __init__(self):
        pass

    @staticmethod
    def get():
        env_dist = os.environ
        path = env_dist.get("CTP_CONFIG_PATH")
        with open(path, 'r') as load_f:
            load_dict = json.load(load_f)
        return load_dict

