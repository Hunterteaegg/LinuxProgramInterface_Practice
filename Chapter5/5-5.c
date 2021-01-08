#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/types.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <linux/uio.h>
#include <string.h>

int main()
{
    int fd, fd_copy, file_flag, ret;
    ssize_t offset;

    fd = open("./hello", O_RDONLY);
    if(fd < 0)
    {
        perror("open file failed.\n");
        exit(EXIT_FAILURE);
    }
    offset = lseek(fd, 10, SEEK_SET);
    file_flag = fcntl(fd, F_GETFL);
    printf("fd : %d, offset : %llu, flag : %00X\n", fd, offset, file_flag);

    fd_copy = dup(fd);
    if(fd_copy < 0)
    {
        perror("dup fd failed.\n");
        exit(EXIT_FAILURE);
    }
    offset = lseek(fd_copy, 0, SEEK_CUR);
    file_flag = fcntl(fd_copy, F_GETFL);
    printf("fd_copy : %d, offset : %llu, flag : %00X\n", fd_copy, offset, file_flag);
    
    ret = close(fd);
    if(ret < 0)
    {
        perror("close fd failed.\n");
        exit(EXIT_FAILURE);
    }
    ret = close(fd_copy);
    if(ret < 0)
    {
        perror("close fd failed.\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}