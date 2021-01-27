#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define SYNC_SIG    SIGUSR1

static void sig_handler(int sig)
{
    ;
}

int main()
{
    pid_t child_pid;
    sigset_t block_mask, origin_mask, empty_mask;
    struct sigaction act = {
        .sa_flags = SA_RESTART,
        .sa_handler = sig_handler,
    };
    sigemptyset(&act.sa_mask);

    sigemptyset(&block_mask);
    if(sigaddset(&block_mask, SYNC_SIG) != 0)
    {
        perror("add block signal failed\n");
        exit(EXIT_FAILURE);
    }
    if(sigprocmask(SIG_BLOCK, &block_mask, &origin_mask) != 0)
    {
        perror("mask signal failed.\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SYNC_SIG, &act, NULL) != 0)
    {
        perror("sigaction failed.\n");
        exit(EXIT_FAILURE);
    }

    switch(child_pid = fork())
    {
        case -1:
        {
            fprintf(stderr, "fork failed.\n");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            sigemptyset(&empty_mask);
            if(sigsuspend(&empty_mask) == -1 && errno != EINTR)
            {
                perror("sigsuspend failed.\n");
                exit(EXIT_FAILURE);
            }
            if(sigprocmask(SIG_SETMASK, &origin_mask, NULL) != 0)
            {
                perror("recover mask failed.\n");
                exit(EXIT_FAILURE);
            }
            sleep(5);
            printf("child PID is %ld, PPID is %ld.\n", getpid(), getppid());
            exit(EXIT_SUCCESS);
        }
        default:
        {
            if(kill(child_pid, SYNC_SIG) != 0)
            {
                perror("send signal failed.\n");
                exit(EXIT_FAILURE);
            }
            printf("parent process exit.\n");
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}