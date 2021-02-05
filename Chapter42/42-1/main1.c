#include <stdio.h>
#include <dlfcn.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void print(void);

int main()
{
    void* lib_handler = NULL;

    lib_handler = dlopen("./libdemo.so", RTLD_LAZY);
    if(NULL == lib_handler)
    {
        perror("lib_handler NULL");
        exit(EXIT_FAILURE);
    }

    void(*func)(void) = dlsym(lib_handler, "print");
    func();

    dlclose(lib_handler);

    return 0;
}