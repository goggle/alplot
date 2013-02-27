#include "alGraph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


algraph_list graph_list = {0, NULL};
algraph *current_graph = NULL;

static int algraph_list_remove();

/*
 * Create a new graph and add it to the list.
 *
 * Note: The input arrays are copied.
 */
//algraph* algraph_create(double datax[], double datay[], unsigned int ndata)
//{
//    unsigned int i;
//    static int id = 1;
//    algraph *g = (algraph *) malloc(sizeof(algraph));
//    if (g == NULL)
//        alerror("algraph_create: Could not allocate memory!");
//    g->data = (alpoint *) malloc(ndata * sizeof(alpoint *));
//    if (g->data == NULL)
//        alerror("algraph_create: Could not allocate memory!");
//
//    for (i = 0; i < ndata; i++) {
//        g->data[i].x = datax[i];
//        g->data[i].y = datay[i];
//    }
//
//    g->id = id;
//    id++;
//    g->ndata = ndata;
//    g->visible = true;
//    g->linewidth = 1.0;
//    g->linecolor.r = 0.0;
//    g->linecolor.g = 0.0;
//    g->linecolor.b = 0.0;
//    g->linecolor.alpha = 1.0;
//    g->name = NULL;
//    g->legend = false;
//    g->show_points = false;
//    g->pointstyle = 0;
//    g->pointsize = 7.0;
//    g->pointcolor.r = 0.0;
//    g->pointcolor.g = 0.0;
//    g->pointcolor.b = 1.0;
//    g->pointcolor.alpha = 1.0;
//    g->bezier_control_points = NULL;
//    g->interpolation_method = 0;
//    return g;
//}

/*
 * Create a new graph and add it to the list of graphs
 */
void algraph_create()
{
    static unsigned int id = 1;
    algraph *g = (algraph *) malloc(sizeof(algraph));
    if (g == NULL)
        alerror("algraph_create: Could not allocate memory!");
    g->id = id;
    id++;
    g->data = NULL;
    g->ndata = 0;
    g->show_graphline = true;
    g->linewidth = 1.0;
    g->linecolor.r = g->linecolor.g = g->linecolor.b = 0.0;
    g->linecolor.alpha = 1.0;
    g->name = NULL;
    g->legend = false;
    g->show_points = false;
    g->pointstyle = 0;
    g->pointsize = 5.0;
    g->pointcolor.r = g->pointcolor.g = g->pointcolor.b = 0.0;
    g->pointcolor.alpha = 1.0;
    g->bezier_control_points = NULL;
    g->interpolation_method = 0;
    algraph_list_add(g);
}


/*
 * Equip a graph with its data points and calculate the Bezier spline control points:
 * The input data will be copied.
 */
void algraph_set_data(algraph *g, double datax[], double datay[], unsigned int n)
{
    unsigned int i;
    if (g == NULL)
        aluierror("algraph_set_data: Graph not initialised.");
    g->data = (alpoint *) malloc(n * sizeof(alpoint));
    if (g->data == NULL)
        alerror("algraph_set_data: Could not allocate memory!");
    for (i = 0; i < n; i++) {
        g->data[i].x = datax[i];
        g->data[i].y = datay[i];
    }
    g->ndata = n;

    /* Calculate the Bezier spline control points and store them. */
    algraph_calculate_bezier_control_points(g);
}



void algraph_print(algraph *g)
{
    unsigned int i;
    unsigned int n_bezier;
    if (g == NULL)
        return;
    printf("Graph %d\n", g->id);
    if (g->name != NULL)
        printf("Name: %s\n", g->name);
    else
        printf("Name:\n");
    printf("Number of data points: %d\n", g->ndata);
    printf("Data points: \n");
    if (g->ndata != 0) {
        for (i = 0; i < g->ndata - 1; i++) {
            printf("(%.3f, %.3f), ", g->data[i].x, g->data[i].y);
        }
        printf("(%.3f, %.3f)\n", g->data[g->ndata - 1].x, g->data[g->ndata - 1].y);
    }
    printf("Show graph line: %d\n", g->show_graphline);
    printf("Line width: %.2f\n", g->linewidth);
    printf("Line color: (%.2f, %.2f, %.2f)\n", g->linecolor.r, g->linecolor.g, g->linecolor.b);
    printf("Line alpha value: %.2f\n", g->linecolor.alpha);
    printf("Legend: %d\n", g->legend);
    printf("Show points: %d\n", g->show_points);
    printf("Point style: %d\n", g->pointstyle);
    printf("Point size: %.1f\n", g->pointsize);
    printf("Point color: (%.2f, %.2f, %.2f)\n", g->pointcolor.r, g->pointcolor.g, g->pointcolor.b);
    printf("Point alpha value: %.2f\n", g->pointcolor.alpha);
    printf("Interpolation method: %d\n", g->interpolation_method);
    printf("Bezier spline control points:\n");
    if (g->bezier_control_points != NULL) {
        n_bezier = 2 * (g->ndata - 1);
        for (i = 0; i < n_bezier - 1; i++) {
            printf("(%.3f, %.3f), ", g->bezier_control_points[i].x, g->bezier_control_points[i].y);
        }
        printf("( %.3f, %.3f)\n", g->bezier_control_points[n_bezier-1].x, g->bezier_control_points[n_bezier-1].y);
    }
    printf("\n");
}

