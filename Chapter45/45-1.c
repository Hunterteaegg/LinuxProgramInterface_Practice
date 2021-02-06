#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>

int main()
{
    struct stat statbuf;

    stat("./", &statbuf);

    printf("device ID(last 8 bit): %0X\n", (long)statbuf.st_dev & 0xFF);
    printf("file inode(last 16 bit): %X\n", (long)statbuf.st_ino & 0x0FFFF);
    printf("proj(last 8 bit): %X\n", 0x4A & 0xFF);

    printf("ftok: %X\n", (long)ftok("./", 0x4A));

    return 0;
}