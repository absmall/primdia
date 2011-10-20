#include <math.h>

#include <Node.h>
#include <GtkView.h>
#include <Point2D.h>
#include <Color.h>
#include <Distance.h>
#include <PointType.h>
#include <GtkCanvas.h>

static PointType instance;

void PointType::render(View *canvas, const Node *n) const
{
	const Point2D *p = (const Point2D *)(n->getValue("Point"));
	const Distance *r = (const Distance *)(n->getValue("Radius"));
	const Color *color = (const Color *)(n->getValue("Color"));

	cairo_t *c = ((GtkView *)canvas)->canvas->getCairoContext();

	double angle1 = 0;
	double angle2 = 2 * M_PI;

	// set the color of the point
	cairo_set_source_rgba(c,
			color->getRed(),
			color->getGreen(),
			color->getBlue(),
			color->getAlpha());
	// move to coord in plane
	cairo_move_to(c, p->getX(), p->getY());
	// draw the point
	cairo_arc(c, p->getX(), p->getY(), r->getDistance(), angle1, angle2);
	cairo_fill_preserve(c);
	// display the point
	cairo_stroke(c);
}

bool PointType::boundingBox(View *canvas, const Node *n,
                           double *left, double *right, double *top, double *bottom)
{
	const Point2D *p = (const Point2D *)(n->getValue("Point"));
	const Distance *r = (const Distance *)(n->getValue("Radius"));

	if (p == NULL)
	{
		return false;
	}

	if (r == NULL)
	{
		*left = p->getX();
		*right = p->getX();
		*top = p->getY();
		*bottom = p->getY();
	} else {
		*left = p->getX() - r->getDistance();
		*right = p->getX() + r->getDistance();
		*top = p->getY() + r->getDistance();
		*bottom = p->getY() - r->getDistance();
	}

	return true;
}
