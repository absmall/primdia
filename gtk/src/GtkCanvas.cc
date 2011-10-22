#include <iostream>

#include <Node.h>
#include <View.h>
#include <GtkView.h>
#include <Document.h>
#include <Attribute.h>
#include <Interface.h>
#include <GtkCanvas.h>
#include <InterfaceAttribute.h>

GtkCanvas::GtkCanvas(View *v)
{
	this->view = v;
	canvas = gtk_drawing_area_new ();
	g_signal_connect (G_OBJECT (canvas), "draw", G_CALLBACK (paint), (void *)this);
}

GtkCanvas::~GtkCanvas()
{
	gtk_widget_destroy (canvas);
}

void GtkCanvas::refresh()
{
	gint width    = gtk_widget_get_allocated_width(canvas);
	gint height   = gtk_widget_get_allocated_height(canvas);

	cairo_save(cairo);
	cairo_identity_matrix(cairo);
	cairo_translate(cairo, width / 2, height / 2);
	cairo_scale(cairo, 1.0, -1.0);

	foreach(i, view->document->getNodes())
	{
		(*i)->render(view);
	}

	InterfaceAttribute *selection = ((GtkView *)view)->selection;
	if (selection != NULL)
	{
		double l, r, t, b;
		if (selection->getInterface()->boundingBox(view,
					&l, &r, &t, &b))
		{
			cairo_rectangle(cairo, l-0.5, b-0.5, r-l+1.0, t-b+1.0);
			cairo_set_source_rgba(cairo, 1.0, 0.0, 0.0, 1.0);
			cairo_set_line_width(cairo, 1.0);
			cairo_stroke(cairo);
		}
	}

	cairo_restore(cairo);
}

void GtkCanvas::paint (GtkWidget *widget, cairo_t *cr, gpointer *data)
{
	((GtkCanvas *)data)->cairo = cr;
	((GtkCanvas *)data)->refresh();
}

cairo_t *GtkCanvas::getCairoContext()
{
	return cairo;
}

GtkWidget *GtkCanvas::getCairoWidget()
{
	return canvas;
}

void GtkCanvas::toCanvas(double *x, double *y)
{
	gint width = gtk_widget_get_allocated_width(GTK_WIDGET(canvas));
	gint height = gtk_widget_get_allocated_height(GTK_WIDGET(canvas));

	*x -= width/2;
	*y = height/2 - *y;
}
