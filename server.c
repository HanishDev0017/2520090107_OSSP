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
    char buffer[BUFFER_SIZE];

    mkfifo(SERVER_FIFO, 0666);

    printf("Server started...\n");
    printf("Waiting for clients...\n");

    while (1)
    {
        int fd = open(SERVER_FIFO, O_RDONLY);

        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        int n = read(fd, buffer, BUFFER_SIZE - 1);
        close(fd);

        if (n > 0)
        {
            buffer[n] = '\0';

            char *separator = strchr(buffer, '|');

            if (separator != NULL)
            {
                *separator = '\0';

                char *client_fifo = buffer;
                char *message = separator + 1;

                printf("Client sent: %s\n", message);

                char response[BUFFER_SIZE];

                snprintf(response, BUFFER_SIZE,
                         "Server processed: %s", message);

                int response_fd = open(client_fifo, O_WRONLY);

                if (response_fd != -1)
                {
                    write(response_fd, response, strlen(response));
                    close(response_fd);
                }
            }
        }
    }

    return 0;
}
