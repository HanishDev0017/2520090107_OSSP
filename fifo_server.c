#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define SIZE 256

int main()
{
    char buffer[SIZE];

    unlink(SERVER_FIFO);
    mkfifo(SERVER_FIFO, 0666);

    printf("FIFO SERVER STARTED\n");
    printf("Waiting for clients...\n");

    int fd = open(SERVER_FIFO, O_RDWR);

    while (1)
    {
        memset(buffer, 0, SIZE);

        int n = read(fd, buffer, SIZE - 1);

        if (n > 0)
        {
            buffer[n] = '\0';

            char *separator = strchr(buffer, '|');

            if (separator != NULL)
            {
                *separator = '\0';

                char *pid = buffer;
                char *message = separator + 1;

                printf("Client %s: %s", pid, message);

                for (int i = 0; message[i] != '\0'; i++)
                    message[i] = toupper(message[i]);

                char client_fifo[100];
                snprintf(client_fifo, sizeof(client_fifo),
                         "/tmp/client_%s.fifo", pid);

                char response[SIZE];
                snprintf(response, sizeof(response),
                         "Server processed: %s", message);

                int client_fd = open(client_fifo, O_WRONLY);

                if (client_fd != -1)
                {
                    write(client_fd, response, strlen(response));
                    close(client_fd);
                }
            }
        }
    }

    close(fd);
    unlink(SERVER_FIFO);

    return 0;
}
