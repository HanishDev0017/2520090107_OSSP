#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child process\n");
        exit(0);
    }
    else
    {
        printf("Parent process\n");
        sleep(30);
    }

    return 0;
}
