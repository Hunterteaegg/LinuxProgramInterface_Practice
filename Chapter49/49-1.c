#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int sourceFd, targetFd;
    struct stat sourceStat;
    char *sourcePointer, *targetPointer;

    if(argc != 3)
    {
        fprintf(stderr, "argument number error\n");
        exit(EXIT_FAILURE);
    }

    sourceFd = open(argv[1], O_RDONLY);
    if(sourceFd == -1)
    {
        perror("open source file failed");
        exit(EXIT_FAILURE);
    }
    targetFd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(targetFd == -1)
    {
        perror("open target file failed");
        exit(EXIT_FAILURE);
    }

    if(fstat(sourceFd, &sourceStat) != 0)
    {
        perror("source file stat failed");
        exit(EXIT_FAILURE);
    }

    if(ftruncate(targetFd, sourceStat.st_size) != 0)
    {
        perror("truncate target file failed");
        exit(EXIT_FAILURE);
    }

    sourcePointer = mmap(NULL, sourceStat.st_size, PROT_READ, MAP_PRIVATE, sourceFd, 0);
    if(sourcePointer == NULL)
    {
        perror("source file mmap failed");
        exit(EXIT_FAILURE);
    }
    targetPointer = mmap(NULL, sourceStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, targetFd, 0);
    if(targetPointer == NULL)
    {
        perror("target file mmap failed");
        exit(EXIT_FAILURE);
    }

    memcpy(targetPointer, sourcePointer, sourceStat.st_size);

    msync(targetPointer, sourceStat.st_size, MS_SYNC);

    if(munmap(sourcePointer, sourceStat.st_size) != 0)
    {
        perror("unmap source file failed");
        exit(EXIT_FAILURE);
    }
    if(munmap(targetPointer, sourceStat.st_size) != 0)
    {
        perror("unmap target file failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}