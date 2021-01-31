#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int status;

    printf("PGID = %ld.\n", getpgrp());
    printf("set PGID.\n");
    setpgrp();
    printf("PGID = %ld.\n", getpgrp());
    printf("set = %ld.\n", getsid(0));
    printf("set SID.\n");
    status = setsid();
    printf("SID = %ld.\n", getsid(0));
    if(status != 0)
    {
        perror("set SID failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}