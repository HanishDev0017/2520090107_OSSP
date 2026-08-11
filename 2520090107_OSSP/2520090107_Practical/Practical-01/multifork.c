#include <stdio.h>
#include <unistd.h>

int main()
{
    fork();
    fork();

    printf("Hello\n");

    return 0;
}
