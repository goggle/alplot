#ifndef ALGRAPH_H
#define ALGRAPH_H

#include <stdbool.h>
#include "color.h"
#include "alCore.h"


typedef struct
{
    unsigned int id;
    alpoint *data;
    unsigned int ndata;
    bool visible;
    bool show_graphline;
    double linewidth;
    rgba_color linecolor;
    int linestyle; /* 0: solid, 1: dashed */
    char *name;
    bool legend;
    bool show_points;

    /* 
     * point styles:
     * 0: o
     * 1: x
     * 2: +
     * 3: *
     * 4: square
     */
    int pointstyle;
    double pointsize;
    rgba_color pointcolor;
    int interpolation_method; /* 0: Cubic Bezier spline interpolation, 1: linear interpolation */
    alpoint *bezier_control_points;
} algraph;


struct algraph_node_
{
    algraph *graph;
    struct algraph_node_ *next;
};
typedef struct algraph_node_ algraph_node;


typedef struct
{
    unsigned int size;
    algraph_node *root;
} algraph_list;


extern algraph *current_graph;
extern algraph_list graph_list;


//algraph* algraph_create(double datax[], double datay[], unsigned int ndata);
void algraph_create();

void algraph_set_data(algraph *g, double datax[], double datay[], unsigned int n);
void algraph_set_show_graphline(algraph *g);
void algraph_unset_show_graphline(algraph *g);
void algraph_set_linewidth(algraph *g, double lw);
void algraph_set_linecolor(algraph *graph, double r, double g, double b, double alpha);
void algraph_set_name(algraph *g, const char *name);
void algraph_set_legend(algraph *g);
void algraph_unset_legend(algraph *g);
void algraph_set_show_points(algraph *g);
void algraph_unset_show_points(algraph *g);
void algraph_set_pointstyle(algraph *g, int style);
void algraph_set_pointsize(algraph *g, double size);
void algraph_set_pointcolor(algraph *graph, double r, double g, double b, double alpha);
void algraph_set_interpolation_method(algraph *g, int m);
unsigned int algraph_get_id(algraph *g);
void algraph_print(algraph *g);



void algraph_calculate_bezier_control_points(algraph *g);
double algraph_get_min_x(algraph *g);
double algraph_get_max_x(algraph *g);
double algraph_get_min_y(algraph *g);
double algraph_get_max_y(algraph *g);

//void algraph_list_init();
void algraph_list_add(algraph *graph);
//int algraph_list_remove();
void algraph_list_clear();
void algraph_list_print();
unsigned int algraph_get(unsigned int id);


#endif
