#ifndef COLOR_H
#define COLOR_H

typedef struct
{
    double r;
    double g;
    double b;
    double alpha;
} rgba_color;

typedef struct
{
    double r;
    double g;
    double b;
} rgb_color;

typedef struct
{
    char r;
    char g;
    char b;
} rgb256_color;

#endif
