#ifndef ALLINE_H
#define ALLINE_H

#include <stdbool.h>
#include "color.h"

typedef struct
{
    double x1;
    double y1;
    double x2;
    double y2;
    rgb_color linecolor;
    double alpha;
    double linewidth;
    int linestyle;
    bool visible;
    bool legend;
} alline;


typedef struct
{
    double x;
    double y;
    double width;
    double height;
    rgba_color linecolor;
    double linewidth;
    bool line_visible[4]; /*south, east, north, west*/
    rgba_color facecolor;
    bool visible;
    bool rounded_corners;
} albox;


alline* alline_create(double x1, double y1, double x2, double y2);
void alline_set_visible(alline *line);
void alline_unset_visible(alline *line);
void alline_set_legend(alline *line);
void alline_unset_legend(alline *line);
void alline_set_color_rgba(alline *line, double r, double g, double b, double alpha);
void alline_set_color_rgb(alline *line, double r, double g, double b);
void alline_set_alpha(alline *line, double alpha);
void alline_set_linewidth(alline *line, double lw);

albox* albox_create(double x, double y, double width, double height);
void albox_set_edgecolor_rgba(albox *box, double r, double g, double b, double alpha);
void albox_set_facecolor_rgba(albox *box, double r, double g, double b, double alpha);
void albox_set_linewidth(albox *box, double lw);

#endif
