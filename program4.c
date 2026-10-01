#include <unistd.h>

int main()
{
    write(STDERR_FILENO, "This is an error\n", 17);
    return 0;
}
