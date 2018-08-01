from service.ControlService import CTPService, ModelService

# s = ModelService('src/py/online.py demo aa', [], 'v0001')
# print s.cmd
# s.start()

# print s.status()

# s.stop()


# s = CTPService('bin/marketService', [], 'SRV_MARKET')
# print s.cmd
# s.start()
# print s.status()
# s.stop()

from common.Data import Data

d = Data()

cnt, data = d.getTickHistory('hc', ['hc1805'], '20180312 21:00:00', '20180312 21:01:00')
print cnt
print data[0]['price']


