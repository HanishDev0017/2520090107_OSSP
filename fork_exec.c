#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t pid;

    pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
    }
    else if (pid == 0) {
        printf("Child process\n");
        printf("Child PID: %d\n", getpid());

        execl("/bin/ls", "ls", "-l", NULL);

        printf("exec failed\n");
    }
    else {
        printf("Parent process\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);

        wait(NULL);

        printf("Child finished\n");
    }

    return 0;
}
