#include <math.h>
#include "alDraw.h"

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
