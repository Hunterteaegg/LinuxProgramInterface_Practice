#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    void *libHandle;
    void (*funcp)(void);
    const char *err;
    Dl_info info;

    if(argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "%s lib-path func-name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    libHandle = dlopen(argv[1], RTLD_LAZY);
    if(libHandle == NULL)
    {
        fprintf(stderr, "dlopen: %s.\n", dlerror());
        exit(EXIT_FAILURE);
    }

    (void)dlerror();
    funcp = dlsym(libHandle, argv[2]);
    err = dlerror();
    if(err != NULL)
    {
        fprintf(stderr, "dlsym: %s.\n", err);
        exit(EXIT_FAILURE);
    }

    if(funcp == NULL)
    {
        printf("%s is NULL\n", argv[2]);
    }
    else 
    {
        funcp();
    }

    dladdr(funcp, &info);
    printf("dli_fname: %s\n", info.dli_fname);
    printf("dli_fbase: %p\n", info.dli_fbase);
    printf("dli_sname: %s\n", info.dli_sname);
    printf("dli_saddr: %p\n", info.dli_saddr);

    dlclose(libHandle);

    return 0;
}