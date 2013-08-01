#!/bin/bash
gcc -flat_namespace -dynamiclib -o hook.dylib hook.c
gcc -dynamiclib -o mysharedlib.dylib mysharedlib.c
gcc mysharedlib.dylib main.c -o main
./main
DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=hook.dylib ./main
