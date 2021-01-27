#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

static void sig_handler(int sig)
{
    ;
}

int main()
{
    sigset_t block_mask, origin_mask, empty_mask;
    struct sigaction sig_act = {
        .sa_flags = SA_RESTART,
        .sa_handler = sig_handler,
    };
    pid_t child_pid; 
    int status;

    sigemptyset(&block_mask);
    if(sigaddset(&block_mask, SIGCHLD) != 0)
    {
        perror("sigaddset failed.\n");
        exit(EXIT_FAILURE);
    }

    if(sigprocmask(SIG_BLOCK, &block_mask, &origin_mask) != 0)
    {
        perror("sigprocmask failed.\n");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGCHLD, &sig_act, NULL) != 0)
    {
        perror("sigaction failed.\n");
        exit(EXIT_FAILURE);
    }

    switch(child_pid = fork())
    {
        case -1:
        {
            perror("fork failed.\n");
            break;
        }
        case 0:
        {
            exit(EXIT_SUCCESS);
        }
        default:
        {
            sleep(3);
            if(sigprocmask(SIG_SETMASK, &origin_mask, NULL) != 0)
            {
                perror("sigprocmask failed.\n");
                exit(EXIT_FAILURE);
            }
            printf("unblocked.\n");

            if(wait(&status) != -1)
            {
                printf("status is %d.\n", status);
            }
            break;
        }
    }

    return 0;
}