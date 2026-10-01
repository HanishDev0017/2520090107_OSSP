#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define BUFFER_SIZE 256

int main()
{
    char message[BUFFER_SIZE];
    char client_fifo[BUFFER_SIZE];

    snprintf(client_fifo, BUFFER_SIZE,
             "/tmp/client_%d_fifo", getpid());

    mkfifo(client_fifo, 0666);

    printf("Enter message: ");
    fgets(message, BUFFER_SIZE, stdin);

    message[strcspn(message, "\n")] = '\0';

    int server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1)
    {
        perror("Server FIFO");
        unlink(client_fifo);
        exit(1);
    }

    char request[BUFFER_SIZE];

    snprintf(request, BUFFER_SIZE,
             "%s|%s", client_fifo, message);

    write(server_fd, request, strlen(request));
    close(server_fd);

    int client_fd = open(client_fifo, O_RDONLY);

    if (client_fd == -1)
    {
        perror("Client FIFO");
        unlink(client_fifo);
        exit(1);
    }

    char response[BUFFER_SIZE];

    int n = read(client_fd, response, BUFFER_SIZE - 1);

    if (n > 0)
    {
        response[n] = '\0';
        printf("Response from server: %s\n", response);
    }

    close(client_fd);
    unlink(client_fifo);

    return 0;
}
