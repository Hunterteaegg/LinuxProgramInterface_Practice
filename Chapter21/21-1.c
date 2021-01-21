#include <signal.h>
#include <unistd.h>

void my_abort(void)
{
    kill(getpid(), SIGABRT);
}