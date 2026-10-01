#include <unistd.h>

int main()
{
    char buffer[20];

    read(STDIN_FILENO, buffer, sizeof(buffer));
    write(STDOUT_FILENO, buffer, sizeof(buffer));

    return 0;
}
