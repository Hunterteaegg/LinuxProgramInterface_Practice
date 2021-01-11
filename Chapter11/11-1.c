#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void sysconfPrint(const char *msg, int name)
{
    long lim;

    errno = 0;
    lim = sysconf(name);
    if(lim != -1)
    {
        printf("%s %ld\n", msg, lim);
    }
    else
    {
        if(errno == 0)
        {
            printf("%s (indeterminate)\n");
        }
        else
        {
            fprintf(stderr, "sysconf %s\n", msg);
            exit(EXIT_FAILURE);
        }
        
    }
}

int main(int argc, char *argv[])
{
    sysconfPrint("_SC_ARG_MAX:          ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX:   ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX:         ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX:      ", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE:         ", _SC_PAGE_SIZE);
    sysconfPrint("_SC_RTSIG_MAX:        ", _SC_RTSIG_MAX);

    exit(EXIT_SUCCESS);
}