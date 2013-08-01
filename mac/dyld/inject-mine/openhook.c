#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "mysharedlib.h"

typedef void (*fType)();
static void (*real_f)() = NULL;

void f()
{
    if (!real_f)
    {
        void* handle = dlopen("mysharedlib.dylib", RTLD_NOW);
        real_f = (fType)dlsym(handle, "f");
        if (!real_f) printf("no real f() found!\n");
    }
    printf("--------message from injected f()--------\n");
    real_f();
}
