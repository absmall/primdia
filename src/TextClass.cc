#include <Font.h>
#include <Node.h>
#include <Type.h>
#include <Label.h>
#include <Point2D.h>
#include <TextClass.h>
#include <ConstraintSet.h>

using namespace std;

TextClass::TextClass() : Primitive("TextPrimitive")
{
}

void TextClass::initialize()
{
	std::string tl = "Top Left";
	std::string tr = "Top Right";
	std::string bl = "Bottom Left";
	std::string br = "Bottom Right";
	std::string t = "Text";
	std::string f = "Font";

	ConstraintSet &cs1 = createConstraintSubset(&solveTLBR);
	ConstraintSet &cs2 = createConstraintSubset(&solveTRBL);

	addAttribute(tl, Type::getType("2D Point"));
	addAttribute(tr, Type::getType("2D Point"));
	addAttribute(bl, Type::getType("2D Point"));
	addAttribute(br, Type::getType("2D Point"));
	addAttribute(t, Type::getType("Text"));
	addAttribute(f, Type::getType("Font"));

	cs1.addConstraint(tl);
	cs1.addConstraint(br);
	cs1.addConstraint(t);
	cs1.addConstraint(f);

	cs2.addConstraint(tr);
	cs2.addConstraint(bl);
	cs2.addConstraint(t);
	cs2.addConstraint(f);
}

TextClass::~TextClass()
{
}

Value *solveTLBR(const Node *n, const std::string &name)
{
	int width, height;

	const Point2D *p1 = (const Point2D *)n->getValue("Top Left");
	const Point2D *p2 = (const Point2D *)n->getValue("Bottom Right");
	const Label *l = (const Label *)n->getValue("Text");
	const Font *f = (const Font *)(n->getValue("Font"));

	if (name == std::string("Top Right")) {
		return new Point2D(p2->getX() + width, p1->getY());
	} else if (name == std::string("Bottom Left")) {
		return new Point2D(p1->getX(), p2->getY());
	} else {
		throw std::string("No member ") + name + std::string(" in Text");
	}
}

Value *solveTRBL(const Node *n, const std::string &name)
{
	int width, height;

	const Point2D *p1 = (const Point2D *)n->getValue("Top Right");
	const Point2D *p2 = (const Point2D *)n->getValue("Bottom Left");
	const Label *l = (const Label *)n->getValue("Text");
	const Font *f = (const Font *)(n->getValue("Font"));

	if (name == std::string("Top Left")) {
		return new Point2D(p2->getX(), p1->getY());
	} else if (name == std::string("Bottom Right")) {
		return new Point2D(p1->getX(), p2->getY());
	} else {
		throw std::string("No member ") + name + std::string(" in Text");
	}
}
