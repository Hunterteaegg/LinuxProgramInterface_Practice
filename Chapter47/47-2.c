#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>

union semun 
{
    int val;
    unsigned short *array;
};

int main()
{
    pid_t childPid;
    int semId;
    struct sembuf sop;
    union semun arg; 

    setbuf(stdout, NULL);
    arg.val = 1;

    if((semId = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) == -1)
    {
        perror("parent semget failed");
        exit(EXIT_FAILURE);
    }
    if(semctl(semId, 0, SETVAL, arg) == -1)
    {
        perror("parent init sem failed");
        exit(EXIT_FAILURE);
    }

    switch(childPid = fork())
    {
        case -1:
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            sop.sem_num = 0;
            sop.sem_op = -1;
            sop.sem_flg = 0;
            printf("child about to handle sem\n");

            if(semop(semId, &sop, 1) == -1)
            {
                perror("child about to handle sem failed");
                exit(EXIT_FAILURE);
            }
            printf("child handle sem successfully\n");

            exit(EXIT_SUCCESS);
        }
        default:
        {
            sop.sem_num = 0;
            sop.sem_op = 0;
            sop.sem_flg = 0;
            printf("parent about to wait for unblocked\n");

            if(semop(semId, &sop, 1) == -1)
            {
                perror("parent wait for unblocked failed");
                exit(EXIT_FAILURE);
            }
            printf("parent is unblocked\n");
        }
    }

    if(semctl(semId, 0, IPC_RMID) == -1)
    {
        perror("parent remove sem failed");
        exit(EXIT_FAILURE);
    }

    printf("parent remove sem successfully\n");
    
    return 0;
}