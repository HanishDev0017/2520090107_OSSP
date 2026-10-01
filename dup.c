#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd1, fd2;

    fd1 = open("student.txt", O_CREAT | O_WRONLY, 0644);

    if (fd1 == -1)
    {
        perror("open");
        return 1;
    }

    printf("fd1 = %d\n", fd1);

    fd2 = dup(fd1);

    printf("fd2 = %d\n", fd2);

    close(fd1);
    close(fd2);

    return 0;
}
