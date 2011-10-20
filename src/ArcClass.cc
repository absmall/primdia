#include <Node.h>
#include <Type.h>
#include <Value.h>
#include <Point2D.h>
#include <ArcClass.h>
#include <ConstraintSet.h>

ArcClass::ArcClass() : Primitive("ArcPrimitive")
{
}

void ArcClass::initialize()
{
	// Label attributes
	std::string p = "Point";
	std::string r = "Radius";
	std::string a1 = "Angle 1";
	std::string a2 = "Angle 2";
	std::string c = "Color";
	std::string w = "Width";

	// Label subsets
	ConstraintSet &cs1 = createConstraintSubset(&solveA);

	// Install attributes
	addAttribute(p, Type::getType("2D Point"));
	addAttribute(r, Type::getType("Distance"));
	addAttribute(a1, Type::getType("Bearing"));
	addAttribute(a2, Type::getType("Bearing"));
	addAttribute(c, Type::getType("Color"));
	addAttribute(w, Type::getType("Distance"));

	// Install subsets
	cs1.addConstraint(p);
	cs1.addConstraint(r);
	cs1.addConstraint(a1);
	cs1.addConstraint(a2);
	cs1.addConstraint(c);
	cs1.addConstraint(w);
}

ArcClass::~ArcClass()
{
}

Value *solveA(const Node *n, const std::string &name)
{
	throw std::string("No member ") + name + std::string(" in Point");
}
