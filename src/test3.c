#include "alFigure.h"
#include "alGraph.h"
#include "alCairo.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(void)
{

    const unsigned int N = 51;
    double datax[N];
    double datay[N];
    unsigned int i;
    double x;
    for (i = 0; i < N; i++) {
        x = -1.0 + 4*i/100.0;
        datax[i] = x;
        datay[i] = 0.5*sin(4*x);
    }
    //double datax[4];
    //double datay[4];
    //datax[0] = -0.8;
    //datax[1] = -0.5;
    //datax[2] = 0.2;
    //datax[3] = 0.8;
    //datay[0] = -0.4;
    //datay[1] = -0.1;
    //datay[2] = 0.4;
    //datay[3] = 0.1;

    //algraph *graph = algraph_create(data, 100);
    //algraph_calculate_bezier_control_points(graph);
    //algraph_print(graph);
    //algraph_list_init();
    //algraph_print(current_graph);
    algraph_create();
    algraph_set_data(current_graph, datax, datay, N);
    algraph_set_show_points(current_graph);
    algraph_set_interpolation_method(current_graph, 1);
    algraph_set_pointstyle(current_graph, 4);
    algraph_set_point_linewidth(current_graph, 1.0);
    algraph_set_pointsize(current_graph, 10.0);
    algraph_set_linecolor(current_graph, 0.2, 0.0, 0.9, 1.0);
    algraph_set_edgecolor(current_graph, 0.0, 0.0, 0.0, 1.0);
    algraph_set_points_filled(current_graph);
    algraph_set_facecolor(current_graph, 0.5, 0.5, 0.5, 0.5);
    //algraph_print(&(graph_list.root->graph));
    //algraph_list_print();
    //algraph_list_print();

    //algraph_list_clear();
    
    alfigure *fig = alfigure_create();
    alfigure_add_graph(fig, current_graph);
    alfigure_create_subxticks(fig, 5);
    alfigure_create_subyticks(fig, 5);

    algraph_create();
    for (i = 0; i < N; i++) {
        //x = -1.0 + 4*i/100.0;
        x = -1.0 + 4*i/100.0;
        datax[i] = x;
        datay[i] = 0.5*cos(4*x);
    }
    algraph_set_data(current_graph, datax, datay, N);
    alfigure_add_graph(fig, current_graph);
    


    ////fig->graph = graph;
    //alfigure_create_subxticks(fig, 20);
    //alfigure_create_subyticks(fig, 9);
    cairo_surface_t *cs;
    cs = alcairo_open_pdf("plot.pdf");
    alcairo_draw_figure(cs, fig);
    alcairo_write_pdf(cs);

    algraph_list_clear();

    return 0;
}
