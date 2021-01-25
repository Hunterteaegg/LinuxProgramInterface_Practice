#define _POSIX_C_SOURCE 199309

#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void sigintHandler(int sig)
{
    return;
}

int main(int argc, char *argv[])
{
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    int s;

    if(argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "%s secs nanosecs\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    request.tv_sec = atoi(argv[1]);
    request.tv_nsec = atoi(argv[2]);
    printf("%d sec and %d nanosec.\n", request.tv_sec, request.tv_nsec);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        fprintf(stderr, "sigaction failed.\n");
        exit(EXIT_FAILURE);
    }

    if(gettimeofday(&start, NULL) == -1)
    {
        fprintf(stderr, "gettimeofday failed.\n");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        s = nanosleep(&request , &remain);
        if(s == -1 && errno != EINTR)
        {
            fprintf(stderr, "nanosleep failed.\n");
            exit(EXIT_FAILURE);
        }

        if(gettimeofday(&finish, NULL) == -1)
        {
            fprintf(stderr, "gettimeofday failed.\n");
            exit(EXIT_FAILURE);
        }
        printf("Slept for %9.6f secs\n", finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec) / 1000000.0);
        
        if(s == 0)
        {
            break;
        }

        printf("Remaining: %2ld.%09ld\n", (long)remain.tv_sec, remain.tv_nsec);
        request = remain;
    }

    printf("Sleep complete\n");

    return 0;
}