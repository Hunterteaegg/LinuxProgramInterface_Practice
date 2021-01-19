#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void sigHandler(int sig)
{
    printf("Ouch!\n");
}

int main(int argc, char *argv[])
{
    int j;
    struct sigaction sigaction_str = {
        .__sigaction_handler = sigHandler,
    };

    if(sigaction(SIGINT, &sigaction_str,NULL) != 0)
    {
        fprintf(stderr, "set signal handler failed.\n");
        exit(EXIT_FAILURE);
    }

    for(j = 0; ; j++)
    {
        printf("%d\n", j);
        sleep(3);
    }
}