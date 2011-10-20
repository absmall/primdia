#include <Binding.h>
#include <Point2D.h>
#include <GtkUtil.h>
#include <GtkView.h>
#include <Point2DType.h>

using namespace std;

class GtkPoint2DType : public Point2DType
{
public:
	/**
	 * Get a value from the user
	 * @param v A view to be used in some
	 *          way to retrieve a value.
	 * @param b A binding to initialize with the value.
	 */
	void input(View *v, Binding *b) const;
};

static GtkPoint2DType instance;

static void set_point_value(Document *doc, double x, double y, void *data)
{
	Binding *b = (Binding *)data;
	if (!b->setValue(doc, new Point2D(x, y)))
	{
		error_dialog(NULL, _("The binding could not be set. It overconstrains an object in the diagram."));
	}
}

void GtkPoint2DType::input(View *v, Binding *b) const
{
	GtkView *gtkv = static_cast<GtkView *>(v);

	gtkv->getPoint(set_point_value, b);
}
