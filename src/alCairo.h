#ifndef ALCAIRO_H
#define ALCAIRO_H

#include <cairo.h>
#include <cairo-pdf.h>
#include "alFigure.h"
#include "alLine.h"
#include "color.h"





cairo_surface_t* open_pdf(const char *fname);

void draw_figure(cairo_surface_t *cs, alfigure *fig);
//void draw_line(alline *line);

void write_pdf(cairo_surface_t *cs);





#endif
