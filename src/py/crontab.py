#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from web.models.Service import Service
from service.ControlService import CTPService, ModelService

services = Service.query.all()

for s in services:
    if s.type == 1:
        control = CTPService(s.cmd, s.run_config, s.global_id)
    else:
        control = ModelService(s.cmd, s.run_config, s.global_id)

    control.run()
