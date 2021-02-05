#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("user: %s.\n", getlogin());
    printf("tty: %s.\n", ttyname(STDIN_FILENO));

    return 0;
}