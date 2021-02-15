#include "seq.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>

static void grimReaper(int sig);
static void serveRequest(int fd, int seq);

int main(int argc, char *argv[])
{
    struct request_msg req;
    struct sockaddr_un server_info;
    struct sigaction sigact;
    int server_fd, client_fd;
    int seqNum = 0;
    pid_t pid;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;
    sigact.sa_handler = grimReaper;
    if(sigaction(SIGCLD, &sigact, NULL) == -1)
    {
        perror("server sigaction failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sun_family = AF_UNIX;
    strncpy(&server_info.sun_path[1], SERVER_SOCK, sizeof(server_info.sun_path) - 2);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        perror("server create sockets failed");
        exit(EXIT_FAILURE);
    }

    if(bind(server_fd, (struct sockaddr*)&server_info, sizeof(server_info)) != 0)
    {
        perror("server bind socket failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, SERVER_BACKLOG) != 0)
    {
        perror("server listen socket failed");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        client_fd = accept(server_fd, NULL, NULL);
        if(client_fd < 0)
        {
            perror("server accept socket failed");
            exit(EXIT_FAILURE);
        }
        if(read(client_fd, &req, sizeof(req)) != sizeof(req))
        {
            fprintf(stderr, "server read socket failed\n");
            continue;
        }
        switch(pid = fork())
        {
            case -1:
            {
                perror("server fork failed");
                exit(EXIT_FAILURE);
            }
            case 0:
            {
                serveRequest(client_fd, seqNum);
                exit(EXIT_SUCCESS);
            }
            default:
            {
                seqNum += req.seqLen;
            }
        }
    }
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

static void serveRequest(int fd, int seq)
{
    struct response_msg res;

    res.seqNum = seq;
    if(write(fd, &res, sizeof(res)) != sizeof(res))
    {
        fprintf(stderr, "server write socket failed\n");
        exit(EXIT_FAILURE);
    }
}

