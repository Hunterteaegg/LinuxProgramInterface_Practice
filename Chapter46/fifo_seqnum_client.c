#include "fifo_msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int clientId, serverId;
    int seqLen, msgLen;
    struct request_msg req;
    struct response_msg res;

    if(argc != 2)
    {
        fprintf(stderr, "need one argument as seqLen.\n");
        exit(EXIT_FAILURE);
    }
    seqLen = atoi(argv[1]);

    clientId = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP);
    if(clientId == -1)
    {
        perror("client msgget failed");
        exit(EXIT_FAILURE);
    }
    serverId = msgget(SERVER_KEY, S_IRUSR | S_IWUSR | S_IRGRP);
    if(serverId == -1)
    {
        perror("get serverId failed");
        exit(EXIT_FAILURE);
    }

    req.mtype = REQ_TYPE;
    req.clientId = clientId;
    req.seqLen = seqLen; 

    if(msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
    {
        perror("client msgsnd failed");
        exit(EXIT_FAILURE);
    }
    if(msgLen = msgrcv(clientId, &res, RESP_MSG_SIZE, RESP_SEQ, 0) == -1)
    {
        perror("client msgrcv failed");
        exit(EXIT_FAILURE);
    }

    printf("client: %d, seqNum: %d\n", getpid(), res.seqNum);

    return 0;
}