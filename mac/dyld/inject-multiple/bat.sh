#!/bin/bash
gcc -O0 -flat_namespace -dynamiclib -o hook1.dylib hook1.c
gcc -O0 -flat_namespace -dynamiclib -o hook2.dylib hook2.c
gcc -O0 -o main main.c
./main
DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=hook1.dylib:hook2.dylib ./main
