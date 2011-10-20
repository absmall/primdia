#include <Node.h>
#include <Type.h>
#include <Value.h>
#include <Point2D.h>
#include <PointClass.h>
#include <ConstraintSet.h>

using namespace std;

PointClass::PointClass() : Primitive("PointPrimitive")
{
}

void PointClass::initialize()
{
	// Label attributes
	std::string p = "Point";
	std::string r = "Radius";
	std::string c = "Color";

	// Label subsets
	ConstraintSet &cs1 = createConstraintSubset(&solveP);

	// Install attributes
	addAttribute(p, Type::getType("2D Point"));
	addAttribute(r, Type::getType("Distance"));
	addAttribute(c, Type::getType("Color"));

	// Install subsets
	cs1.addConstraint(p);
	cs1.addConstraint(r);
	cs1.addConstraint(c);
}

PointClass::~PointClass()
{
}

Value *solveP(const Node *n, const std::string &name)
{
	throw std::string("No member ") + name + std::string(" in Point");
}
