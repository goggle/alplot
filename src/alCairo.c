#include "alCairo.h"
#include "alCore.h"
#include "alGraph.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * TODO: The not drawing parts of these functions should be
 *       implemented in Figure.c
 */
static void draw_xticks(cairo_surface_t *cs, alfigure *fig, bool loc);
static void draw_yticks(cairo_surface_t *cs, alfigure *fig, bool loc);
static void draw_subxticks(cairo_surface_t *cs, alfigure *fig, bool loc);
static void draw_subyticks(cairo_surface_t *cs, alfigure *fig, bool loc);
static void draw_ticks(cairo_surface_t *cs, alfigure *fig, bool n, bool e, bool s, bool w);
static void draw_points(cairo_surface_t *cs, alfigure *fig);
static void draw_point_o(cairo_t *cr, alpoint2d pw);
static void draw_point_x(cairo_t *cr, alpoint2d pw);
static void draw_point_plus(cairo_t *cr, alpoint2d pw);
static void draw_point_star(cairo_t *cr, alpoint2d pw);
static void draw_point_square(cairo_t *cr, alpoint2d pw);
static void draw_point_square45(cairo_t *cr, alpoint2d pw);
static void draw_graph(cairo_surface_t *cs, alfigure *fig);
static void clip_graph(cairo_t *cr);
static bool point_in_figure(alpoint2d pw);


cairo_surface_t* alcairo_open_pdf(const char *fname)
{
    cairo_surface_t *cs;
    cs = cairo_pdf_surface_create(fname, DEFAULT_SIZE_X, DEFAULT_SIZE_Y);
    return cs;
}


void alcairo_draw_figure(cairo_surface_t *cs, alfigure *fig)
{
    /* Draw the figure window */
    cairo_t *c;
    c = cairo_create(cs);
    cairo_move_to(c, FIGURE_X, FIGURE_Y);
    //cairo_line_to(c, FIGURE_X + FIGURE_WIDTH, FIGURE_Y);
    //cairo_line_to(c, FIGURE_X + FIGURE_WIDTH, FIGURE_Y - FIGURE_HEIGHT);
    //cairo_line_to(c, FIGURE_X, FIGURE_Y - FIGURE_HEIGHT);
    //cairo_line_to(c, FIGURE_X, FIGURE_Y);
    cairo_rectangle(c, FIGURE_X, FIGURE_Y, FIGURE_WIDTH, -FIGURE_HEIGHT);
    cairo_set_line_width(c, 1.5);
    cairo_stroke(c);
    cairo_destroy(c);

    draw_ticks(cs, fig, fig->show_ticks[0], fig->show_ticks[1], fig->show_ticks[2], fig->show_ticks[3]);

    draw_graph(cs, fig);
    draw_points(cs, fig);
}


void alcairo_write_pdf(cairo_surface_t *cs)
{
    cairo_surface_flush(cs);
    cairo_surface_destroy(cs);
}

static void draw_ticks(cairo_surface_t *cs, alfigure *fig, bool north, bool east, bool south, bool west)
{
    if (north) {
        draw_xticks(cs, fig, false);
        draw_subxticks(cs, fig, false);
    }
    if (east) {
        draw_yticks(cs, fig, false);
        draw_subyticks(cs, fig, false);
    }
    if (south) {
        draw_xticks(cs, fig, true);
        draw_subxticks(cs, fig, true);
    }
    if (west) {
        draw_yticks(cs, fig, true);
        draw_subyticks(cs, fig, true);
    }
}


/*
 * Draw the xticks.
 * loc:     false: north
 *          true:  south
 */
