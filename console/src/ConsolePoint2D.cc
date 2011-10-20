#include <Binding.h>
#include <Point2D.h>
#include <Point2DType.h>

using namespace std;

class ConsolePoint2DType : public Point2DType
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

static ConsolePoint2DType instance;

#if 0
void set_point_value(Document *doc, double x, double y, void *data)
{
	Binding *b = (Binding *)data;
	if (!b->setValue(doc, new Point2D(x, y)))
	{
		fprintf(stderr _("The binding could not be set. It overconstrains an object in the diagram."));
	}
}
#endif

void ConsolePoint2DType::input(View *v, Binding *b) const
{
#if 0
	GtkView *gtkv = static_cast<GtkView *>(v);

	gtkv->getPoint(set_point_value, b);
#endif
}
