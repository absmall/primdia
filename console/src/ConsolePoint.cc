#include <Node.h>
#include <Point2D.h>
#include <Color.h>
#include <Distance.h>
#include <PointType.h>
#include <iostream>

using namespace std;

void PointType::render(View *canvas, const Node *n) const
{
	const Point2D *p = (const Point2D *)(n->getValue("Point"));
	const Distance *r = (const Distance *)(n->getValue("Radius"));
	const Color *color = (const Color *)(n->getValue("Color"));

    cout << "Point at (" << p->getX() << ", " << p->getY() << ")" << endl;
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
