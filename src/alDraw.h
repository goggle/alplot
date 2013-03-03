#ifndef ALDRAW_H
#define ALDRAW_H

/*
 * All the calculations used for drawing should should be done here.
 */
#include "alFigure.h"
#include "alCore.h"

#define EPS 1e-15

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


alarrow aldraw_calculate_arrow(alpoint2d begin, alpoint2d end, double headlength, double angle);




#endif
