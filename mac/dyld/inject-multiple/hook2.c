#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

char* gets(char* str)
{
    puts("You called gets() but we do not get s for you");
}
