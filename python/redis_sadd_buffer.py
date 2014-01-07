# Demo the difference between single operation and batch operation.
# This is sometimes more than performance issue,
# e.g. a matter of monetary cost!

# We use set insertion of Redis for this demo

# The POC here may result in a project called PyPiPe (PPP)

# A sample result:
# $python t.py
# elapsed 8.38782501221
# check OK
# elapsed 0.515118837357
# check OK

import redis
import time

r = redis.Redis()

class Timer(object):
    def __enter__(self):
        self.start_time = time.time()
    def __exit__(self, type, value, traceback):
        elapse = time.time() - self.start_time
        print "elapsed", elapse

TOTAL = 100000
SEGMENT = 1000

def func1():
    for i in xrange(TOTAL):
        r.sadd('test', i)

def func2():
    for i in xrange(TOTAL / SEGMENT):
        l = []
        for j in xrange(SEGMENT):
            l.append(i * SEGMENT + j)
        r.sadd('test', *l)

def check():
    ground_truth = set(map(str, range(TOTAL)))
    from_redis = r.smembers('test')
    if ground_truth == from_redis:
        print "check OK"
    else:
        print "check fail"

r.delete('test')
with Timer():
    func1()
check()

r.delete('test')
with Timer():
    func2()
check()

