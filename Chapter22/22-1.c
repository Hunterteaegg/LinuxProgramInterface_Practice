#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void signal_handler(int sig);

int main()
{
    sigset_t sig;
    sigset_t prev_sig;
    struct sigaction sigcont_action = {
        .__sigaction_handler = signal_handler,
    };

    if(sigaction(SIGCONT, &sigcont_action, NULL) != 0)
    {
        fprintf(stderr, "sigaction failed.\n");
    }
    if(sigemptyset(&sig) != 0)
    {
        fprintf(stderr, "sigemptyset failed.\n");
    }

    if(sigaddset(&sig, SIGCONT) != 0)
    {
        fprintf(stderr, "sigaddset failed.\n");
    }

    if(sigprocmask(SIG_BLOCK, &sig, &prev_sig) != 0)
    {
        fprintf(stderr, "sigprocmask failed.\n");
    }

    for(int i = 0; i < 5; i++)
    {
        printf("%d\n", i);
        sleep(5);
    }

    if(sigprocmask(SIG_SETMASK, &prev_sig, NULL) != 0)
    {
        fprintf(stderr, "sigprocmask failed.\n");
    }

    return 0;
}

static void signal_handler(int sig)
{
    char *prompt = "you have deposited SIGCONT.\n";

    write(STDOUT_FILENO, prompt, sizeof(prompt));
    fflush(stdout);
}