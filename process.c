#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Before fork()\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("fork() failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        printf("I am the CHILD process\n");
        printf("Child PID: %d\n", getpid());
    }
    else
    {
        printf("I am the PARENT process\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
    }

    return 0;
}
