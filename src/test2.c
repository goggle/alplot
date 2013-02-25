#include <stdio.h>

int testStatic()
{
    static int i = 0;
    printf("%d\n", i);
    i++;
}


int main(void)
{
    testStatic();
    testStatic();
    testStatic();

    
    return 0;
}
