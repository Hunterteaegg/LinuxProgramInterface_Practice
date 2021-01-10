#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passwd *my_getpwnam(const char *name);

int main()
{
    struct passwd *pi_password = my_getpwnam("pi");
    if(pi_password == NULL)
    {
        fprintf(stderr, "get Pi password failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Pi password : %s\n", pi_password->pw_passwd);

    return 0;
}

struct passwd *my_getpwnam(const char *name)
{
    struct passwd *pwd;

    if(NULL == name)
    {
        fprintf(stderr, "my_getpwname : invaild parameter.");
        exit(EXIT_FAILURE);
    }

    while((pwd = getpwent()) != NULL)
    {
        if(strcmp(name, pwd->pw_name) == 0)
        {
            return pwd;
        }
    }

    endpwent();

    return NULL;
}