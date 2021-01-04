#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_READ    128

extern int optind, opterr, optopt;
extern char *optarg;

int fd;
char buffer[MAX_READ + 1];

void tee_cover(char *filename);
void tee_append(char *filename);

int main(int argc, char *argv[])
{
    int opt;
    char filename[64];

    while((opt = (getopt(argc, argv, ":a:"))) != -1)
    {
        printf("opt is %d(%c).\n", opt, opt);
        switch(opt)
        {
            case 'a':
            {
                printf("the filename is %s.\n", optarg);
                tee_append(optarg);
                exit(EXIT_SUCCESS);
            }
            default:
            {
                break;
            }
        }
    }
    strncpy(filename, argv[1], sizeof(filename));
    printf("the filename is %s.\n", filename);
    tee_cover(filename);
    

    return 0;
}

void tee_cover(char *filename)
{
    int num;
    fd = open(filename, O_RDWR | O_TRUNC | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IXUSR);
    if(fd < 0)
    {
        perror("open file failed.\n");
        exit(-1);
    }
    printf("open file successfully.\n");

    while((num = read(STDIN_FILENO, buffer, MAX_READ)) != 0)
    {
        printf("read %d letters.\n", num);
        write(fd, buffer, num);
    }
    fsync(fd);

    close(fd);
}

void tee_append(char *filename)
{
    int num;
    fd = open(filename, O_RDWR | O_APPEND);
    if(fd < 0)
    {
        perror("open file failed.\n");
        exit(-1);
    }
    printf("open file successfully.\n");

    while((num = read(STDIN_FILENO, buffer, MAX_READ)) != 0)
    {
        printf("read %d letters.\n", num);
        write(fd, buffer, num);
    }
    fsync(fd);

    close(fd);
}