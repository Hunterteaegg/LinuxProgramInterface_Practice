#ifndef __MSG_TYPE_H__
#define __MSG_TYPE_H__

#include <signal.h>
#include <mqueue.h>

#define SERVER_PATH "/serverQueue"
#define MAX_MSG     10

struct request_msg
{
    char *clientPath;
    int seqLen;
};

struct response_msg
{
    int seqNum;
};

#endif