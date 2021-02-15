#include "us_xfr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        perror("server creates socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0 ,sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1], SV_SOCK_PATH, sizeof(addr.sun_path) - 2);

    if(bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("server binds socket failed");
        exit(EXIT_FAILURE);
    }

    if(listen(sfd, BACKLOG) == -1)
    {
        perror("server listen socket failed");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        cfd = accept(sfd, NULL, NULL);
        if(cfd == -1)
        {
            perror("server accepts socket failed");
            exit(EXIT_FAILURE);
        }
        while((numRead = read(cfd, buf, BUF_SIZE)) > 0)
        {
            if(write(STDOUT_FILENO, buf, numRead) != numRead)
            {
                perror("server writes socket failed");
                exit(EXIT_FAILURE);
            }
        }

        if(numRead == -1)
        {
            perror("server reads socket failed");
            exit(EXIT_FAILURE);
        }
        if(close(cfd) == -1)
        {
            perror("server closes socket failed");
            exit(EXIT_FAILURE);
        }
    }
}