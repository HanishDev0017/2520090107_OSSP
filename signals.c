#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig)
{
    if (sig == SIGINT)
        printf("\nReceived SIGINT (Ctrl+C)\n");
    else if (sig == SIGTERM)
        printf("\nReceived SIGTERM\n");
    else if (sig == SIGUSR1)
        printf("\nReceived SIGUSR1\n");

    fflush(stdout);
}

int main()
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGUSR1, signal_handler);

    printf("Signal handling program started.\n");
    printf("Process ID: %d\n", getpid());
    printf("Waiting for signals...\n");

    while (1)
    {
        printf("Program is running...\n");
        sleep(3);
    }

    return 0;
}
