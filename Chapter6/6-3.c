#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 64

char *g_env_name = "SCHOOL";
char *g_env_value = "SCAU";
char g_buffer[BUFFER_SIZE + 1];

int my_setenv(const char *name, const char *value, int overwrite);
int my_unsetenv(const char *name);

int main()
{
    printf("env : SCHOOL = %s \n", getenv(g_env_name));
    my_setenv(g_env_name, g_env_value, 1);
    printf("after my_setenv : SCHOOL=%s \n", getenv(g_env_name));
    my_unsetenv(g_env_name);
    printf("after my_unsetenv : SCHOOL=%s \n", getenv(g_env_name));

    return 0;
}

int my_setenv(const char *name, const char *value, int overwrite)
{
    const char *env_value = getenv(name);

    if((NULL == env_value) || (0 != overwrite))
    {
        snprintf(g_buffer, BUFFER_SIZE, "%s=%s", name, value);
        printf("%s\n", g_buffer);
        putenv(g_buffer);
    }

    return 0;
}

int my_unsetenv(const char *name)
{
    char buffer[BUFFER_SIZE];

    snprintf(buffer, sizeof(BUFFER_SIZE), "%s=NULL", name);

    return 0;
}