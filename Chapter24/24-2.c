#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define SYNC_SIG    SIGUSR1

static void handler(int sig)
{
    ;
}

int main()
{
    pid_t child_pid;
    sigset_t block_mask, original_mask, empty_mask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SYNC_SIG);
    if(sigprocmask(SIG_BLOCK, &block_mask, &original_mask) == -1)
    {
        fprintf(stderr, "sigprocmask failed.\n");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if(sigaction(SYNC_SIG, &sa, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed.\n");
        exit(EXIT_FAILURE);
    }


    switch(vfork())
    {
        case -1:
        {
            fprintf(stderr, "vfork() failed.\n");

            exit(EXIT_FAILURE);
        }

        case 0:
        {
            sleep(2);
            
            fprintf(stdout, "Child process is about to close STDOUT_FILENO.\n");
            if(close(STDOUT_FILENO) != 0)
            {
                fprintf(stderr, "close STDOUT_FILENO failed.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "close STDOUT_FILENO successfully.\n");
            if(kill(getppid(), SYNC_SIG) != 0)
            {
                fprintf(stderr, "kill failed.\n");
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "send signal successfully.\n");

            exit(EXIT_SUCCESS);
        }

        default:
        {
            fprintf(stderr, "waiting for signal from child.\n");
            sigemptyset(&empty_mask);
            if(sigsuspend(&empty_mask) == -1 && errno != EINTR)
            {
                fprintf(stderr, "sigsuspend failed.\n");
                exit(EXIT_FAILURE);
            }

            write(STDOUT_FILENO, "parent writes successfully.\n", sizeof("parent writes successfully.\n"));

            if(sigprocmask(SIG_SETMASK, &original_mask, NULL) == -1)
            {
                fprintf(stderr, "sigprocmask failed.\n");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}