#ifndef ALCORE_H
#define ALCORE_H

#define DEBUG
//#define CAIRO_DEBUG
#define PI (3.14159265358979)

typedef struct 
{
    double x;
    double y;
} alpoint2d;

void alerror(const char *msg);
void aluierror(const char *msg);

#endif
