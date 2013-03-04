#ifndef ALDRAW_H
#define ALDRAW_H

/*
 * All the calculations used for drawing should should be done here.
 */
#include "alFigure.h"
#include "alCore.h"

#define EPS 1e-15

#define DEFAULT_SIZE_X (740)
#define DEFAULT_SIZE_Y (520)
#define FIGURE_X (70)
#define FIGURE_Y (480)
#define FIGURE_HEIGHT (400)
#define FIGURE_WIDTH (600)


typedef struct
{
    alpoint2d begin;
    alpoint2d end;
} alline;

/*
 * A structure that stores an arrow from begin to end in world coordinates.
 * head1 and head2 are the coordinates of the arrow head
 */
typedef struct
{
    alpoint2d begin;
    alpoint2d end;
    alpoint2d head1;
    alpoint2d head2;
    bool valid;
} alarrow;


alpoint2d fig_to_world(alpoint2d fig_p, alfigure *fig);
alarrow aldraw_calculate_arrow(alpoint2d begin, alpoint2d end, double headlength, double angle);
alline* aldraw_get_ticks(alfigure *fig, char pos, bool subticks);




#endif
