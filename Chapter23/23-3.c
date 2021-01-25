#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    timer_t timer_id;
    struct itimerspec timer_value = {
        .it_value.tv_sec = 60,
    };

    if(timer_create(CLOCK_REALTIME, NULL, &timer_id) != 0)
    {
        perror("timer_create failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("timer_create successfully.\n");

    if(timer_settime(timer_id, 0, &timer_value, NULL) != 0)
    {
        perror("timer_settime failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("timer_settime successfully.\n");

    
}