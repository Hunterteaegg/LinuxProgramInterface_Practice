#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SERVER_SOCK     "server"
#define SERVER_BACKLOG  10

struct request_msg
{
    int seqLen;
};

struct response_msg
{
    int seqNum;
};