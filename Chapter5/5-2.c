#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd;
    off_t offset;
    char buffer[128] = "what is this";
    fd = open("./test.txt", O_RDWR | O_APPEND);
    if(fd < 0)
    {
        perror("open file failed.\n");
        exit(-1);
    }

    offset = lseek(fd, 0, SEEK_SET);

    int num = write(fd, buffer, strlen(buffer));
    if(num < 0)
    {
        perror("write failed.\n");
    }
    exit(-1);

    close(fd);

    return 0;
}