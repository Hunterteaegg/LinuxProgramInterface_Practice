#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_READ   128
int main(int argc, char *argv[])
{
    char file_destination[64];
    char file_source[64];
    char buffer[MAX_READ];
    int num;

    int fd_file_destination, fd_file_sources;

    if(argc != 3)
    {
        fprintf(stderr, "argument number error!\n");
        exit(-1);
    }

    strncpy(file_destination, argv[1], sizeof(file_destination));
    strncpy(file_source, argv[2], sizeof(file_source));

    fd_file_destination = open(file_destination, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd_file_destination < 0)
    {
        perror("open file destination failed.\n");
        exit(-1);
    }

    fd_file_sources = open(file_source, O_RDONLY);
    if(fd_file_sources < 0)
    {
        perror("open file source failed.\n");
        exit(-1);
    }

    while((num = read(fd_file_sources, buffer, MAX_READ)) != 0)
    {
        num = write(fd_file_destination, buffer, num);
        printf("write %d letters.\n", num);
    }

    close(fd_file_destination);
    close(fd_file_sources);

    return 0;
}