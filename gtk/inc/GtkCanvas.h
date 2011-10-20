#ifndef __GTKCANVAS_H__
#define __GTKCANVAS_H__

#include <config/common.h>

#include <Canvas.h>

class GtkCanvas : public Canvas
{
public:
	GtkCanvas(View *v);

	~GtkCanvas();

	void refresh();

	static void paint (GtkWidget *widget, GdkEventExpose *eev, gpointer *data);

	cairo_t *getCairoContext();

	GtkWidget *getCairoWidget();

	void toCanvas(double *x, double *y);
private:
	View *view;
	GtkWidget *canvas;
	cairo_t *cairo;
};

#endif /* __GTKCANVAS_H__ */