void algraph_set_show_graphline(algraph *g)
{
    g->show_graphline = true;
}


void algraph_unset_show_graphline(algraph *g)
{
    g->show_graphline = false;
}

void algraph_set_linewidth(algraph *g, double lw)
{
    if (lw < 0)
        aluierror("algraph_set_linewidth: Line width must be positive.");
    g->linewidth = lw;
}

void algraph_set_linecolor(algraph *graph, double r, double g, double b, double alpha)
{
    graph->linecolor.r = r;
    graph->linecolor.g = g;
    graph->linecolor.b = b;
    graph->linecolor.alpha = alpha;
}


void algraph_set_name(algraph *g, const char *name)
{
    int len = strlen(name);
    g->name = (char *) malloc((len + 1) * sizeof(char));
    strcpy(g->name, name);
}


void algraph_set_legend(algraph *g)
{
    g->legend = true;
}

void algraph_unset_legend(algraph *g)
{
    g->legend = false;
}

void algraph_set_show_points(algraph *g)
{
    g->show_points = true;
}

void algraph_unset_show_points(algraph *g)
{
    g->show_points = false;
}

void algraph_set_pointstyle(algraph *g, int style)
{
    if (style != 0 && style != 1)
        aluierror("algraph_set_pointstyle: Point style must be 0 or 1.");
    g->pointstyle = style;
}

void algraph_set_pointsize(algraph *g, double size)
{
    g->pointsize = size;
}


void algraph_set_pointcolor(algraph *graph, double r, double g, double b, double alpha)
{
    graph->pointcolor.r = r;
    graph->pointcolor.g = g;
    graph->pointcolor.b = b;
    graph->pointcolor.alpha = alpha;
}


void algraph_set_interpolation_method(algraph *g, int m)
{
    if (m != 0 && m!= 1)
        aluierror("algraph_set_interpolation_method: Interpolation method must be 0 or 1.");
    g->interpolation_method = m;
}


unsigned int algraph_get_id(algraph *g)
{
    return g->id;
}


