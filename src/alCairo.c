#include "alCairo.h"
#include "alCore.h"
#include "alGraph.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * TODO: The not drawing parts of these functions should be
 *       implemented in Figure.c
 */
static void draw_xticks(cairo_surface_t *cs, alfigure *fig, int mode);
static void draw_yticks(cairo_surface_t *cs, alfigure *fig, int mode);
static void draw_subxticks(cairo_surface_t *cs, alfigure *fig, int mode);
static void draw_subyticks(cairo_surface_t *cs, alfigure *fig, int mode);
static void draw_ticks(cairo_surface_t *cs, alfigure *fig, int mode);
static void draw_points(cairo_surface_t *cs, alfigure *fig);
static void draw_graph(cairo_surface_t *cs, alfigure *fig);


cairo_surface_t* open_pdf(const char *fname)
{
    cairo_surface_t *cs;
    cs = cairo_pdf_surface_create(fname, DEFAULT_SIZE_X, DEFAULT_SIZE_Y);
    return cs;
}


void draw_figure(cairo_surface_t *cs, alfigure *fig)
{
    /* Draw the figure window */
    cairo_t *c;
    c = cairo_create(cs);
    cairo_move_to(c, FIGURE_X, FIGURE_Y);
    cairo_line_to(c, FIGURE_X + FIGURE_WIDTH, FIGURE_Y);
    cairo_line_to(c, FIGURE_X + FIGURE_WIDTH, FIGURE_Y - FIGURE_HEIGHT);
    cairo_line_to(c, FIGURE_X, FIGURE_Y - FIGURE_HEIGHT);
    cairo_line_to(c, FIGURE_X, FIGURE_Y);
    cairo_set_line_width(c, 1.5);
    cairo_stroke(c);
    cairo_destroy(c);

    /* Draw the xticks and yticks */
    //draw_xticks(cs, fig, 2);
    //draw_yticks(cs, fig, 2);
    //draw_subxticks(cs, fig, 2);
    //draw_subyticks(cs, fig, 2);
    draw_ticks(cs, fig, 2);

    draw_points(cs, fig);
    draw_graph(cs, fig);
}


void write_pdf(cairo_surface_t *cs)
{
    cairo_surface_flush(cs);
    cairo_surface_destroy(cs);
}

static void draw_ticks(cairo_surface_t *cs, alfigure *fig, int mode)
{
    assert (mode == 1 || mode == 2);
    draw_xticks(cs, fig, 1);
    draw_yticks(cs, fig, 1);
    draw_subxticks(cs, fig, 1);
    draw_subyticks(cs, fig, 1);
    if (mode == 2) {
        draw_xticks(cs, fig, 2);
        draw_yticks(cs, fig, 2);
        draw_subxticks(cs, fig, 2);
        draw_subyticks(cs, fig, 2);
    }
}


/*
 * Draw the xticks.
 * mode:    1: only draw the xticks at the bottom
 *          2: draw the xticks at the top the figure
 */
static void draw_xticks(cairo_surface_t *cs, alfigure *fig, int mode)
{
    assert(mode == 1 || mode == 2);

    cairo_t *c;
    unsigned int i;
    alpoint p_begin;
    alpoint p_end;
    alpoint p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nxticks; i++) {
        p_fig.x = fig->xticks[i];
        if (mode == 1)
            p_fig.y = fig->ylim[0];
        else if (mode == 2)
            p_fig.y = fig->ylim[1];
        p_begin = p_end = fig_to_world(p_fig, fig);
        p_begin.y  += 5;
        p_end.y -= 5;
#ifdef DEBUG
        printf("draw_xticks: xtick beginning point, xtick ending point:\n");
        printf("(%.2f, %.2f), ", p_begin.x, p_begin.y);
        printf("(%.2f, %.2f)\n", p_end.x, p_end.y);
#endif
        cairo_move_to(c, p_begin.x, p_begin.y);
        cairo_line_to(c, p_end.x, p_end.y);
    }
    cairo_set_line_width(c, 1.5);
    cairo_stroke(c);
    cairo_destroy(c);
}

static void draw_subxticks(cairo_surface_t *cs, alfigure *fig, int mode)
{
    assert(mode == 1 || mode == 2);

    if (fig->nsubxticks == 0) {
#ifdef DEBUG
        printf("draw_subxticks: No subxticks specified.\n");
#endif
        return;
    }
    cairo_t *c;
    unsigned int i;
    alpoint p_begin;
    alpoint p_end;
    alpoint p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nsubxticks; i++) {
        p_fig.x = fig->subxticks[i];
        if (mode == 1)
            p_fig.y = fig->ylim[0];
        else if (mode == 2)
            p_fig.y = fig->ylim[1];
        p_begin = p_end = fig_to_world(p_fig, fig);
        p_begin.y += 3;
        p_end.y -= 3;
        cairo_move_to(c, p_begin.x, p_begin.y);
        cairo_line_to(c, p_end.x, p_end.y);
    }
    cairo_set_line_width(c, 0.5);
    cairo_stroke(c);
    cairo_destroy(c);
}


