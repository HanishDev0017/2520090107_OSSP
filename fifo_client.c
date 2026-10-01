#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define SIZE 256

int main()
{
    char message[SIZE];
    char response[SIZE];

    pid_t pid = getpid();

    char client_fifo[100];

    snprintf(client_fifo, sizeof(client_fifo),
             "/tmp/client_%d.fifo", pid);

    unlink(client_fifo);
    mkfifo(client_fifo, 0666);

    printf("Enter message: ");
    fgets(message, SIZE, stdin);

    int server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1)
    {
        perror("Server is not running");
        unlink(client_fifo);
        return 1;
    }

    char request[SIZE];

    snprintf(request, sizeof(request),
             "%d|%s", pid, message);

    write(server_fd, request, strlen(request));
    close(server_fd);

    int client_fd = open(client_fifo, O_RDWR);

    if (client_fd == -1)
    {
        perror("Client FIFO");
        unlink(client_fifo);
        return 1;
    }

    int n = read(client_fd, response, SIZE - 1);

    if (n > 0)
    {
        response[n] = '\0';
        printf("SERVER RESPONSE: %s\n", response);
    }

    close(client_fd);
    unlink(client_fifo);

    return 0;
}
