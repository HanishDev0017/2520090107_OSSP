#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int value = 10;

    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child process\n");
        printf("Value before modification: %d\n", value);

        value = 50;

        printf("Value after modification: %d\n", value);
    }
    else
    {
        sleep(1);

        printf("Parent process\n");
        printf("Parent value: %d\n", value);
    }

    return 0;
}