static void draw_yticks(cairo_surface_t *cs, alfigure *fig, int mode)
{
    assert (mode == 1 || mode == 2);

    cairo_t *c;
    unsigned int i;
    alpoint p_begin;
    alpoint p_end;
    alpoint p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nyticks; i++) {
        if (mode == 1)
            p_fig.x = fig->xlim[0];
        else if (mode == 2)
            p_fig.x = fig->xlim[1];
        p_fig.y = fig->yticks[i];
        p_begin = p_end = fig_to_world(p_fig, fig);
        p_begin.x += 5;
        p_end.x -= 5;
#ifdef DEBUG
        printf("draw_yticks: ytick beginning point, ytick ending point:\n");
        printf("(%.2f, %.2f), ", p_begin.x, p_begin.y);
        printf("(%.2f, %.2f)\n", p_end.x, p_end.y);
#endif
        cairo_move_to(c, p_begin.x, p_begin.y);
        cairo_line_to(c, p_end.x, p_end.y);
    }
    cairo_set_line_width(c, 1.5);
    cairo_stroke(c);
    cairo_destroy(c);
}

static void draw_subyticks(cairo_surface_t *cs, alfigure *fig, int mode)
{
    assert (mode == 1 || mode == 2);

    if (fig->nsubyticks == 0) {
#ifdef DEBUG
        printf("draw_subyticks: No subyticks specified.\n");
#endif
        return;
    }
    cairo_t *c;
    unsigned int i;
    alpoint p_begin; 
    alpoint p_end;
    alpoint p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nsubyticks; i++) {
        if (mode == 1)
            p_fig.x = fig->xlim[0];
        else if (mode == 2)
            p_fig.x = fig->xlim[1];
        p_fig.y = fig->subyticks[i];
        p_begin = p_end = fig_to_world(p_fig, fig);
        p_begin.x += 3;
        p_end.x -= 3;
        cairo_move_to(c, p_begin.x, p_begin.y);
        cairo_line_to(c, p_end.x, p_end.y);
    }
    cairo_set_line_width(c, 0.5);
    cairo_stroke(c);
    cairo_destroy(c);
}


static void draw_points(cairo_surface_t *cs, alfigure *fig)
{
    cairo_t *c;
    unsigned int i;
    alpoint pw;

    if (fig->graph == NULL) {
#ifdef DEBUG
        printf("draw_points: No graph specified.\n");
#endif
        return;
    }

    if (current_graph->visible && current_graph->show_points) {
        c = cairo_create(cs);
        for (i = 0; i < current_graph->ndata; i++) {
            pw = fig_to_world(current_graph->data[i], fig);
            //cairo_move_to(c, pw.x, pw.y);
            cairo_new_sub_path(c);
            cairo_arc(c, pw.x, pw.y, current_graph->pointsize/2.0, 0.0, 2*PI);
        }
        cairo_set_line_width(c,  0.5);
        cairo_set_source_rgb(c, current_graph->pointcolor.r, current_graph->pointcolor.g, current_graph->pointcolor.b);
        cairo_stroke(c);
        cairo_destroy(c);
    }
}


static void draw_graph(cairo_surface_t *cs, alfigure *fig)
{
    cairo_t *c;
    unsigned int i, j;
    alpoint p0, p1, p2, p3;

    if (fig->ngraph == 0) {
#ifdef DEBUG
        printf("draw_graph: No graph specified.\n");
#endif
        return;
    }
    for (j = 0; j < fig->ngraph; j++) {
        if (algraph_get(fig->graph_id[j])) {
            if (current_graph->visible) {
                c = cairo_create(cs);
                for (i = 0; i < current_graph->ndata - 1; i++) {
                    p0 = fig_to_world(current_graph->data[i], fig);
                    cairo_move_to(c, p0.x, p0.y);
                    p3 = fig_to_world(current_graph->data[i+1], fig);
                    p1 = fig_to_world(current_graph->bezier_control_points[2*i], fig);
                    p2 = fig_to_world(current_graph->bezier_control_points[2*i+1], fig);
                    cairo_curve_to(c, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
                    //printf("(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
                }
                cairo_set_line_width(c, current_graph->linewidth);
                cairo_stroke(c);
                cairo_destroy(c);
            }
        }
    }
}


//void paint(cairo_surface_t *cs)
//{
//    cairo_t *c;
//    c = cairo_create(cs);
//
//    //cairo_rectangle(c, 50.0, 50.0, 700, 480);
//    //cairo_set_source_rgb(c, 0.8, 0.8, 0.8);
//    //cairo_fill(c);
//
//    cairo_move_to(c, 50.0, 50.0);
//    cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
//    cairo_line_to(c, 750, 50.0);
//    cairo_line_to(c, 750, 300);
//    cairo_set_line_width(c, 2.0);
//    cairo_stroke(c);
//
//    cairo_move_to(c, 10.0, 10.0);
//    cairo_set_source_rgb(c, 1.0, 0.0, 0.0);
//    cairo_show_text(c, "Hello World!");
//
//    cairo_show_page(c);
//    cairo_destroy(c);
//}
