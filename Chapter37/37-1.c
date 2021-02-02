#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    unsigned int level;

    if(argc != 3)
    {
        fprintf(stderr, "argument failed: [LEVEL] [MESSAGE].\n");
        exit(EXIT_FAILURE);
    }
    switch(atoi(argv[1]))
    {
        case 0:
        {
            level = LOG_DEBUG;
            break;
        }
        case 1:
        {
            level = LOG_INFO;
            break;
        }
        case 2:
        {
            level = LOG_NOTICE;
            break;
        }
        case 3:
        {
            level = LOG_WARNING;
            break;
        }
        case 4:
        {
            level = LOG_ERR;
            break;
        }
        case 5:
        {
            level = LOG_CRIT;
            break;
        }
        case 6:
        {
            level = LOG_ALERT;
            break;
        }
        case 7:
        {
            level = LOG_EMERG;
            break;
        }
        default:
        {
            fprintf(stderr, "LEVEL argument failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    openlog(NULL, LOG_CONS | LOG_PERROR | LOG_PID, LOG_USER);
    syslog(level, argv[2]);
    closelog();

    return 0;
}