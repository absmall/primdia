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
	GtkWidget *dialog = gtk_color_chooser_dialog_new(_("Select color"), GTK_WINDOW(gtkv->window));
	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));

	if (ret == GTK_RESPONSE_OK)
	{
        GdkRGBA rgba;
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog), &rgba);

		if (!b->setValue(gtkv->document, new Color((unsigned char)scale_round(rgba.red, 255),
						(unsigned char)scale_round(rgba.green,255),
						(unsigned char)scale_round(rgba.blue,255),
						(unsigned char)scale_round(rgba.alpha,255))))
		{
			GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, _("The binding could not be set. It overconstrains an object in the diagram."));
			g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
			gtk_dialog_run(GTK_DIALOG(dialog));
		}
	}

	gtk_widget_destroy(dialog);
}
