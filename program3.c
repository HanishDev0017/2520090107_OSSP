#include <stdio.h>
#include <unistd.h>

int main()
{
    char buffer[100];
    int n;

    printf("Enter something: ");

    n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        write(STDOUT_FILENO, buffer, n);
    }

    return 0;
}
