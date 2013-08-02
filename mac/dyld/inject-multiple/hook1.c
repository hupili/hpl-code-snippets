#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

typedef int (*fType)(const char*);
static int (*real_puts)(const char*) = NULL;

int puts(const char* str)
{
    if (!real_puts)
    {
        real_puts = (fType)dlsym(RTLD_NEXT, "puts");
    }
    real_puts("--------message from injected puts1()--------\n");
    real_puts(str);
}
