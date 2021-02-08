#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stddef.h>
#include <fcntl.h>

#define SERVER_KEY  (0x4A)

struct request_msg
{
    long mtype;

    int clientId;
    int seqLen;
};

struct response_msg
{
    long mtype;

    int seqNum;
};

#define REQ_MSG_SIZE    (offsetof(struct request_msg, seqLen)   - \
                         offsetof(struct request_msg, clientId) + sizeof(int))
#define RESP_MSG_SIZE   (sizeof(int))

#define REQ_TYPE        (1L << 0) 
#define RESP_FAILURE    (1L << 1) 
#define RESP_SEQ        (1L << 2) 