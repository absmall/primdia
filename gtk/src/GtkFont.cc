#include <gtk/gtk.h>

#include <Font.h>
#include <Binding.h>
#include <GtkView.h>
#include <Document.h>
#include <GtkFontType.h>

static GtkFontType instance;

void GtkFontType::input(View *v, Binding *b) const
{
	GtkView *gtkv = static_cast<GtkView *>(v);
	GtkWidget *dialog = gtk_font_selection_dialog_new(_("Please choose font"));

	if (b->hasValue())
	{
		gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(dialog), b->getValue()->getString().c_str());
	}

	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));
	if (ret == GTK_RESPONSE_OK)
	{
		if (b->setValue(gtkv->document, new Font(gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog)))))
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