static void draw_xticks(cairo_surface_t *cs, alfigure *fig, bool loc)
{
    cairo_t *c;
    unsigned int i;
    alpoint2d p_begin;
    alpoint2d p_end;
    alpoint2d p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nxticks; i++) {
        p_fig.x = fig->xticks[i];
        if (loc)
            p_fig.y = fig->ylim[0];
        else
            p_fig.y = fig->ylim[1];
        p_begin = p_end = fig_to_world(p_fig, fig);
        p_begin.y  += 5;
        p_end.y -= 5;
#ifdef CAIRO_DEBUG
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

static void draw_subxticks(cairo_surface_t *cs, alfigure *fig, bool loc)
{
    if (fig->nsubxticks == 0) {
#ifdef CAIRO_DEBUG
        printf("draw_subxticks: No subxticks specified.\n");
#endif
        return;
    }
    cairo_t *c;
    unsigned int i;
    alpoint2d p_begin;
    alpoint2d p_end;
    alpoint2d p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nsubxticks; i++) {
        p_fig.x = fig->subxticks[i];
        if (loc)
            p_fig.y = fig->ylim[0];
        else
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


/*
 * Draw the xticks.
 * loc:     false: east
 *          true:  west
 */
static void draw_yticks(cairo_surface_t *cs, alfigure *fig, bool loc)
{
    cairo_t *c;
    unsigned int i;
    alpoint2d p_begin;
    alpoint2d p_end;
    alpoint2d p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nyticks; i++) {
        if (loc)
            p_fig.x = fig->xlim[0];
        else
            p_fig.x = fig->xlim[1];
        p_fig.y = fig->yticks[i];
        p_begin = p_end = fig_to_world(p_fig, fig);
        p_begin.x += 5;
        p_end.x -= 5;
#ifdef CAIRO_DEBUG
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

static void draw_subyticks(cairo_surface_t *cs, alfigure *fig, bool loc)
{
    if (fig->nsubyticks == 0) {
#ifdef CAIRO_DEBUG
        printf("draw_subyticks: No subyticks specified.\n");
#endif
        return;
    }
    cairo_t *c;
    unsigned int i;
    alpoint2d p_begin; 
    alpoint2d p_end;
    alpoint2d p_fig;

    c = cairo_create(cs);
    for (i = 0; i < fig->nsubyticks; i++) {
        if (loc)
            p_fig.x = fig->xlim[0];
        else
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
    unsigned int i, j;
    alpoint2d pw;

    if (fig->ngraph == 0) {
#ifdef CAIRO_DEBUG
        printf("draw_points: No graph specified.\n");
#endif
        return;
    }

    for (j = 0; j < fig->ngraph; j++) {
        if (algraph_get(fig->graph_id[j])) {
            if (current_graph->points.show) {
                c = cairo_create(cs);
                //for (i = 0; i < current_graph->ndata; i++) {
                //    pw = fig_to_world(current_graph->data[i], fig);
                //    if (current_graph->points.style == 0)
                //        draw_point_o(c, pw);
                //    else if (current_graph->points.style == 1)
                //        draw_point_x(c, pw);
                //    else if (current_graph->points.style == 2)
                //        draw_point_plus(c, pw);
                //    else if (current_graph->points.style == 3)
                //        draw_point_star(c, pw);
                //    else if (current_graph->points.style == 4)
                //        draw_point_square(c, pw);
                //    else if (current_graph->points.style == 5)
                //        draw_point_square45(c, pw);
                //}
                switch (current_graph->points.style) {
                    case 0:
                        for (i = 0; i < current_graph->ndata; i++) {
                            pw = fig_to_world(current_graph->data[i], fig);
                            if (point_in_figure(pw))
                                draw_point_o(c, pw);
                        }
                        break;
                    case 1:
                        for (i = 0; i < current_graph->ndata; i++) {
                            pw = fig_to_world(current_graph->data[i], fig);
                            if (point_in_figure(pw))
                                draw_point_x(c, pw);
                        }
                        break;
                    case 2:
                        for (i = 0; i < current_graph->ndata; i++) {
                            pw = fig_to_world(current_graph->data[i], fig);
                            if (point_in_figure(pw))
                                draw_point_plus(c, pw);
                        }
                        break;
                    case 3:
                        for (i = 0; i < current_graph->ndata; i++) {
                            pw = fig_to_world(current_graph->data[i], fig);
                            if (point_in_figure(pw))
                                draw_point_star(c, pw);
                        }
                        break;
                    case 4:
                        for (i = 0; i < current_graph->ndata; i++) {
                            pw = fig_to_world(current_graph->data[i], fig);
                            if (point_in_figure(pw))
                                draw_point_square(c, pw);
                        }
                        break;
                    case 5:
                        for (i = 0; i < current_graph->ndata; i++) {
                            pw = fig_to_world(current_graph->data[i], fig);
                            if (point_in_figure(pw))
                                draw_point_square45(c, pw);
                        }
                        break;
                    default: break;
                }
                //cairo_set_source_rgba(c, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                //        current_graph->points.edgecolor.alpha);
                //cairo_set_line_width(c, current_graph->points.linewidth);
                //cairo_stroke(c);
                cairo_destroy(c);
            }
        }
    }
}

static void draw_point_o(cairo_t *cr, alpoint2d pw)
{
    cairo_new_sub_path(cr);
    if (current_graph->points.filled) {
        cairo_arc(cr, pw.x, pw.y, current_graph->points.size * 0.5, 0.0, 2*PI);
        cairo_close_path(cr);
        cairo_set_source_rgba(cr, current_graph->points.facecolor.r, current_graph->points.facecolor.g, current_graph->points.facecolor.b, 
                current_graph->points.facecolor.alpha);
        cairo_fill(cr);
    }
    cairo_new_sub_path(cr);
    cairo_arc(cr, pw.x, pw.y, current_graph->points.size * 0.5, 0.0, 2*PI);
    cairo_set_line_width(cr, current_graph->points.linewidth);
    cairo_set_source_rgba(cr, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                current_graph->points.edgecolor.alpha);
    cairo_stroke(cr);
}

static void draw_point_x(cairo_t *cr, alpoint2d pw)
{
    const double offset = sqrt(0.125 * current_graph->points.size * current_graph->points.size);
    cairo_move_to(cr, pw.x - offset, pw.y - offset);
    cairo_line_to(cr, pw.x + offset, pw.y + offset);
    cairo_move_to(cr, pw.x - offset, pw.y + offset);
    cairo_line_to(cr, pw.x + offset, pw.y - offset);
    cairo_set_line_width(cr, current_graph->points.linewidth);
    cairo_set_source_rgba(cr, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                current_graph->points.edgecolor.alpha);
    cairo_stroke(cr);
}

static void draw_point_plus(cairo_t *cr, alpoint2d pw)
{
    cairo_move_to(cr, pw.x - current_graph->points.size * 0.5, pw.y);
    cairo_line_to(cr, pw.x + current_graph->points.size * 0.5, pw.y);
    cairo_move_to(cr, pw.x, pw.y - current_graph->points.size * 0.5);
    cairo_line_to(cr, pw.x, pw.y + current_graph->points.size * 0.5);
    cairo_set_line_width(cr, current_graph->points.linewidth);
    cairo_set_source_rgba(cr, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                current_graph->points.edgecolor.alpha);
    cairo_stroke(cr);
}

static void draw_point_star(cairo_t *cr, alpoint2d pw)
{
    const double offset = sqrt(0.125 * current_graph->points.size * current_graph->points.size);
    cairo_move_to(cr, pw.x - offset, pw.y - offset);
    cairo_line_to(cr, pw.x + offset, pw.y + offset);
    cairo_move_to(cr, pw.x - offset, pw.y + offset);
    cairo_line_to(cr, pw.x + offset, pw.y - offset);
    cairo_move_to(cr, pw.x - current_graph->points.size * 0.5, pw.y);
    cairo_line_to(cr, pw.x + current_graph->points.size * 0.5, pw.y);
    cairo_move_to(cr, pw.x, pw.y - current_graph->points.size * 0.5);
    cairo_line_to(cr, pw.x, pw.y + current_graph->points.size * 0.5);
    cairo_set_line_width(cr, current_graph->points.linewidth);
    cairo_set_source_rgba(cr, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                current_graph->points.edgecolor.alpha);
    cairo_stroke(cr);
}

static void draw_point_square(cairo_t *cr, alpoint2d pw)
{
    const double offset = sqrt(0.125 * current_graph->points.size * current_graph->points.size);

    if (current_graph->points.filled) {
        cairo_rectangle(cr, pw.x - offset, pw.y - offset,  2 * offset, 2 * offset);
        cairo_set_source_rgba(cr, current_graph->points.facecolor.r, current_graph->points.facecolor.g, current_graph->points.facecolor.b, 
                current_graph->points.facecolor.alpha);
        cairo_fill(cr);
    }

    cairo_rectangle(cr, pw.x - offset, pw.y - offset, 2  * offset, 2 * offset);
    cairo_set_line_width(cr, current_graph->points.linewidth);
    cairo_set_source_rgba(cr, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                current_graph->points.edgecolor.alpha);
    cairo_stroke(cr);

}

static void draw_point_square45(cairo_t *cr, alpoint2d pw)
{
    const double offset = current_graph->points.size * 0.5;

    if (current_graph->points.filled) {
        cairo_move_to(cr, pw.x, pw.y - offset);
        cairo_line_to(cr, pw.x + offset, pw.y);
        cairo_line_to(cr, pw.x, pw.y + offset);
        cairo_line_to(cr, pw.x - offset, pw.y);
        cairo_close_path(cr);
        cairo_set_source_rgba(cr, current_graph->points.facecolor.r, current_graph->points.facecolor.g, current_graph->points.facecolor.b, 
                current_graph->points.facecolor.alpha);
        cairo_fill(cr);
    }

    cairo_move_to(cr, pw.x, pw.y - offset);
    cairo_line_to(cr, pw.x + offset, pw.y);
    cairo_line_to(cr, pw.x, pw.y + offset);
    cairo_line_to(cr, pw.x - offset, pw.y);
    cairo_close_path(cr);
    cairo_set_line_width(cr, current_graph->points.linewidth);
    cairo_set_source_rgba(cr, current_graph->points.edgecolor.r, current_graph->points.edgecolor.g, current_graph->points.edgecolor.b,
                current_graph->points.edgecolor.alpha);
    cairo_stroke(cr);
}


static void draw_graph(cairo_surface_t *cs, alfigure *fig)
{
    cairo_t *c;
    unsigned int i, j;
    alpoint2d p0, p1, p2, p3;

    if (fig->ngraph == 0) {
#ifdef CAIRO_DEBUG
        printf("draw_graph: No graph specified.\n");
#endif
        return;
    }


    for (j = 0; j < fig->ngraph; j++) {
        if (algraph_get(fig->graph_id[j])) {
            if (current_graph->show_graphline) {
                c = cairo_create(cs);

                //cairo_set_source_surface(c, cs, 1, 1);

                if (current_graph->interpolation_method == 0) {
                    for (i = 0; i < current_graph->ndata - 1; i++) {
                        p0 = fig_to_world(current_graph->data[i], fig);
                        cairo_move_to(c, p0.x, p0.y);
                        p3 = fig_to_world(current_graph->data[i+1], fig);
                        p1 = fig_to_world(current_graph->bezier_control_points[2*i], fig);
                        p2 = fig_to_world(current_graph->bezier_control_points[2*i+1], fig);
                        cairo_curve_to(c, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
                        //printf("(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
                    }
                }
                else if (current_graph->interpolation_method == 1) {
                    for (i = 0; i < current_graph->ndata - 1; i++) {
                        p0 = fig_to_world(current_graph->data[i], fig);
                        p3 = fig_to_world(current_graph->data[i+1], fig);
                        cairo_move_to(c, p0.x, p0.y);
                        cairo_line_to(c, p3.x, p3.y);
                    }
                }
                cairo_set_line_width(c, current_graph->linewidth);
                cairo_set_source_rgb(c, current_graph->linecolor.r, current_graph->linecolor.g, current_graph->linecolor.b);

                clip_graph(c);

                cairo_stroke(c);
                cairo_destroy(c);
            }
        }
    }
}

static void clip_graph(cairo_t *cr)
{
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_rectangle(cr, FIGURE_X, FIGURE_Y, FIGURE_WIDTH, -FIGURE_HEIGHT);
    //cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_clip_preserve(cr);
}


/*
 * Check if a given point is in the figure window.
 */
static bool point_in_figure(alpoint2d pw)
{
    if (pw.x >= FIGURE_X
            && pw.x <= FIGURE_X + FIGURE_WIDTH
            && pw.y >= FIGURE_Y - FIGURE_HEIGHT
            && pw.y <= FIGURE_Y)
        return true;
    return false;
}
