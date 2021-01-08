#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char filename[64];
    char buffer[1] = "a";
    int numbytes;
    int fd;

    if((argc != 3) && (argc != 4))
    {
        fprintf(stderr, "parameters error.\n");
        exit(-1);
    }

    strncpy(filename, argv[1], sizeof(filename)-1);
    numbytes = atoll(argv[2]);

    if(argc == 3)
    {
        fd = open(filename, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if(fd < 0)
        {
            perror("open file failed.\n");
            exit(-1);
        }
        for(int i = 0; i < numbytes; i++)
        {
            write(fd, buffer, sizeof(buffer));
        }
    }
    if(argc == 4)
    {
        fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if(fd < 0)
        {
            perror("open file failed.\n");
            exit(-1);
        }
        for(int i = 0; i < numbytes; i++)
        {
            lseek(fd, 0, SEEK_END);
            write(fd, buffer, sizeof(buffer));
        }
    }

    close(fd);
    
    return 0;
}