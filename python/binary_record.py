import os
import struct

# part.ca: a text file, one line per integer

root = '.'
f = 'part.cq'
path = os.path.join(root, f)
ids = map(myint, open(path).readlines())

with open(f + '.bin', 'wb') as fp:
    for i in ids:
        fp.write(struct.pack('I', i))

with open(f + '.bin', 'rb') as fin:
    with open(f + 'txt', 'w') as fout:
        while True:
            r = fin.read(4)
            if not r:
                break
            i = struct.unpack('I', r)[0]
            fout.write(str(i) + '\n')

