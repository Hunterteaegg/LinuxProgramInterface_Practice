#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int status;

    switch(fork()) 
    {
        case -1:
        {
            printf("fork failed.\n");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            printf("child process: PID %ld.\n", getpid());
            exit(-1);
        }
        default:
        {
            wait(&status);
            printf("child return: %ld.\n",WEXITSTATUS(status));
            exit(EXIT_SUCCESS);
        }
    }

    return 0;   
}