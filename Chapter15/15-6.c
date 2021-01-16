#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "the number of arguments error.\n");
        exit(EXIT_FAILURE);
    }

    struct stat statbuff;
    mode_t file_mode;
    
    if(stat(argv[1], &statbuff) != 0)
    {
        fprintf(stderr, "stat error.\n");
        exit(EXIT_FAILURE);
    }

    file_mode = statbuff.st_mode | (S_IRUSR | S_IRGRP | S_IROTH);
    if(S_ISDIR(statbuff.st_mode) || (statbuff.st_mode & S_IXUSR == S_IXUSR) || (statbuff.st_mode & S_IXGRP == S_IXGRP) || (statbuff.st_mode & S_IXOTH == S_IXOTH))
    {
        file_mode |= (S_IXUSR | S_IXGRP | S_IXOTH);
    }

    if(chmod(argv[1], file_mode) != 0)
    {
        fprintf(stderr, "chmod failed.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("chmod a+rX successfully.\n");

    return 0;
}