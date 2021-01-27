#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t childPid;
    siginfo_t sig_info;

    if(argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "%s [exit-status]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    switch(fork())
    {
        case -1:
        {
            fprintf(stderr, "fork error.\n");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            printf("Child started with PID = %ld\n", (long)getpid);
            if(argc > 1)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                for(;;)
                {
                    pause();
                }
            }
            exit(EXIT_FAILURE);
        }
        default:
        {
            for(;;)
            {
                childPid = waitid(P_ALL, 0, &sig_info, WUNTRACED);
                if(childPid == -1)
                {
                    fprintf(stderr, "waitid failed.\n");
                    exit(EXIT_FAILURE);
                }
                printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
                (long)childPid, (unsigned int)status, status >> 8, status & 0xFF);

                printf("si_code: %d, si_errno: %d, si_signo: %d\n",sig_info.si_code, sig_info.si_errno, sig_info.si_signo);

                if(WEXITSTATUS(sig_info.si_status) || WIFSIGNALED(sig_info.si_status))
                {
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }
}