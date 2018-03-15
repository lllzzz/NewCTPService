import time
import sys

for x in xrange(1,3):
    for i in xrange(1, 100):
        print x
    sys.stdout.flush()
    time.sleep(1)