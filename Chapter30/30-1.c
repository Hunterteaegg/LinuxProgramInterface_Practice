#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct thread_info
{
    int thread_id;
    int cycle_time;
};

static int glob = 0;

static void* threadFunc(void *arg)
{
    int id = ((struct thread_info *)arg)->thread_id;
    int loops = ((struct thread_info *)arg)->cycle_time;
    int loc, j;

    for(j = 0; j < loops; j++)
    {
        loc = glob;
        loc++;
        glob = loc;
        printf("thread ID: %ld, glob= %ld.\n", id, loops);
    }
}

int main()
{
    pthread_t t1, t2;
    int s;
    struct thread_info infos[2] = {
        {
            .thread_id= 0,
            .cycle_time = 1000000
        },
        {
            .thread_id = 1,
            .cycle_time = 5000000
        },
    };

    s = pthread_create(&t1, NULL, threadFunc, &infos[0]);
    if(s != 0) 
    {
        perror("pthread create failed.\n");
        exit(EXIT_FAILURE);
    }
    s = pthread_create(&t2, NULL, threadFunc, &infos[1]);
    if(s != 0)
    {
        perror("pthread create failed.\n");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t1, NULL);
    if(s != 0)
    {
        perror("pthread join failed.\n");
        exit(EXIT_FAILURE);
    }
    s = pthread_join(t2, NULL);
    if(s != 0)
    {
        perror("pthread join failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}