#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i;

    int *ptr1 = (int *)malloc(5 * sizeof(int));

    printf("Memory allocated using malloc()\n");

    for (i = 0; i < 5; i++)
    {
        ptr1[i] = i + 1;
        printf("%d ", ptr1[i]);
    }

    printf("\n");

    int *ptr2 = (int *)calloc(5, sizeof(int));

    printf("Memory allocated using calloc()\n");

    for (i = 0; i < 5; i++)
    {
        ptr2[i] = i + 10;
        printf("%d ", ptr2[i]);
    }

    printf("\n");

    ptr1 = realloc(ptr1, 10 * sizeof(int));

    printf("Memory reallocated using realloc()\n");

    free(ptr1);
    free(ptr2);

    printf("Memory freed successfully.\n");

    return 0;
}
