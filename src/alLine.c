#include "alLine.h"
#include "alCore.h"
#include <stdlib.h>

alline* alline_create(double x1, double y1, double x2, double y2)
{
    alline *line = (alline *) malloc(sizeof(alline));
    if (line == NULL)
        alerror("alline_create: Could not allocate memory.");
    line->x1 = x1;
    line->y1 = y1;
    line->x2 = x2;
    line->y2 = y2;
    line->linecolor.r = 0.0;
    line->linecolor.g = 0.0;
    line->linecolor.b = 0.0;
    line->alpha = 0.0;
    line->linewidth = 1.0;
    line->linestyle = 0;
    line->visible = true;
    line->legend = true;

    return line;
}

void alline_set_visible(alline *line)
{
    line->visible = true;
}

void alline_unset_visible(alline *line)
{
    line->visible = false;
}

void alline_set_legend(alline *line)
{
    line->legend = true;
}

void alline_unset_legend(alline *line)
{
    line->legend = false;
}

void alline_set_color_rgba(alline *line, double r, double g, double b, double alpha)
{
    line->linecolor.r = r;
    if (r > 1.0)
        line->linecolor.r = 1.0;
    else if (r < 0.0)
        line->linecolor.r = 0.0;

    line->linecolor.g = g;
    if (g > 1.0)
        line->linecolor.g = 1.0;
    else if (g < 0.0)
        line->linecolor.g = 0.0;

    line->linecolor.b = b;
    if (b > 1.0)
        line->linecolor.b = 1.0;
    else if (b < 0.0)
        line->linecolor.b = 0.0;

    line->alpha = alpha;
    if (alpha > 1.0)
        line->alpha = 1.0;
    else if (alpha < 0.0)
        line->alpha = 0.0;
}

void alline_set_color_rgb(alline *line, double r, double g, double b)
{
    line->linecolor.r = r;
    if (r > 1.0)
        line->linecolor.r = 1.0;
    else if (r < 0.0)
        line->linecolor.r = 0.0;

    line->linecolor.g = g;
    if (g > 1.0)
        line->linecolor.g = 1.0;
    else if (g < 0.0)
        line->linecolor.g = 0.0;

    line->linecolor.b = b;
    if (b > 1.0)
        line->linecolor.b = 1.0;
    else if (b < 0.0)
        line->linecolor.b = 0.0;
}

void alline_set_alpha(alline *line, double alpha)
{
    line->alpha = alpha;
    if (alpha > 1.0)
        alpha = 1.0;
    else if (alpha < 0.0)
        alpha = 0.0;
}

void alline_set_linewidth(alline *line, double lw)
{
    line->linewidth = lw;
}


albox* albox_create(double x, double y, double width, double height)
{
    int i;
    albox *box = (albox *) malloc(sizeof(albox));
    box->x = x;
    box->y = y;
    box->width = width;
    box->height = height;
    box->linecolor.r = 0.0;
    box->linecolor.g = 0.0;
    box->linecolor.b = 0.0;
    box->linecolor.alpha = 1.0;
    box->linewidth = 1.0;
    for (i = 0; i < 4; ++i)
        box->line_visible[i] = true;
    box->facecolor.r = 1.0;
    box->facecolor.g = 1.0;
    box->facecolor.b = 1.0;
    box->facecolor.alpha = 0.0;
    box->visible = true;
    box->rounded_corners = false;
    return box;
}

    

void albox_set_edgecolor_rgba(albox *box, double r, double g, double b, double alpha)
{
    box->linecolor.r = r;
    box->linecolor.g = g;
    box->linecolor.b = b;
    box->linecolor.alpha = alpha;
}


void albox_set_facecolor_rgba(albox *box, double r, double g, double b, double alpha)
{
    box->facecolor.r = r;
    box->facecolor.g = g;
    box->facecolor.b = b;
    box->facecolor.alpha = alpha;
}

void albox_set_linewidth(albox *box, double lw)
{
    box->linewidth = lw;
}

