#include <signal.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define CMD_SIZE    200
#define SYNC_SIG    SIGUSR1

static void sig_handler(int sig)
{
    ;
}

int main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;
    sigset_t block_mask, origin_mask, empty_mask;
    struct sigaction sig_act = {
        .sa_flags = SA_RESTART,
        .sa_handler = sig_handler,
    }; 

    setbuf(stdout, NULL);
    
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SYNC_SIG);
    if(sigprocmask(SIG_BLOCK, &block_mask, &origin_mask) != 0)
    {
        perror("sigpromask failed.\n");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SYNC_SIG, &sig_act, NULL) != 0)
    {
        perror("sigaction failed.\n");
        exit(EXIT_FAILURE);
    }

    switch(childPid = fork())
    {
        case -1:
        {
            fprintf(stderr, "fork failed.\n");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            if(kill(getppid(), SYNC_SIG) != 0)
            {
                fprintf(stderr, "send signal failed.\n");
                exit(EXIT_FAILURE);
            }
            printf("child process exit.\n");
            exit(EXIT_SUCCESS);
        }
        default:
        {
            sigemptyset(&empty_mask);
            if(sigsuspend(&empty_mask) == -1 && errno != EINTR)
            {
                fprintf(stderr, "sigsuspend failed.\n");
                exit(EXIT_FAILURE);
            }
            if(sigprocmask(SIG_SETMASK, &origin_mask, NULL) != 0)
            {
                fprintf(stderr, "sigpromask failed.\n");
                exit(EXIT_FAILURE);
            }
            
            snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
            cmd[CMD_SIZE] = '\0';
            system(cmd);

            if(kill(childPid, SIGKILL) == -1)
            {
                fprintf(stderr, "kill failed.\n");
            }
            sleep(3);
            printf("After sending SIGKILL to zombie (PID=%ld):\n", (long)childPid);
            system(cmd);

            exit(EXIT_SUCCESS);
        }
    }
}