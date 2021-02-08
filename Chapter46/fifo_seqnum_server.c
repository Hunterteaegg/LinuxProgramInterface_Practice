#include "fifo_msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <signal.h>

static void grimReaper(int sig);
static void serveRequest(const struct request_msg *req, int seq);

int main(int argc, char* argv[])
{
    struct request_msg req;
    struct response_msg res;
    struct sigaction sigact;
    int seqNum = 0;
    int serverId;
    ssize_t msgLen;
    pid_t pid;

    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | 
                         S_IRUSR | S_IWUSR | S_IWGRP);
    if(serverId == -1)
    {
        if(errno == EEXIST)
        {
            if(msgctl(serverId, IPC_RMID, NULL) == -1)
            {
                perror("server msgctl remove failed");
            }
            serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                                S_IRUSR | S_IWUSR | S_IWGRP);
        }
        else
        {
            perror("server msgget failed");
            exit(EXIT_FAILURE);
        }
    }

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;
    sigact.sa_handler = grimReaper;
    if(sigaction(SIGCHLD, &sigact, NULL) == -1)
    {
        perror("server sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("server is running\n");

    for(;;)
    {
       msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, REQ_TYPE, 0);
       if(msgLen == -1)
       {
           if(errno == EINTR)
           {
               continue;
           }
           perror("msgrcv");
           break;
       }
       printf("server get message.\n");
       
       switch(pid = fork())
       {
           case -1:
           {
               perror("server fork failed");
               break;
           }
           case 0:
           {
               serveRequest(&req, seqNum);
               exit(EXIT_SUCCESS);
           }
           default:
           {
               seqNum += req.seqLen;
           }
       }
    }

    if(msgctl(serverId, IPC_RMID, NULL) == -1)
    {
        perror("server msg remove failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

static void grimReaper(int sig)
{
    int savedErrno;

    savedErrno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0)
    {
        continue;
    }
    errno = savedErrno;
}

static void serveRequest(const struct request_msg *req, int seq)
{
    struct response_msg res;

    res.mtype = RESP_SEQ;
    res.seqNum = seq;

    msgsnd(req->clientId, &res, RESP_MSG_SIZE, 0);
    printf("server num: %d\n", seq);
}
