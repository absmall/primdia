#include <iostream>

#include <Node.h>
#include <Point2D.h>
#include <Distance.h>
#include <Bearing.h>
#include <Color.h>
#include <GtkView.h>
#include <GtkCanvas.h>
#include <ArcType.h>

static ArcType instance;

void ArcType::render(View *canvas, const Node *n) const
{
    const Point2D *p = (const Point2D *)(n->getValue("Point"));
	const Bearing *angle1 = (const Bearing *)(n->getValue("Angle 1"));
	const Bearing *angle2 = (const Bearing *)(n->getValue("Angle 2"));
	const Distance *r = (const Distance *)(n->getValue("Radius"));
	const Color *color = (const Color *)(n->getValue("Color"));
	const Distance *width = (const Distance *)(n->getValue("Width"));

	cairo_t *c = ((GtkView *)canvas)->canvas->getCairoContext();

	// move to coord in plane
	cairo_move_to(c, p->getX(), p->getY());
	cairo_new_sub_path(c);
	cairo_set_source_rgba(c,
	                      color->getRed(),
	                      color->getGreen(),
	                      color->getBlue(),
	                      color->getAlpha());
	// draw the point
	cairo_arc(c, p->getX(), p->getY(), r->getDistance(), angle1->getAngle(), angle2->getAngle());
	cairo_set_line_width(c, width->getDistance());
	// display the point
	cairo_stroke(c);
}

bool ArcType::boundingBox(View *canvas, const Node *n,
                           double *left, double *right, double *top, double *bottom)
{
	const Point2D *p = (const Point2D *)(n->getValue("Point"));
	const Distance *r = (const Distance *)(n->getValue("Radius"));
	const Distance *w = (const Distance *)(n->getValue("Width"));

	if (p == NULL)
	{
		return false;
	}

	if (r == NULL || w == NULL)
	{
		*left = p->getX();
		*right = p->getX();
		*top = p->getY();
		*bottom = p->getY();
	} else {
		*left = p->getX() - r->getDistance() - w->getDistance() / 2;
		*right = p->getX() + r->getDistance() + w->getDistance() / 2;
		*top = p->getY() + r->getDistance() + w->getDistance() / 2;
		*bottom = p->getY() - r->getDistance() - w->getDistance() / 2;
	}
	return true;
}

