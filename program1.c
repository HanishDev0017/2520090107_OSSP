#include <unistd.h>

int main()
{
    write(STDOUT_FILENO, "Hello World\n", 12);
    return 0;
}
