#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

#define BUFF_SIZE   128
int main()
{
    int parent2child_pipe[2], child2parent_pipe[2];
    int readnum;
    pid_t childpid;
    char buffer[BUFF_SIZE + 1];

    if(pipe(parent2child_pipe) != 0)
    {
        perror("create pipe failed");
        exit(EXIT_FAILURE);
    }
    if(pipe(child2parent_pipe) != 0)
    {
        perror("create pipe failed");
        exit(EXIT_FAILURE);
    }

    switch(childpid = fork())
    {
        case -1:
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        case 0:
        {
            if(close(parent2child_pipe[1]) != 0)    //child closed write end
            {
                perror("close p2c write end failed");
                exit(EXIT_FAILURE);
            }
            if(close(child2parent_pipe[0]) != 0)    //child closed read end
            {
                perror("close c2p read end failed");
                exit(EXIT_FAILURE);
            }
            while((readnum = read(parent2child_pipe[0], buffer, BUFF_SIZE)) != 0) 
            {
                for(int i = 0; i < readnum; i++)
                {
                    toupper((unsigned char)buffer[i]);
                }
                write(child2parent_pipe[1], buffer, BUFF_SIZE);
            }
            if(close(parent2child_pipe[0]) != 0)
            {
                perror("close p2c read end failed");
                exit(EXIT_FAILURE);
            }
            if(close(child2parent_pipe[1] != 0))
            {
                perror("close c2p write end failed");
                exit(EXIT_FAILURE);
            }
        }
        default:
        {
            if(close(parent2child_pipe[0]) != 0)    //parent closed read end
            {
                perror("close p2c read end failed");
                exit(EXIT_FAILURE);
            }
            if(close(child2parent_pipe[1]) != 0)
            {
                perror("close c2p write end failed");
                exit(EXIT_FAILURE);
            }
            while((readnum = read(STDIN_FILENO, buffer, BUFF_SIZE) != 0))
            {
                write(parent2child_pipe[1], buffer, BUFF_SIZE);
                read(child2parent_pipe[0], buffer, BUFF_SIZE);
                buffer[BUFF_SIZE] = '\0';
                printf("%s", buffer);
            }
            if(close(parent2child_pipe[1]) != 0)
            {
                perror("close p2c write end failed");
                exit(EXIT_FAILURE);
            }
            if(close(child2parent_pipe[0]) != 0)
            {
                perror("close c2p read end failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}