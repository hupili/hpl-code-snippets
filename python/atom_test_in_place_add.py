# This demos that Python's `+=` operation is not atomic
# Usage: atom_test_in_place_add.py {N}
# {N} is the number of threads
#
# Some sample result
# $python atom_test_in_place_add.py 1
# sum: 100000
# $python atom_test_in_place_add.py 1
# sum: 100000
# $python atom_test_in_place_add.py 2
# sum: 186650
# $python atom_test_in_place_add.py 2
# sum: 180453
# $python atom_test_in_place_add.py 2
# sum: 175350

import sys
import threading

g_sum = 0

def in_place_add():
    global g_sum
    for i in range(100000):
        g_sum += 1

N = int(sys.argv[1])
t = []
for i in range(N):
    t.append(threading.Thread(target=in_place_add))
    t[i].start()

for i in range(len(t)):
    t[i].join()

print "sum:", g_sum


print
print "=========="
print "Here is a more direct demo"
print

def in_place_add_local():
    i += 1

print "--- following is the source code ---"
import inspect
print ''.join(inspect.getsourcelines(in_place_add_local)[0])

print "--- following is the byte code ---"
import dis
dis.dis(in_place_add_local)
