#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t running = 1;

void handler(int sig)
{
    if (sig == SIGINT)
        write(1, "\nSIGINT received (Ctrl+C)\n", 26);

    else if (sig == SIGTERM)
    {
        write(1, "\nSIGTERM received\n", 18);
        running = 0;
    }

    else if (sig == SIGUSR1)
        write(1, "\nSIGUSR1 received\n", 18);
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    printf("SIGNAL HANDLING PROGRAM\n");
    printf("PID: %d\n", getpid());
    printf("Ctrl+C = SIGINT\n");
    printf("kill -USR1 %d = SIGUSR1\n", getpid());
    printf("kill -TERM %d = SIGTERM\n", getpid());

    while (running)
    {
        printf("Program is running...\n");
        sleep(3);
    }

    printf("Program terminated safely.\n");

    return 0;
}
