#include <stdio.h>
#include <sys/time.h>
#include <time.h>

static struct itimerval timervalue;

unsigned int my_alarm(unsigned int seconds)
{
    timervalue.it_value.tv_sec = seconds;

    return setitimer(ITIMER_REAL, &timervalue, NULL);
}

int main()
{
    struct timespec sleeptime = {
        .tv_sec = 10,
    };

    printf("timer begin.\n");
    my_alarm(5);
    nanosleep(&sleeptime, NULL);
    printf("timer ended.\n");

    return 0;
}