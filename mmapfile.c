#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char *data;

    fd = open("mmap.txt", O_RDWR | O_CREAT, 0644);

    ftruncate(fd, 100);

    data = mmap(
        NULL,
        100,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    strcpy(data, "Memory mapped file example.");

    printf("%s\n", data);

    munmap(data, 100);

    close(fd);

    return 0;
}
