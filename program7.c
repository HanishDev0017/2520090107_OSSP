#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;

    fd = open("student.txt", O_WRONLY | O_TRUNC);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd, "New Data\n", 9);

    close(fd);

    return 0;
}
