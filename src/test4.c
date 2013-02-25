#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i = 0;
    int *arr;
    arr = (int *) malloc(100 * sizeof(int));

    for (i = 0; i < 100; i++)
        arr[i] = i;

    int *p;
    p = arr;

    free(p);

    return 0;
}
    
