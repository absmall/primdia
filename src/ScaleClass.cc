#include <Node.h>
#include <Type.h>
#include <Point2D.h>
#include <Distance.h>
#include <ScaleType.h>
#include <ConstraintSet.h>

static ScaleType instance;

ScaleClass::ScaleClass() : Primitive("ScalePrimitive")
{
}

void ScaleClass::initialize()
{
	std::string start = "Point 1";
	std::string end = "Point 2";
	std::string comb = "Combination";
	std::string ratio = "Ratio";

	ConstraintSet &cs1 = createConstraintSubset(&solveSE);
	ConstraintSet &cs2 = createConstraintSubset(&solveSC);
	ConstraintSet &cs3 = createConstraintSubset(&solveCE);

	addAttribute(start, Type::getType("2D Point"));
	addAttribute(end, Type::getType("2D Point"));
	addAttribute(comb, Type::getType("2D Point"));
	addAttribute(ratio, Type::getType("Distance"));

	cs1.addConstraint(start);
	cs1.addConstraint(end);
	cs1.addConstraint(ratio);
	cs2.addConstraint(start);
	cs2.addConstraint(comb);
	cs2.addConstraint(ratio);
	cs3.addConstraint(comb);
	cs3.addConstraint(end);
	cs3.addConstraint(ratio);
}

ScaleClass::~ScaleClass()
{
}

Value *solveSE(const Node *n, const std::string &name, const View *view)
{
	Point2D *p1 = (Point2D *)n->getValue("Point 1");
	Point2D *p2 = (Point2D *)n->getValue("Point 2");
	Distance *d = (Distance *)n->getValue("Ratio");

	if (name == std::string("Combination"))
	{
		return new Point2D(p1->getX() * (1-d->getDistance()) + p2->getX() * (d->getDistance()),
				p1->getY() * (1-d->getDistance()) + p2->getY() * (d->getDistance()));
	} else {
		throw std::string("No member ") + name + std::string(" in Scale");
	}
}

Value *solveSC(const Node *n, const std::string &name, const View *view)
{
	Point2D *p1 = (Point2D *)n->getValue("Point 1");
	Point2D *p2 = (Point2D *)n->getValue("Combination");
	Distance *d = (Distance *)n->getValue("Ratio");

	if (name == std::string("Point 2"))
	{
		return new Point2D((p1->getX() * (1-d->getDistance()) - p2->getX()) / d->getDistance(),
				(p1->getY() * (1-d->getDistance()) - p2->getY()) / d->getDistance());
	} else {
		throw std::string("No member ") + name + std::string(" in Scale");
	}
}

Value *solveCE(const Node *n, const std::string &name, const View *view)
{
	Point2D *p1 = (Point2D *)n->getValue("Combination");
	Point2D *p2 = (Point2D *)n->getValue("Point 2");
	Distance *d = (Distance *)n->getValue("Ratio");

	if (name == std::string("Point 1"))
	{
		return new Point2D((p1->getX()  + p2->getX() * d->getDistance()) /  (1-d->getDistance()),
				(p1->getX()  + p2->getX() * d->getDistance()) /  (1-d->getDistance()));
	} else {
		throw std::string("No member ") + name + std::string(" in Scale");
	}
}

void ScaleType::render(View *canvas, const Node *n) const
{
}

bool ScaleType::boundingBox(View *canvas, const Node *n,
                            double *left, double *right, double *top, double *bottom)
{
	return false;
}
