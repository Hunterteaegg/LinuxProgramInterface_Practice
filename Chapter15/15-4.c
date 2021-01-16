#include <stdio.h>
#include <unistd.h>

int main()
{
    char *pathName = "./test.o";
    int stat;

    stat = access(pathName, F_OK);
    if(stat == 0)
    {
        printf("File exists.\n");
    }
    else
    {
        printf("File misses.\n");
    }
    
    stat = access(pathName, R_OK);
    if(stat == 0)
    {
        printf("File readable.\n");
    }
    else
    {
        printf("File unreadable.\n");
    }

    stat = access(pathName, W_OK);
    if(stat == 0)
    {
        printf("File writable.\n");
    }
    else
    {
        printf("File unwritable.\n");
    }
    
    return 0;
}