#include <math.h>

#include <Color.h>
#include <Binding.h>
#include <GtkUtil.h>
#include <GtkView.h>
#include <Document.h>
#include <ColorType.h>

using namespace std;

static double
scale_round (double val, double factor)
{
  val = floor (val * factor + 0.5);
  val = MAX (val, 0);
  val = MIN (val, factor);
  return val;
}

class GtkColorType : public ColorType
{
public:
	/**
	 * Get a value from the user
	 * @param v A view to be used in some
	 *          way to retrieve a value.
	 * @param b A binding to initialize with the value.
	 */
	void input(View *v, Binding *b) const;
} static instance;

void GtkColorType::input(View *v, Binding *b) const
{
	GtkView *gtkv = static_cast<GtkView *>(v);
	GtkWidget *dialog = gtk_color_selection_dialog_new(_("Select color"));
	GtkColorSelection *colorsel = GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(dialog)));
	gtk_color_selection_set_has_opacity_control(colorsel, true);

	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));

	if (ret == GTK_RESPONSE_OK)
	{
		GdkColor color;
		double alpha = gtk_color_selection_get_current_alpha(colorsel);
		gtk_color_selection_get_current_color(colorsel, &color);
		if (b->setValue(gtkv->document, new Color((unsigned char)scale_round(color.red/65535.0, 255),
						(unsigned char)scale_round(color.green/65535.0,255),
						(unsigned char)scale_round(color.blue/65535.0,255),
						(unsigned char)scale_round(alpha/65535.0,255))))
		{
			gtkv->document->update(Document::SetValue, NULL, NULL, b);
		} else {
			GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, _("The binding could not be set. It overconstrains an object in the diagram."));
			g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
			gtk_dialog_run(GTK_DIALOG(dialog));
		}
	}

	gtk_widget_destroy(dialog);
}
