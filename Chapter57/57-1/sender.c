#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
    struct sockaddr_un sender_addr, receiver_addr;
    int senderFd, num;
    char sendBuffer[128];

    if(argc != 2)
    {
        fprintf(stderr, "sender argument number invalid\n");
        exit(EXIT_FAILURE);
    }

    memset(&sender_addr, 0, sizeof(struct sockaddr_un));
    sender_addr.sun_family = AF_UNIX;
    strncpy(&sender_addr.sun_path[1], "sender", sizeof(struct sockaddr_un) - 2);
    memset(&receiver_addr, 0, sizeof(struct sockaddr_un));
    receiver_addr.sun_family = AF_UNIX;
    strncpy(&receiver_addr.sun_path[1], "receiver", sizeof(struct sockaddr_un) - 2);

    senderFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(senderFd < 0)
    {
        perror("sender creates socket failed");
        exit(EXIT_FAILURE);
    }
    if(bind(senderFd, (struct sockaddr*)&sender_addr, sizeof(struct sockaddr_un)) != 0)
    {
        perror("sender bind socket failed");
        exit(EXIT_FAILURE);
    }
    
    num = atoi(argv[1]);
    for(int i = 0; i < num; i++)
    {
        snprintf(sendBuffer, sizeof(sendBuffer) - 1, "MESSAGE %d", i);
        if(sendto(senderFd, sendBuffer, sizeof(sendBuffer), 0, (struct sockaddr*)&receiver_addr, sizeof(struct sockaddr_un)) != sizeof(sendBuffer))
        {
            perror("send message failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}