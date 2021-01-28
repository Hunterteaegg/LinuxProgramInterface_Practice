#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

static pthread_t thread;

static void* task(void *param)
{
    write(STDIN_FILENO, "task one running.\n", sizeof("task one running.\n"));
    pthread_join(pthread_self(), NULL);
}

int main()
{
    pthread_create(&thread, NULL, task, NULL);
    printf("thread create successfully.\n");

    exit(EXIT_SUCCESS);
}