void algraph_calculate_bezier_control_points(algraph *g)
{
    unsigned int i;
    int j;
    double *mdiag;
    double *supdiag;
    double *subdiag;
    double *vx; /* rhs for x coordinates */
    double *vy; /* rhs for y coordinates */
    double *cprime;  /* used in Thomas Algorithm */
    double *dprimex; /* used in Thomas Algorithm */
    double *dprimey; /* used in Thomas Algorithm */
    double tmp;      /* used in Thomas Algoritm */
    unsigned int n_control_points = 2 * (g->ndata - 1);

    /* Maybe not needed: */
    if (g->ndata == 0)
        alerror("algraph_calculate_bezier_control_points: No data specified!");

    g->bezier_control_points = (alpoint*) malloc(n_control_points * sizeof(alpoint));
    mdiag = (double *) malloc((g->ndata - 1) * sizeof(double));
    supdiag = (double *) malloc((g->ndata - 1) * sizeof(double));
    subdiag = (double *) malloc((g->ndata - 1) * sizeof(double));
    vx = (double *) malloc((g->ndata - 1) * sizeof(double));
    vy = (double *) malloc((g->ndata - 1) * sizeof(double));

    if (g-> bezier_control_points == NULL ||
            mdiag == NULL ||
            supdiag == NULL ||
            subdiag == NULL ||
            vx == NULL ||
            vy == NULL)
        alerror("algraph_calculate_bezier_control_points: Could not allocate memory!");

    /* Initialize the matrix A */
    mdiag[0] = 2.0;
    mdiag[g->ndata - 2] = 7.0;
    supdiag[0] = 1.0;
    supdiag[g->ndata - 2] = 0.0;
    subdiag[0] = 0.0;
    subdiag[g->ndata - 2] = 2.0;
    for (i = 1; i < g->ndata - 2; i++) {
        mdiag[i] = 4.0;
        supdiag[i] = subdiag[i] = 1.0;
    }


    /* Initialize the rhs */
    vx[0] = g->data[0].x + 2 * g->data[1].x;
    vy[0] = g->data[0].y + 2 * g->data[1].y;
    vx[g->ndata - 2] = 8 * g->data[g->ndata - 2].x + g->data[g->ndata - 1].x;
    vy[g->ndata - 2] = 8 * g->data[g->ndata - 2].y + g->data[g->ndata - 1].y;

    for (i = 1; i < g->ndata - 2; i++) {
        vx[i] = 4 * g->data[i].x + 2 * g->data[i+1].x;
        vy[i] = 4 * g->data[i].y + 2 * g->data[i+1].y;
    }


    /*
     * Use Thomas Algorithm to solve the tridiagonal linear system: 
     * http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm 
     */
    cprime = (double *) malloc((g->ndata - 1) * sizeof(double));
    dprimex = (double *) malloc((g->ndata - 1) * sizeof(double));
    dprimey = (double *) malloc((g->ndata - 1) * sizeof(double));
    if (cprime == NULL || dprimex == NULL || dprimey == NULL)
        alerror("algraph_calculate_bezier_control_points: Could not allocate memory!");
    cprime[0] = supdiag[0] / mdiag[0];
    dprimex[0] = vx[0] / mdiag[0];
    dprimey[0] = vy[0] / mdiag[0];
    for (i = 1; i < g->ndata - 1; i++) {
        tmp = 1.0 / (mdiag[i] - cprime[i-1] * subdiag[i]);
        cprime[i] = supdiag[i] * tmp;
        dprimex[i] = (vx[i] - dprimex[i-1] * subdiag[i]) * tmp;
        dprimey[i] = (vy[i] - dprimey[i-1] * subdiag[i]) * tmp;
    }
    for (j = g->ndata - 2; j >= 0; j--) {
        dprimex[j] -= cprime[j] * dprimex[j+1]; 
        dprimey[j] -= cprime[j] * dprimey[j+1]; 
    }


    /* 
     * Now we have the coordinates of the first bezier spline control points stored in the
     * arrays dprimex and primey. 
     * Copy them into g->bezier_spline_control_points:
     */
    j = 0;
    for (i = 0; i < g->ndata - 1; i++) {
        g->bezier_control_points[j].x = dprimex[i];
        g->bezier_control_points[j].y = dprimey[i];
        j += 2;
    }

    /* Calculate the second bezier spline control points */
    j = 1;
    for (i = 0; i < g->ndata - 2; i++) {
        g->bezier_control_points[j].x = 2 * g->data[i+1].x - dprimex[i+1]; 
        g->bezier_control_points[j].y = 2 * g->data[i+1].y - dprimey[i+1];
        j += 2;
    }
    g->bezier_control_points[n_control_points - 1].x = 0.5 * (g->data[g->ndata - 1].x + dprimex[g->ndata - 2]);
    g->bezier_control_points[n_control_points - 1].y = 0.5 * (g->data[g->ndata - 1].y + dprimey[g->ndata - 2]);

//#ifdef DEBUG
//    printf("algraph_bezier_control_points: Bezier Spline segments:\n");
//    for (i = 0; i < g->ndata - 1; i++) {
//        printf("(%.3f, %.3f), ", g->data[i].x, g->data[i].y);
//        printf("(%.3f, %.3f), ", g->bezier_control_points[2*i].x, g->bezier_control_points[2*i].y);
//        printf("(%.3f, %.3f), ", g->bezier_control_points[2*i+1].x, g->bezier_control_points[2*i+1].y);
//        printf("(%.3f, %.3f)\n", g->data[i+1].x, g->data[i+1].y);
//    }
//#endif

    free(cprime);
    free(dprimex);
    free(dprimey);
    free(mdiag);
    free(supdiag);
    free(subdiag);
    free(vx);
    free(vy);

}

double algraph_get_min_x(algraph *g)
{
    unsigned int i;
    double min = g->data[0].x;
    for (i = 1; i < g->ndata; i++) {
        if (g->data[i].x < min)
            min = g->data[i].x;
    }
    return min;
}

    
double algraph_get_max_x(algraph *g)
{
    unsigned int i;
    double max = g->data[0].x;
    for (i = 1; i < g->ndata; i++) {
        if (g->data[i].x > max)
            max = g->data[i].x;
    }
    return max;
}


double algraph_get_min_y(algraph *g)
{
    unsigned int i;
    double min = g->data[0].y;
    for (i = 1; i < g->ndata; i++) {
        if (g->data[i].y < min)
            min = g->data[i].y;
    }
    return min;
}


