#include "msg_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

static volatile int seqNum = 0;
static pthread_mutex_t seqNumMutex = PTHREAD_MUTEX_INITIALIZER;

static void* serveRequest(void *param);

int main()
{
    struct request_msg req;
    pthread_t threadID;
    pthread_attr_t threadAttr;
    mqd_t serverID;
    struct mq_attr serverAttr = {
        .mq_maxmsg = MAX_MSG,
        .mq_msgsize = sizeof(struct request_msg),
    };

    serverID = mq_open(SERVER_PATH, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &serverAttr);
    if(serverID == -1)
    {
        perror("server open message queue failed");
        exit(EXIT_FAILURE);
    }
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    for(;;)
    {
        if(mq_receive(serverID, (char*)&req, sizeof(struct request_msg), NULL) == -1)
        {
            perror("server receive message failed");
            exit(EXIT_FAILURE);
        }
        if(pthread_create(&threadID, &threadAttr, serveRequest, &req) != 0)
        {
            perror("server create thread failed");
            exit(EXIT_FAILURE);
        }
    }
}

static void* serveRequest(void *param)
{
    struct request_msg *req = (struct request_msg *)param;
    struct response_msg res;
    mqd_t clientId;

    pthread_mutex_lock(&seqNumMutex);
    res.seqNum = seqNum;
    seqNum += req->seqLen;
    pthread_mutex_unlock(&seqNumMutex);

    clientId = mq_open(req->clientPath, O_RDWR);
    if(clientId == -1)
    {
        perror("server open client queue failed");
        exit(EXIT_FAILURE);
    }

    if(mq_send(clientId, (char*)&res, sizeof(struct response_msg), 0) == -1)
    {
        perror("server send message failed");
        exit(EXIT_FAILURE);
    }

    return NULL;
}
