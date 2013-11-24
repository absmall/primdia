#include <Node.h>
#include <Color.h>
#include <Point2D.h>
#include <LineType.h>
#include <Distance.h>
#include <iostream>

using namespace std;

void LineType::render(View *canvas, const Node *n) const
{
	const Point2D *p1 = (const Point2D *)(n->getValue("Point 1"));
	const Point2D *p2 = (const Point2D *)(n->getValue("Point 2"));
	const Color *color = (const Color *)(n->getValue("Color"));
	const Distance *width = (const Distance *)(n->getValue("Width"));

    cout << "(" << p1->getX() << ", " << p1->getY() << ")-(" << p2->getX() << ", " << p2->getY() << ")" << endl;
}

bool LineType::boundingBox(View *canvas, const Node *n,
                           double *left, double *right, double *top, double *bottom)
{
	const Point2D *p1 = (const Point2D *)(n->getValue("Point 1"));
	const Point2D *p2 = (const Point2D *)(n->getValue("Point 2"));
	const Distance *d = (const Distance *)(n->getValue("Width"));

	if (p1 == NULL && p2 == NULL) return false;

	if (p1 == NULL)
	{
		*left = *right = p2->getX();
		*top = *bottom = p2->getY();
	} else if (p2 == NULL) {
		*left = *right = p1->getX();
		*top = *bottom = p1->getY();
	} else if (d == NULL) {
		*left = std::min(p1->getX(), p2->getX());
		*right = std::max(p1->getX(), p2->getX());
		*bottom = std::min(p1->getY(), p2->getY());
		*top = std::max(p1->getY(), p2->getY());
	} else {
		*left = std::min(p1->getX(), p2->getX()) - d->getDistance()/2;
		*right = std::max(p1->getX(), p2->getX()) + d->getDistance()/2;
		*bottom = std::min(p1->getY(), p2->getY()) - d->getDistance()/2;
		*top = std::max(p1->getY(), p2->getY()) + d->getDistance()/2;
	}

	return true;
}
