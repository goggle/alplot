#include <stdlib.h>
#include <math.h>
#include "alDraw.h"


/*
 * Transorm the figure coordinates fig_p into screen coordinates
 */
alpoint2d fig_to_world(alpoint2d fig_p, alfigure *fig)
{
    alpoint2d world_p = {FIGURE_X, FIGURE_Y};
    world_p.x += (fig_p.x - fig->xlim[0])/(fig->xlim[1] - fig->xlim[0]) * FIGURE_WIDTH;
    world_p.y -= (fig_p.y - fig->ylim[0])/(fig->ylim[1] - fig->ylim[0]) * FIGURE_HEIGHT;
    return world_p;
}

/*
 * Calculates the coordinates for an arrow from 'begin' to 'end' width a head length 'headlength' and
 * the half of the head angle 'angle'.
 * 'angle' should be in the range [0, pi/2).
 */
alarrow aldraw_calculate_arrow(alpoint2d begin, alpoint2d end, double headlength, double angle)
{
    double w, h;
    double norm;
    double v1x, v1y; /* normed vector from 'end' in the direction of 'begin' */
    double v2x, v2y; /* normed vector from 'end' perpendicular to the line */
    double px, py;
    alarrow arrow = { .begin = {begin.x, begin.y}, .end = {end.x, end.y}, .head1 = {0.0, 0.0}, .head2 = {0.0, 0.0}, .valid = false };


    if (fabs(end.x - begin.x) < EPS && fabs(end.y - begin.y) < EPS) {
        return arrow;
    }

    if (headlength <= 0.0 
            || angle > M_PI / 2.0 
            || angle < EPS) {
        return arrow;
    }

    h = sin(angle) * headlength;
    if (headlength * headlength - h * h < 0) {
        return arrow;
    }
    w = sqrt(headlength * headlength - h * h);

    norm = sqrt((begin.x - end.x) * (begin.x - end.x) + (begin.y - end.y) * (begin.y - end.y));
    v1x = (begin.x - end.x) / norm;
    v1y = (begin.y - end.y) / norm;
    v2x = -v1y;
    v2y = v1x;
    px = end.x + v1x * w;
    py = end.y + v1y * w;
    px = px + v2x * h;
    py = py + v2y * h;
    arrow.head1.x = px;
    arrow.head1.y = py;

    px = end.x + v1x * w;
    py = end.y + v1y * w;
    px = px - v2x * h;
    py = py - v2y * h;
    arrow.head2.x = px;
    arrow.head2.y = py;

    arrow.valid = true;
    return arrow;
}


/*
 * Returns the coordinates of the ticks:
 *
 * fig: The figure object
 * pos: 'n': north (xticks)
 *      'e': east  (yticks)
 *      's': south (xticks)
 *      'w': west  (yticks)
 * subticks: false: Draw the main ticks
 *           true: Draw the subticks
 */
alline* aldraw_get_ticks(alfigure *fig, char pos, bool subticks)
{
    unsigned int i;
    double d;
    alline* ticks = NULL;
    alpoint2d p = {0.0, 0.0};

    if (pos == 'n' || pos == 's') {
        if (pos == 's')
            p.y = fig->ylim[0];
        else
            p.y = fig->ylim[1];

            
        if (!subticks) {
            d = fig->ticklength * 0.5;
            ticks = (alline *) malloc(fig->nxticks * sizeof(alline));
            if (ticks == NULL)
                alerror("get_ticks: Could not allocate memory!");
            for (i = 0; i < fig->nxticks; i++) {
                p.x = fig->xticks[i];
                ticks[i].begin = ticks[i].end = fig_to_world(p, fig);
                ticks[i].begin.y += d; /* bottom */
                ticks[i].end.y -= d; /* top */
            }
        }
        else {
            d = fig->subticklength * 0.5;
            ticks = (alline *) malloc(fig->nsubxticks * sizeof(alline));
            if (ticks == NULL)
                alerror("get_ticks: Could not allocate memory!");
            for (i = 0; i < fig->nsubxticks; i++) {
                p.x = fig->subxticks[i];
                ticks[i].begin = ticks[i].end = fig_to_world(p, fig);
                ticks[i].begin.y += d; /* bottom */
                ticks[i].end.y -= d; /* top */
            }
        }
    }

    else if (pos == 'e' || pos == 'w') {
        if (pos == 'w')
            p.x = fig->xlim[0];
        else
            p.x = fig->xlim[1];

        if (!subticks) {
            d = fig->ticklength * 0.5;
            ticks = (alline *) malloc(fig->nyticks * sizeof(alline));
            if (ticks == NULL)
                alerror("get_ticks: Could not allocate memory!");
            for (i = 0; i < fig->nyticks; i++) {
                p.y = fig->yticks[i];
                ticks[i].begin = ticks[i].end = fig_to_world(p, fig);
                ticks[i].begin.x -= d; /* left */
                ticks[i].end.x += d; /* right */
            }
        }
        else {
            d = fig->subticklength * 0.5;
            ticks = (alline *) malloc(fig->nsubyticks * sizeof(alline));
            if (ticks == NULL)
                alerror("get_ticks: Could not allocate memory!");
            for (i = 0; i < fig->nsubyticks; i++) {
                p.y = fig->subyticks[i];
                ticks[i].begin = ticks[i].end = fig_to_world(p, fig);
                ticks[i].begin.x -= d; /* left */
                ticks[i].end.x += d; /* right */
            }
        }
    }
    return ticks;
}




