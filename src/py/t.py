from service.ControlService import CTPService, ModelService

s = ModelService('src/py/online.py demo aa', [], 'v0001')
# print s.cmd
# s.start()

# print s.status()

# s.stop()


s = CTPService('bin/marketService', [], 'SRV_MARKET')
# print s.cmd
# s.start()
# print s.status()
s.stop()


