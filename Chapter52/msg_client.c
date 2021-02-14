#include "msg_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define CLIENT_PATH "/client"

int main(int argc, char *argv[])
{
    int seqLen; 
    mqd_t clientId, serverId;
    struct mq_attr clientAttr = {
        .mq_maxmsg = MAX_MSG,
        .mq_msgsize = sizeof(struct response_msg),
    };
    struct request_msg req;
    struct response_msg res;

    if(argc != 2)
    {
        fprintf(stderr, "argument error\n");
        exit(EXIT_FAILURE);
    }

    seqLen = atoi(argv[1]);

    clientId = mq_open(CLIENT_PATH, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &clientAttr);
    if(clientId < 0)
    {
        perror("client open message queue failed");
        exit(EXIT_FAILURE);
    }
    serverId = mq_open(SERVER_PATH, O_RDWR);
    if(serverId < 0)
    {
        perror("client open server queue failed");
        exit(EXIT_FAILURE);
    }

    req.clientPath = CLIENT_PATH; 
    req.seqLen = seqLen;

    if(mq_send(serverId, (char*)&req, sizeof(struct request_msg), 0) == -1)
    {
        perror("client send message failed");
        exit(EXIT_FAILURE);
    }
    if(mq_receive(clientId, (char*)&res, sizeof(struct response_msg), 0) == -1)
    {
        perror("client receive message failed");
        exit(EXIT_FAILURE);
    }
    printf("The allocated number is %d\n", res.seqNum);

    return 0;
}