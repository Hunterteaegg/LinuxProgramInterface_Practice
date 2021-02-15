#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main()
{
    struct sockaddr_un receiver_addr;
    int receiverFd, num;
    char receiverBuffer[128];

    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sun_family = AF_UNIX;
    strncpy(&receiver_addr.sun_path[1], "receiver", sizeof(struct sockaddr_un) - 2);

    receiverFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(receiverFd < 0)
    {
        perror("receiver creates socket failed");
        exit(EXIT_FAILURE);
    }
    if(bind(receiverFd, (struct sockaddr*)&receiver_addr, sizeof(struct sockaddr_un)) != 0)
    {
        perror("receiver bind socket failed");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        num = recvfrom(receiverFd, receiverBuffer, sizeof(receiverBuffer), 0, NULL, NULL);
        printf("receiver %d bytes: %s\n", num, receiverBuffer);
        sleep(3);
    }


    return 0;
}