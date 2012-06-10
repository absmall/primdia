#include <Label.h>
#include <Binding.h>
#include <GtkUtil.h>
#include <GtkView.h>
#include <Document.h>
#include <LabelType.h>

class GtkLabelType : public LabelType
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

void GtkLabelType::input(View *v, Binding *b) const
{
	GtkView *gtkv = static_cast<GtkView *>(v);
	GtkWidget *dialog = input_dialog(gtkv->window, _("Please enter text"));

	GtkWidget *text = gtk_entry_new();
	gtk_widget_show(text);

	GtkWidget *label = gtk_label_new(_("Please enter text"));
	gtk_widget_show(label);

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), text);

	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));
	if (ret == GTK_RESPONSE_ACCEPT)
	{
		if (!b->setValue(gtkv->document, new Label(gtk_entry_get_text(GTK_ENTRY(text)))))
		{
			GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, _("The binding could not be set. It overconstrains an object in the diagram."));
			g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
			gtk_dialog_run(GTK_DIALOG(dialog));
		}
	}

	gtk_widget_destroy(dialog);
}
