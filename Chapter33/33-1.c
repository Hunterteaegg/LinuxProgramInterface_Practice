#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void* threadFun(void* param)
{
    sigset_t sig_fullset, sig_examset;

    sigfillset(&sig_fullset);
    sigprocmask(SIG_SETMASK, &sig_fullset, NULL);
    sleep(1);
    sigpending(&sig_examset);
    if(sigismember(&sig_examset, SIGUSR1) == 1)
    {
        printf("thread: SIGUSR1.\n");
    }
    else if(sigismember(&sig_examset, SIGUSR2) == 1)
    {
        printf("thread: SIGUSR2.\n");
    }

    return NULL;
}

int main()
{
    pthread_t thread1, thread2;
    int status;

    status = pthread_create(&thread1, NULL, threadFun, NULL);
    if(status != 0)
    {
        perror("create thread failed.\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_create(&thread2, NULL, threadFun, NULL);
    if(status != 0)
    {
        perror("create thread failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("create thread successfully.\n");

    status = pthread_kill(thread1, SIGUSR1);
    if(status != 0)
    {
        fprintf(stderr, "pthread_kill failed.\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_kill(thread2, SIGUSR2);
    if(status != 0)
    {
        fprintf(stderr, "pthread_kill failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("pthread_kill successed.\n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}