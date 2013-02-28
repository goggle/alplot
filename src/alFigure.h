#ifndef ALFIGURE_H
#define ALFIGURE_H

//#include "color.h"
#include <stdbool.h>
#include "alLine.h"
#include "alCore.h"
#include "alGraph.h"

#define DEFAULT_SIZE_X (740)
#define DEFAULT_SIZE_Y (520)
#define FIGURE_X (70)
#define FIGURE_Y (480)
#define FIGURE_HEIGHT (400)
#define FIGURE_WIDTH (600)


typedef struct
{
    unsigned int id;
    char *title;
    double xlim[2];
    double ylim[2];
    bool legend;

    /*
     * Where the ticks should appear:
     * 0: north
     * 1: east
     * 2: south
     * 3: west
     */
    bool show_ticks[4];
    double *xticks;
    unsigned int nxticks; /* number of xticks */
    double *yticks;
    unsigned int nyticks; /* number of yticks */

    double *subxticks;
    unsigned int nsubxticks;
    double *subyticks;
    unsigned int nsubyticks;

    unsigned int ngraph; /* number of graphs in this figure */
    unsigned int *graph_id; /* contains the ids of the graphs */

} alfigure;



alpoint2d fig_to_world(alpoint2d fig_p, alfigure *fig);

alfigure* alfigure_create();
void alfigure_print(alfigure *fig);
void alfigure_add_graph(alfigure *fig, algraph *graph);
double alfigure_get_height(alfigure *fig);
double alfigure_get_width(alfigure *fig);

void alfigure_set_show_ticks(alfigure *fig, bool n, bool e, bool s, bool w);
void alfigure_create_subxticks(alfigure *fig, unsigned int n);
void alfigure_create_subyticks(alfigure *fig, unsigned int n);

void alfigure_print_subxticks(alfigure *fig);
void alfigure_print_subyticks(alfigure *fig);

void set_xlim(alfigure *fig, double xmin, double xmax);
void set_ylim(alfigure *fig, double ymin, double ymax);


#endif
