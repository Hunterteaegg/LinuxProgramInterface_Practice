#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>

extern int optind, opterr, optopt;
extern char *optarg;

typedef enum
{
    DEFUALT,
    ADD,
    MINUS,
    EQU,
} operate_t;

int main(int argc, char *argv[])
{
    int opt, fd, attr;
    int inode_flag = 0;
    operate_t operate;

    while((opt = getopt(argc, argv, ":+-=aAcCdDeijPsStTu")) != -1)
    {
        switch(opt)
        {
            case '+':
            {
                operate = ADD;
                break;
            }
            case '-':
            {
                operate = MINUS;
                break;
            }
            case '=':
            {
                operate = EQU;
                break;
            }
            case 'a':
            {
                inode_flag |= FS_APPEND_FL;
                break;
            }
            case 'c':
            {
                inode_flag |= FS_COMPR_FL;
                break;
            }
            case 'D':
            {
                inode_flag |= FS_DIRSYNC_FL;
                break;
            }
            case 'i':
            {
                inode_flag |= FS_IMMUTABLE_FL;
                break;
            }
            case 'j':
            {
                inode_flag |= FS_JOURNAL_DATA_FL;
                break;
            }
            case 'A':
            {
                inode_flag |= FS_NOATIME_FL;
                break;
            }
            case 'd':
            {
                inode_flag |= FS_NODUMP_FL;
                break;
            }
            case 't':
            {
                inode_flag |= FS_NOTAIL_FL;
                break;
            }
            case 's':
            {
                inode_flag |= FS_SECRM_FL;
                break;
            }
            case 'S':
            {
                inode_flag |= FS_SYNC_FL;
                break;
            }
            case 'T':
            {
                inode_flag |= FS_TOPDIR_FL;
                break;
            }
            case 'u':
            {
                inode_flag |= FS_UNRM_FL;
                break;
            }
            default:
            {
                fprintf(stderr, "argument failed.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if(argc == 3)
    {
        fd = open(argv[2], O_RDWR);
        if(fd < 0)
        {
            perror("open file failed");
            exit(EXIT_FAILURE);
        }

        if(ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1)
        {
            fprintf(stderr, "get inode flags failed.\n");
            exit(EXIT_FAILURE);
        }

        switch(operate)
        {
            case ADD:
            {
                attr |= inode_flag;
                break;
            }
            case MINUS:
            {
                attr &= ~inode_flag;
                break;
            }
            case EQU:
            {
                attr = inode_flag;
                break;
            }
            default:
            {
                fprintf(stderr, "operate failed.\n");
                exit(EXIT_FAILURE);
            }
        }

        if(ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1)
        {
            fprintf(stderr, "set inode flags failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
