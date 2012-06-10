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
	GtkWidget *dialog = gtk_font_chooser_dialog_new(_("Please choose font"), NULL);

	if (b->hasValue())
	{
		gtk_font_chooser_set_font(GTK_FONT_CHOOSER(dialog), b->getValue()->getString().c_str());
	}

	gint ret = gtk_dialog_run(GTK_DIALOG(dialog));
	if (ret == GTK_RESPONSE_OK)
	{
		if (!b->setValue(gtkv->document, new Font(gtk_font_chooser_get_font(GTK_FONT_CHOOSER(dialog)))))
		{
			GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, _("The binding could not be set. It overconstrains an object in the diagram."));
			g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
			gtk_dialog_run(GTK_DIALOG(dialog));
		}
	}

	gtk_widget_destroy(dialog);
}
