#include <iostream>

#include <Node.h>
#include <Point2D.h>
#include <Distance.h>
#include <Bearing.h>
#include <Color.h>
#include <ArcType.h>

using namespace std;

void ArcType::render(View *canvas, const Node *n) const
{
    const Point2D *p = (const Point2D *)(n->getValue("Point"));
	const Bearing *angle1 = (const Bearing *)(n->getValue("Angle 1"));
	const Bearing *angle2 = (const Bearing *)(n->getValue("Angle 2"));
	const Distance *r = (const Distance *)(n->getValue("Radius"));
	const Color *color = (const Color *)(n->getValue("Color"));
	const Distance *width = (const Distance *)(n->getValue("Width"));

    cout << "Arc from (" << p->getX() << ", " << p->getY() << ") " << angle1->getAngle() << " to " << angle2->getAngle() << endl;
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

