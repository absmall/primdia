#include <Binding.h>
#include <GtkUtil.h>
#include <GtkView.h>
#include <Distance.h>
#include <Document.h>
#include <DistanceType.h>

class GtkDistanceType : public DistanceType
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

void GtkDistanceType::input(View *v, Binding *b) const
{
	GtkView *gtkv = static_cast<GtkView *>(v);
	GtkWidget *dialog = input_dialog(gtkv->window, _("Please enter distance"));

	GtkAdjustment *adj = gtk_adjustment_new(0, 0, 1000, 1, 10, 0);
	GtkWidget *value = gtk_spin_button_new(adj, 1, 0);
	gtk_widget_show(value);

	GtkWidget *label = gtk_label_new(_("Please enter distance"));
	gtk_widget_show(label);

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), value);

	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));
	if (ret == GTK_RESPONSE_ACCEPT)
	{
		if (b->setValue(gtkv->document, new Distance(gtk_adjustment_get_value(adj))))
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
