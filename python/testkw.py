# Use params = {} style
# or
# Use **kwargs style to expect variable arguments?
#
# Considerations of cleanness, extensibility, etc:
#    * https://github.com/hupili/snsapi/issues/93
#
# This simple script test the execution.
# Conclusion: no big difference.

import time

class Timer(object):
    def __init__(self, name=None):
        self.name = name

    def __enter__(self):
        self.tstart = time.time()

    def __exit__(self, type, value, traceback):
        if self.name:
            print '[%s]' % self.name,
        print 'Elapsed: %s' % (time.time() - self.tstart)

def func_params(params):
    # simulate use of params
    params['count'] += 1
    return params['count']

def func_kwargs(**kwargs):
    # simulate use of params
    kwargs['count'] += 1
    return kwargs['count']

def test_params():
    for i in xrange(1,100000):
        params = {
                'count': 1,
                'path': 'xxx',
                'uid': 'yyyy',
                'hello': 'world'
                }
        func_params(params)

def test_kwargs():
    for i in xrange(1,100000):
        func_kwargs(count=1, path='xxx', uid='yyyy', hello='world')


with Timer('kwargs-style'):
    test_kwargs()

with Timer('params-style'):
    test_params()

with Timer('kwargs-style'):
    test_kwargs()

with Timer('params-style'):
    test_params()

with Timer('kwargs-style'):
    test_kwargs()

with Timer('params-style'):
    test_params()
