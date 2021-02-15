#include "us_xfr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        perror("client creates socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1], SV_SOCK_PATH, sizeof(addr.sun_path));

    if(connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("client connects socket failed");
        exit(EXIT_FAILURE);
    }

    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        if(write(sfd, buf, numRead) != numRead)
        {
            exit(EXIT_FAILURE);
        }
    }


    return 0;
}