#include <unistd.h>
#include <stdio.h>

extern int optind, opterr, optopt;
extern char *optarg;

int main(int argc, char *argv[])
{
    int opt, xfnd;
    char *pstr;

    while((opt = getopt(argc, argv, ":p:x"))!= -1)
    {
        printf("opt = %3d(%c); optind = %d.\n", opt, opt, optind);

        switch(opt)
        {
            case 'p':
            {
                printf("the value of p is %s.\n", optarg);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if(optind < argc)
    {
        printf("First nonoption argument is \"%s\" at argv[%d]\n.", argv[optind], optind);
    }

    return 0;
}