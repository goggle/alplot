#include <cairo.h>
#include <cairo-pdf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEX 842
#define SIZEY 595

void paint(cairo_surface_t *cs)
{
    cairo_t *c;
    c = cairo_create(cs);

    //cairo_rectangle(c, 50.0, 50.0, 700, 480);
    //cairo_set_source_rgb(c, 0.8, 0.8, 0.8);
    //cairo_fill(c);

    cairo_move_to(c, 50.0, 50.0);
    cairo_set_source_rgb(c, 0.0, 0.0, 0.0);
    cairo_line_to(c, 750, 50.0);
    cairo_line_to(c, 750, 300);
    cairo_set_line_width(c, 2.0);
    cairo_stroke(c);

    cairo_move_to(c, 10.0, 10.0);
    cairo_set_source_rgb(c, 1.0, 0.0, 0.0);
    cairo_show_text(c, "Hello World!");

    cairo_show_page(c);
    cairo_destroy(c);
}

void writepdf(const char *fname)
{
    cairo_surface_t *cs;
    cs = cairo_pdf_surface_create(fname, SIZEX, SIZEY);
    paint(cs);

    cairo_surface_flush(cs);
    cairo_surface_destroy(cs);
}


int main(void)
{
    writepdf("test.pdf");
    return 0;
}
