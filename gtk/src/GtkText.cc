#include <pango/pangocairo.h>

#include <Font.h>
#include <Node.h>
#include <Label.h>
#include <GtkView.h>
#include <Point2D.h>
#include <TextType.h>
#include <GtkCanvas.h>
#include <iostream>

using namespace std;

static TextType instance;

void TextType::render(View *canvas, const Node *n) const
{
	PangoLayout *layout;
	PangoFontDescription *desc;

	// Retrieve every node to guarantee that a value is calculated for them
	const Point2D *p1 = (const Point2D *)(n->getValue("Top Left"));
	const Point2D *p2 = (const Point2D *)(n->getValue("Bottom Right"));
	const Label *t = (const Label *)(n->getValue("Text"));
	const Font *f = (const Font *)(n->getValue("Font"));


	cairo_t *c = ((GtkView *)canvas)->canvas->getCairoContext();
	cairo_move_to(c, p1->getX(), p1->getY());

	layout = pango_cairo_create_layout(c);
	pango_layout_set_text(layout, t->getText(), -1);
	pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
	pango_layout_set_width(layout, (p2->getX()-p1->getX()) * PANGO_SCALE);
	pango_layout_set_height(layout, (p1->getY() - p2->getY()) * PANGO_SCALE);
	desc = pango_font_description_from_string(f->getFont());
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	cairo_save(c);
	cairo_translate(c, 0.0, -23.0);
	cairo_scale(c, 1.0, -1.0);
	pango_cairo_update_layout(c, layout);
	pango_cairo_show_layout(c, layout);
	cairo_restore(c);
	cairo_stroke(c);

	g_object_unref(layout);
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
