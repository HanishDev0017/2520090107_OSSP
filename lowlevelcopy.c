#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int source, destination;
    char buffer[100];
    int bytes;

    source = open("input.txt", O_RDONLY);

    destination = open(
        "output.txt",
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    lseek(source, 0, SEEK_SET);

    while ((bytes = read(source, buffer, sizeof(buffer))) > 0)
    {
        write(destination, buffer, bytes);
    }

    close(source);
    close(destination);

    printf("File copied successfully.\n");

    return 0;
}
