#ifndef ALCAIRO_H
#define ALCAIRO_H

#include <cairo.h>
#include <cairo-pdf.h>
#include "alFigure.h"
#include "color.h"



cairo_surface_t* alcairo_open_pdf(const char *fname);
void alcairo_draw_figure(cairo_surface_t *cs, alfigure *fig);
void alcairo_write_pdf(cairo_surface_t *cs);



#endif
