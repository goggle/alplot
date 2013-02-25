#include "alCore.h"
#include <stdio.h>
#include <stdlib.h>


void alerror(const char *msg)
{
    printf("alerror\n");
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

void aluierror(const char *msg)
{
    printf("UI Error.\n");
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}
