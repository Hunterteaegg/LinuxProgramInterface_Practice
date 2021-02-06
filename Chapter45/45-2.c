#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

key_t my_ftok(const char *pathname, int proj_id);

int main()
{
    printf("my_ftok: %X\n", my_ftok("./", 0x4A));

    return 0;
}

key_t my_ftok(const char *pathname, int proj_id)
{
    struct stat statbuf;
    uint32_t key;
    
    stat(pathname, &statbuf);
    key = (proj_id & 0xFF) << 24 | (statbuf.st_dev & 0xFF) << 16 | (statbuf.st_ino & 0xFFFF);

    return key;
}