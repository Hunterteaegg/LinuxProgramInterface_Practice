#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static void sig_handler(int sig)
{
    printf("Caught signal.\n");
}

int main()
{
    pid_t child_pid;
    int status;
    struct sigaction sig_act = {
        .sa_handler = sig_handler,
        .sa_flags = SA_RESTART,
    };

    status = sigaction(SIGUSR1, &sig_act, NULL);
    if(status != 0)
    {
        perror("sigaction failed.\n");
        exit(EXIT_FAILURE);
    }

    switch(child_pid = fork())
    {
        case -1:
        {
            perror("fork failed.\n");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            printf("child PGID = %ld.\n", getpgrp());
            pause();
            printf("child PGID = %ld.\n", getpgrp());
            pause();
            execve("./helloworld.o", NULL, NULL);
        }
        default:
        {
            sleep(3);
            status = setpgid(child_pid, 0);
            if(status != 0)
            {
                perror("setpgid 1 failed.\n");
                exit(EXIT_FAILURE);
            }
            printf("change child PGID.\n");
            status = kill(child_pid, SIGUSR1);
            if(status != 0)
            {
                printf("send signal failed.\n");
            }
            sleep(3);
            status = kill(child_pid, SIGUSR1);
            if(status != 0)
            {
                printf("send signal failed.\n");
            }
            
        }
    }
}