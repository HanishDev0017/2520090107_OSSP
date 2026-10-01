#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int file;

    file = open(
        "redirect.txt",
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    dup2(file, STDOUT_FILENO);

    printf("This text is redirected to the file.\n");

    close(file);

    return 0;
}
