#include "seq.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int client_fd;
    int seqLen;
    struct sockaddr_un client_info;
    struct sockaddr_un server_info;
    struct request_msg req;
    struct response_msg res;

    if(argc != 2)
    {
        fprintf(stderr, "argument number invalid\n");
        exit(EXIT_FAILURE);
    }
    seqLen = atoi(argv[1]);
    
    memset(&server_info, 0, sizeof(server_info));
    server_info.sun_family = AF_UNIX;
    strncpy(&server_info.sun_path[1], SERVER_SOCK, sizeof(server_info.sun_path) - 2);

    req.seqLen = seqLen;

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(client_fd < 0)
    {
        perror("client create socket failed");
        exit(EXIT_FAILURE);
    }
    
    if(connect(client_fd, (struct sockaddr *)&server_info, sizeof(server_info)) == -1)
    {
        perror("client connect socket failed");
        exit(EXIT_FAILURE);
    }

    if(write(client_fd, &req, sizeof(req)) != sizeof(req))
    {
        fprintf(stderr, "client write socket failed\n");
        exit(EXIT_FAILURE);
    }
    if(read(client_fd, &res, sizeof(res)) != sizeof(res))
    {
        fprintf(stderr, "client read socket failed\n");
        exit(EXIT_FAILURE);
    }
    printf("client seqNum: %d\n", res.seqNum);
    if(close(client_fd) != 0)
    {
        perror("client close socket failed");
        exit(EXIT_FAILURE);
    }
}