#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid1, pid2;

    pid1 = fork();

    if (pid1 == 0)
    {
        printf("Child 1 PID: %d\n", getpid());
        sleep(3);
        exit(0);
    }

    pid2 = fork();

    if (pid2 == 0)
    {
        printf("Child 2 PID: %d\n", getpid());
        sleep(5);
        exit(0);
    }

    wait(NULL);

    printf("First child completed.\n");

    waitpid(pid2, NULL, 0);

    printf("Second child completed.\n");

    return 0;
}
