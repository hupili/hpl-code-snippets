#!/bin/bash
gcc -O0 -flat_namespace -dynamiclib -o openhook.dylib openhook.c
gcc -O0 -o main main.c
./main
DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=openhook.dylib ./main
