#include "alFigure.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


alfigure* alfigure_create()
{
    static int id = 1;
    alfigure *fig = (alfigure *) malloc(sizeof(alfigure)); // TODO: Free memory!
    if (fig == NULL)
        alerror("alfigure_create(): Could not allocate memory.");

    fig->id = id;
    id++;
    fig->title = NULL;

    fig->xlim[0] = -1.0;
    fig->xlim[1] = 1.0;
    fig->ylim[0] = -0.5;
    fig->ylim[1] = 0.5;

    fig->show_ticks[0] = true;
    fig->show_ticks[1] = true;
    fig->show_ticks[2] = true;
    fig->show_ticks[3] = true;

    /* TODO: Free memory! */
    fig->xticks = (double *) malloc(3 * sizeof(int));
    fig->yticks = (double *) malloc(3 * sizeof(int));

    fig->xticks[0] = -1.0;
    fig->xticks[1] = 0.0;
    fig->xticks[2] = 1.0;
    fig->yticks[0] = -0.5;
    fig->yticks[1] = 0.0;
    fig->yticks[2] = 0.5;

    fig->nxticks = 3;
    fig->nyticks = 3;

    
    fig->subxticks = NULL;
    fig->nsubxticks = 0;
    fig->subyticks = NULL;
    fig->nsubyticks = 0;

    fig->show_xaxis = false;
    fig->show_yaxis = false;

    fig->ngraph = 0;
    fig->graph_id = NULL;
    
    return fig;
}

/*
 * Add the graph id of the given graph to the figure
 */
void alfigure_add_graph(alfigure *fig, algraph *graph)
{
    unsigned int id;
#ifdef DEBUG
    printf("Enterig alfigure_add_graph.\n");
#endif
    if (fig == NULL)
        alerror("alfigure_add_graph: fig not allocated!");
    if (graph == NULL)
        alerror("alfigure_add_graph: graph not allocated!");

    id = algraph_get_id(graph);
    fig->graph_id = (unsigned int *) realloc(fig->graph_id, (fig->ngraph + 1) * sizeof(unsigned int));
    if (fig->graph_id == NULL)
        alerror("alfigure_add_graph: Could not allocate memory!");
    fig->graph_id[fig->ngraph] = id;
    fig->ngraph++;
#ifdef DEBUG
    printf("Exit alfigure_add_graph.\n");
#endif
}


void alfigure_print(alfigure *fig)
{
    unsigned int i;
    printf("Figure %d\n", fig->id);
    printf("Title: %s\n", fig->title);
    printf("x limits: %.2f, %.2f\n", fig->xlim[0], fig->xlim[1]);
    printf("y limits: %.2f, %.2f\n", fig->ylim[0], fig->ylim[1]);
    printf("Legend: %d\n", fig->legend);
    printf("xticks: ");
    for (i = 0; i < fig->nxticks; i++)
        printf("%.2f ", fig->xticks[i]);
    printf("\nyticks: ");
    for (i = 0; i < fig->nyticks; i++)
        printf("%.2f ", fig->xticks[i]);
    printf("\nsubxticks: ");
    for (i = 0; i < fig->nsubxticks; i++)
        printf("%.2f ", fig->subxticks[i]);
    printf("\nsubyticks: ");
    for (i = 0; i < fig->nsubyticks; i++)
        printf("%.2f ", fig->subyticks[i]);
    printf("\nGraph ids: ");
    for (i = 0; i < fig->ngraph; i++)
        printf("%d ", fig->graph_id[i]);
    printf("\n");
}



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


double alfigure_get_height(alfigure *fig)
{
    return fig->ylim[1] - fig->ylim[0];
}

double alfigure_get_width(alfigure *fig)
{
    return fig->xlim[1] - fig->xlim[0];
}


/*
 * Create n equally distributed subxticks between two existing xticks and store them
 * in the struct figure array subxticks.
 */
void alfigure_create_subxticks(alfigure *fig, unsigned int n)
{
    unsigned int i, j;
    int c = 0;
    double step;
    double x;
    fig->subxticks = (double *) malloc((fig->nxticks - 1) * n * sizeof(double)); /* TODO: Free memory */
    if (fig->subxticks == NULL)
        alerror("create_subxticks: Could not allocate memory!");
    for (i = 0; i < fig->nxticks - 1; i++) {
        x = fig->xticks[i];
        step = (fig->xticks[i+1] - fig->xticks[i]) / (n + 1);
        for (j = 0; j < n; j++) {
            x += step;
            fig->subxticks[c++] = x;
        }
    }
    fig->nsubxticks = c;
}


/*
 * Same as alfigure_create_subxticks but on the y axis.
 */
void alfigure_create_subyticks(alfigure *fig, unsigned int n)
{
    unsigned int i, j;
    int c = 0;
    double step;
    double y;
    fig->subyticks = (double *) malloc((fig->nyticks - 1) * n * sizeof(double)); /* TODO: Free memory */
    if (fig->subyticks == NULL)
        alerror("create subyticks: Could not allocate memory!");
    for (i = 0; i < fig->nyticks - 1; i++) {
        y = fig->yticks[i];
        step = (fig->yticks[i+1] - fig->yticks[i]) / (n + 1);
        for (j = 0; j < n; j++) {
            y += step;
            fig->subyticks[c++] = y;
        }
    }
    fig->nsubyticks = c;
}

void alfigure_print_subxticks(alfigure *fig)
{
    unsigned int i;
    printf("subxticks:\n");
    for (i = 0; i < fig->nsubxticks; i++)
        printf("%.5f ", fig->subxticks[i]);
    printf("\n");
}

void alfigure_print_subyticks(alfigure *fig)
{
    unsigned int i;
    printf("subyticks:\n");
    for (i = 0; i < fig->nsubyticks; i++)
        printf("%.5f ", fig->subyticks[i]);
    printf("\n");
}

void alfigure_set_show_ticks(alfigure *fig, bool north, bool east, bool south, bool west)
{
    fig->show_ticks[0] = north;
    fig->show_ticks[1] = east;
    fig->show_ticks[2] = south;
    fig->show_ticks[3] = west;
}


void alfigure_set_show_xaxis(alfigure *fig)
{
    fig->show_xaxis = true;
}

void alfigure_unset_show_xaxis(alfigure *fig)
{
    fig->show_xaxis = false;
}

void alfigure_set_show_yaxis(alfigure *fig)
{
    fig->show_yaxis = true;
}

void alfigure_unset_show_yaxis(alfigure *fig)
{
    fig->show_yaxis = false;
}

void set_xlim(alfigure *fig, double xmin, double xmax)
{
    assert(xmin < xmax);

    fig->xlim[0] = xmin;
    fig->xlim[1] = xmax;
}

void set_ylim(alfigure *fig, double ymin, double ymax)
{
    assert(ymin < ymax);
    
    fig->ylim[0] = ymin;
    fig->ylim[1] = ymax;
}

