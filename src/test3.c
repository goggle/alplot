#include "alFigure.h"
#include "alGraph.h"
#include "alLine.h"
#include "alCairo.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(void)
{

    double datax[100];
    double datay[100];
    int i;
    double x;
    for (i = 0; i < 100; i++) {
        x = -1.0 + 2*i/100.0;
        datax[i] = x;
        datay[i] = 0.5*sin(4*x);
    }

    //algraph *graph = algraph_create(data, 100);
    //algraph_calculate_bezier_control_points(graph);
    //algraph_print(graph);
    //algraph_list_init();
    //algraph_print(current_graph);
    algraph_create();
    algraph_set_data(current_graph, datax, datay, 100);
    algraph_set_show_points(current_graph);
    //algraph_print(&(graph_list.root->graph));
    //algraph_list_print();
    //algraph_list_print();

    //algraph_list_clear();
    

    alfigure *fig = alfigure_create();
    alfigure_add_graph(fig, current_graph);




    ////fig->graph = graph;
    //alfigure_create_subxticks(fig, 20);
    //alfigure_create_subyticks(fig, 9);
    cairo_surface_t *cs;
    cs = open_pdf("plot.pdf");
    draw_figure(cs, fig);
    write_pdf(cs);

    algraph_list_clear();
    return 0;
}
