#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/types.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <linux/uio.h>
#include <string.h>

#define BUFFER_1_SIZE   10
#define BUFFER_2_SIZE   11

ssize_t my_readv(int fd, const struct iovec *iov, int iovint);
ssize_t my_writev(int fd, const struct iovec *iov, int iovint);

int main()
{
    char *buffer1, *buffer2;
    int fd;
    ssize_t data_size;

    fd = open("./hello", O_RDWR);
    if(fd < 0)
    {
        perror("open file failed.\n");
        exit(EXIT_FAILURE);
    }

    buffer1 = (char*)malloc((BUFFER_1_SIZE + 1) * sizeof(char));
    if(NULL == buffer1)
    {
        perror("buffer1 allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    buffer2 = (char*)malloc((BUFFER_2_SIZE + 1) * sizeof(char));
    if(NULL == buffer2)
    {
        perror("buffer2 allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    memset((char*)buffer1, 0, BUFFER_1_SIZE + 1);
    memset((char*)buffer2, 0, BUFFER_2_SIZE + 1);

    struct iovec iov[2] = {
        {
            .iov_base = buffer1,
            .iov_len = BUFFER_1_SIZE,
        },
        {
            .iov_base = buffer2,
            .iov_len = BUFFER_2_SIZE,
        },
    };

    data_size = my_readv(fd, iov, sizeof(iov) / sizeof(struct iovec));
    printf("The read data size is %llu.\n", (unsigned long long)data_size);
    printf("buffer1 is \"%s\"\n", buffer1);
    printf("buffer2 is \"%s\"\n", buffer2);

    lseek(fd, 0, SEEK_END);
    
    memset((char*)buffer1, 0, BUFFER_1_SIZE + 1);
    memset((char*)buffer2, 0, BUFFER_2_SIZE + 1);

    snprintf(buffer1, BUFFER_1_SIZE, "SCNU ");
    snprintf(buffer2, BUFFER_2_SIZE, "SCUT ");

    data_size = my_writev(fd, iov, sizeof(iov) / sizeof(struct iovec));
    printf("The write data size is %llu.\n", (unsigned long long)data_size);

    free(buffer1);
    buffer1 = NULL;
    free(buffer2);
    buffer2 = NULL;

    int ret = close(fd);
    if(ret < 0)
    {
        perror("close file failed.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

ssize_t my_readv(int fd, const struct iovec *iov, int iovint)
{
    ssize_t byte_num = 0;

    for(int i = 0; i < iovint; i++)
    {
        byte_num += read(fd, iov[i].iov_base, iov[i].iov_len);
    }

    return byte_num;
}

ssize_t my_writev(int fd, const struct iovec *iov, int iovint)
{
    ssize_t byte_num = 0;

    for(int i = 0; i < iovint; i++)
    {
        byte_num += write(fd ,iov[i].iov_base, iov[i].iov_len);
    }

    return byte_num;
}