#include <stdio.h>
#include <stdlib.h>
#include "alLine.h"

int main(void)
{
    //alLine_create(0.0, 0.0, 5.0, 6.0);
    //line.x2 = 2.0;
    alline *line = alline_create(0.0, 0.0, 5.0, 6.0);
    printf("%f\n", line->x2);

    free(line);


    return 0;
}
