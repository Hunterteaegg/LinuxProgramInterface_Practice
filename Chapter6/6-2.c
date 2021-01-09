#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

static jmp_buf env;

static void f1(void)
{
    switch (setjmp(env))
    {
        case 0:
        {
            printf("setjmp successfully.\n");
            break;
        }
        case 1:
        {
            printf("longjmp successfully.\n");
            break;
        }
        default:
        {
            printf("longjmp failed.\n");
            break;
        }
    }
}

static void f2(void)
{
    longjmp(env, 1);
}

int main()
{
    f1();
    f2();

    return 0;
}