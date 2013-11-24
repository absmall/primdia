#include <Font.h>
#include <Node.h>
#include <Label.h>
#include <Point2D.h>
#include <TextType.h>
#include <iostream>

using namespace std;

void TextType::render(View *canvas, const Node *n) const
{
	// Retrieve every node to guarantee that a value is calculated for them
	const Point2D *p1 = (const Point2D *)(n->getValue("Top Left"));
	const Point2D *p2 = (const Point2D *)(n->getValue("Bottom Right"));
	const Label *t = (const Label *)(n->getValue("Text"));
	const Font *f = (const Font *)(n->getValue("Font"));

    cout << "(" << p1->getX() << ", " << p1->getY() << ") - " << t->getText() << " - (" << p2->getX() << ", " << p2->getY() << ")" << endl;
}

bool TextType::boundingBox(View *canvas, const Node *n,
                           double *left, double *right, double *top, double *bottom)
{
	const Label *t = (const Label *)(n->getValue("Text"));
	const Point2D *p1 = (const Point2D *)(n->getValue("Bottom Left"));
	const Point2D *p2 = (const Point2D *)(n->getValue("Top Right"));


	if (t == NULL || p1 == NULL || p2 == NULL) return false;

	*left = p1->getX();
	*right = p2->getX();
	*bottom = p1->getY();
	*top = p2->getY();

	return true;
}
