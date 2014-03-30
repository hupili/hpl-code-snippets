
def _cat_generator(*args):
     for g in args:
         for el in g:
             yield el

a = (x for x in xrange(1, 5))
b = (x for x in xrange(10, 15))

c = _cat_generator([], a, ['hello'], b)
print list(c)
