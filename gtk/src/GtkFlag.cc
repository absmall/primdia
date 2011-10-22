#include <Flag.h>
#include <Binding.h>
#include <GtkUtil.h>
#include <GtkView.h>
#include <Document.h>
#include <FlagType.h>

using namespace std;

class GtkFlagType : public FlagType
{
public:
	/**
	 * Get a value from the user
	 * @param v A view to be used in some
	 *          way to retrieve a value.
	 * @param b A binding to initialize with the value.
	 */
	void input(View *v, Binding *b) const;
} instance;

void GtkFlagType::input(View *v, Binding *b) const
{
	GtkWidget *check;
	GtkView *gtkv = static_cast<GtkView *>(v);
	GtkWidget *dialog = input_dialog(gtkv->window, _("Is this line visible?"));

	check = gtk_check_button_new_with_label("Visible");
	gtk_widget_show(check);

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), check);
	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));

	if (ret == GTK_RESPONSE_ACCEPT)
	{
		if (b->setValue(gtkv->document, new Flag(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check)))))
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
