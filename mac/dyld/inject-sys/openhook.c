#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

typedef int (*fType)(const char*);
static int (*real_puts)(const char*) = NULL;

int puts(const char* str)
{
    if (!real_puts)
    {
        //void* handle = dlopen("mysharedlib.dylib", RTLD_NOW);
        //real_puts = (fType)dlsym(handle, "f");
        real_puts = (fType)dlsym(RTLD_NEXT, "puts");
        //if (!real_puts) printf("no real puts() found!\n");
    }
    real_puts("--------message from injected puts()--------\n");
    real_puts(str);
}