double algraph_get_max_y(algraph *g)
{
    unsigned int i;
    double max = g->data[0].y;
    for (i = 1; i < g->ndata; i++) {
        if (g->data[i].y > max)
            max = g->data[i].y;
    }
    return max;
}

///*
// * Initialize the list of graph objects and set the current graph to NULL
// *
// * FIXME: This function is not needed!
// */
//void algraph_list_init()
//{
//#ifdef DEBUG
//    printf("Entering algraph_list_init.\n");
//#endif
//    graph_list.size = 0;
//    graph_list.root = NULL;
//    current_graph = NULL;
//#ifdef DEBUG
//    printf("Exit algraph_list_init.\n");
//#endif
//}


/*
 * Add a new graph node to the list and set the current graph to the new graph.
 */
void algraph_list_add(algraph *graph)
{
    unsigned int i;
    algraph_node *cur;
    algraph_node *new_node;

    current_graph = graph;

#ifdef DEBUG
    printf("Entering algraph_list_add.\n");
#endif

    new_node = (algraph_node *) malloc(sizeof(algraph_node));
    new_node->graph = graph;
    new_node->next = NULL;

    if (new_node == NULL)
        alerror("algraph_list_add: Could not allocate memory!\n");

    if (graph_list.size == 0) {
        graph_list.size = 1;
        graph_list.root = new_node;
        //current_graph = &(graph_list.root->graph); /* set the current graph */
#ifdef DEBUG
        printf("Exit algraph_list_add (first element added).\n");
#endif
        return;
    }

    cur = graph_list.root;
    /* Iterate to the last element: */
    for (i = 0; i < graph_list.size - 1; i++) {
        cur = cur->next;
    }
    cur->next = new_node;
    //current_graph = &(cur->next->graph); /* set the current graph */
    graph_list.size++;
#ifdef DEBUG
    printf("Exit algraph_list_add.\n");
#endif
}


/*
 * Remove the last graph node and set the current graph to the previous one 
 * 
 * Return 0 if the list was empty, 1 otherwise.
 */
static int algraph_list_remove()
{
#ifdef DEBUG
    printf("Entering algraph_list_remove.\n");
#endif
    unsigned int i;
    algraph_node *cur;
    algraph_node *prev;
    
    /* if the list is empty, we just ensure assure that the current graph is set to NULL */
    if (graph_list.size == 0) {
        current_graph = NULL;
#ifdef DEBUG
        printf("Exit algraph_list_remove with return value 0.\n");
#endif
        return 0;
    }
    cur = graph_list.root;

    /* if the list has exactly one element, */
    if (graph_list.size == 1) {
        prev = NULL;
        current_graph = NULL;
    }
    else {
        for (i = 0; i < graph_list.size - 1; i++) {
            prev = cur;
            cur = cur->next;
        }
        current_graph = prev->graph;
    }
    
    if (cur->graph != NULL) {
        if (cur->graph->name != NULL)
            free(cur->graph->name);
        if (cur->graph->bezier_control_points != NULL)
            free(cur->graph->bezier_control_points);
        if (cur->graph->data != NULL)
            free(cur->graph->data);
        free(cur->graph);
    }
    graph_list.size--;
    free(cur);
#ifdef DEBUG
    printf("Exit algraph_list_remove with return value 1.\n");
#endif
    return 1;
}

/*
 * Remove and free all the entries in the list of graph objects.
 */
void algraph_list_clear()
{
#ifdef DEBUG
    printf("Entering algraph_list_clear.\n");
#endif
    int ret;
    do {
        ret = algraph_list_remove();
    } while(ret);
    current_graph = NULL;
#ifdef DEBUG
    printf("Exit algraph_list_clear.\n");
#endif
}


/*
 * Print out all the graphs in the list.
 */
void algraph_list_print()
{
#ifdef DEBUG
    printf("Entering algraph_list_print.\n");
#endif
    unsigned int i;
    algraph_node *node = graph_list.root;
    for (i = 0; i < graph_list.size; i++) {
        algraph_print(node->graph);
        node = node->next;
    }
#ifdef DEBUG
    printf("Exit algraph_list_print.\n");
#endif
}
    

/*
 * Set the current_graph pointer to the graph with the given input id.
 * If the graph does not exist, let the current graph unchanged and return 0
 */
unsigned int algraph_get(unsigned int id)
{
    unsigned int i;
    algraph_node *node;

    if (graph_list.size == 0) {
        current_graph = NULL;
        return 0;
    }
    node = graph_list.root;
    for (i = 0; i < graph_list.size; i++) {
        if (node->graph->id == id) {
            current_graph = node->graph;
            return id;
        }
        node = node->next;
    }
    return 0;
}


