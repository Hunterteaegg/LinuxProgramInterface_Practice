#include <stdio.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int my_dup(int old_fd);
int my_dup2(int old_fd, int new_fd);

int main()
{
    int fd, dup_fd, dup2_fd;
    
    fd = open("./5-1.c", O_RDONLY);
    if(fd < 0)
    {
        perror("open file failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("open file successfully.\n");

    dup_fd = my_dup(fd);
    if(dup_fd < 0)
    {
        fprintf(stderr,"dup fd failed.\n");
    }
    printf("dup fd successfully : %d\n", dup_fd);

    dup2_fd = my_dup2(fd, 10);
    if(dup2_fd < 0)
    {
        fprintf(stderr, "dup2 fd failed.\n");
    }
    printf("dup2 fd successfully : %d\n", dup2_fd);

    close(fd);
    close(dup_fd);
    close(dup2_fd);

    return 0;
}

int my_dup(int old_fd)
{
    int new_fd;

    new_fd = fcntl(old_fd, F_DUPFD, 0);
    if(new_fd < 0)
    {
        perror("dup file descriptor failed.\n");
        exit(EXIT_FAILURE);
    }

    return new_fd;
}

int my_dup2(int old_fd, int new_fd)
{
    int fd;

    fd = fcntl(old_fd, F_DUPFD, new_fd);
    if(fd != new_fd)
    {
        errno = EBADF;
        exit(EXIT_FAILURE);
    }

    return fd;
}