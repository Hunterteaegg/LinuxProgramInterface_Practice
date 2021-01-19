#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int my_siginterrupt(int sig, int flag)
{
    struct sigaction act;
    int status;

    status = sigaction(sig, NULL, &act);
    if(status == -1)
    {
        return -1;
    }

    if(flag)
    {
        act.sa_flags &= ~SA_RESTART;
    }
    else
    {
        act.sa_flags |= SA_RESTART;
    }
    
    return sigaction(sig, &act, NULL);
}

int main()
{
    ;

    return 0;
}