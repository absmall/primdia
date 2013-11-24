#include <math.h>
#include <assert.h>

#include <Node.h>
#include <Type.h>
#include <Value.h>
#include <Bearing.h>
#include <Point2D.h>
#include <Distance.h>
#include <LineType.h>
#include <ConstraintSet.h>

static LineType instance;

using namespace std;

LineClass::LineClass() : Primitive("LinePrimitive")
{
}

void LineClass::initialize()
{
	// Label attributes
	std::string p1 = "Point 1";
	std::string p2 = "Point 2";
	std::string l = "Length";
	std::string s = "Slope";
	std::string v = "Color";
	std::string w = "Width";

	// Label subsets
	ConstraintSet &cs1 = createConstraintSubset(&solvePP);
	ConstraintSet &cs2 = createConstraintSubset(&solveP1LA);
	ConstraintSet &cs3 = createConstraintSubset(&solveP2LA);

	// Install attributes
	addAttribute(p1, Type::getType("2D Point"));
	addAttribute(p2, Type::getType("2D Point"));
	addAttribute(l, Type::getType("Distance"));
	addAttribute(s, Type::getType("Bearing"));
	addAttribute(v, Type::getType("Color"));
	addAttribute(w, Type::getType("Distance"));

	// Install subsets
	cs1.addConstraint(p1);
	cs1.addConstraint(p2);
	cs1.addConstraint(v);
	cs1.addConstraint(w);
	cs2.addConstraint(p1);
	cs2.addConstraint(l);
	cs2.addConstraint(s);
	cs2.addConstraint(v);
	cs2.addConstraint(w);
	cs3.addConstraint(p2);
	cs3.addConstraint(l);
	cs3.addConstraint(s);
	cs3.addConstraint(v);
	cs3.addConstraint(w);
}

LineClass::~LineClass()
{
}

Value *solvePP(const Node *n, const std::string &name, const View *view)
{
	Point2D *p1 = (Point2D *)n->getValue("Point 1");
	Point2D *p2 = (Point2D *)n->getValue("Point 2");

	if (name == std::string("Length")) {
		double x1 = p1->getX();
		double y1 = p1->getY();
		double x2 = p2->getX();
		double y2 = p2->getY();
		return new Distance(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
	} else if (name == std::string("Slope")) {
		double x1 = p1->getX();
		double y1 = p1->getY();
		double x2 = p2->getX();
		double y2 = p2->getY();
		return new Bearing(y2-y1,x2-x1);
	} else {
		assert(true);
		return NULL;
	}
}

Value *solveP1LA(const Node *n, const std::string &name, const View *view)
{
	Point2D *p1 = (Point2D *)n->getValue("Point 1");
	Distance *l = (Distance *)n->getValue("Length");
	Bearing *b = (Bearing *)n->getValue("Slope");

	if (name == std::string("Point 2")) {
		return new Point2D(p1->getX() + cos(b->getAngle())*l->getDistance(),
		                   p1->getY() + sin(b->getAngle())*l->getDistance());
	} else {
		throw std::string("No member ") + name + std::string(" in Line");
		return NULL;
	}
}

Value *solveP2LA(const Node *n, const std::string &name, const View *view)
{
	Point2D *p2 = (Point2D *)n->getValue("Point 2");
	Distance *l = (Distance *)n->getValue("Length");
	Bearing *b = (Bearing *)n->getValue("Slope");

	if (name == std::string("Point 1")) {
		return new Point2D(p2->getX() - cos(b->getAngle())*l->getDistance(),
		                   p2->getY() - sin(b->getAngle())*l->getDistance());
	} else {
		throw std::string("No member ") + name + std::string(" in Line");
		return NULL;
	}
}
