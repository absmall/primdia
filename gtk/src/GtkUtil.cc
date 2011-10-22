#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <GtkUtil.h>

static int defresponse(GtkWidget *widget, GdkEventKey *key)
{
	if (key->keyval == GDK_KEY_Escape)
	{
		gtk_dialog_response(GTK_DIALOG(widget), GTK_RESPONSE_REJECT);
	} else if (key->keyval == GDK_KEY_Return) {
		gtk_dialog_response(GTK_DIALOG(widget), GTK_RESPONSE_ACCEPT);
	}

	return FALSE;
}

GtkWidget *input_dialog(GtkWidget *parent, const char *title)
{
	GtkWidget *ret = gtk_dialog_new_with_buttons(title,
		GTK_WINDOW(parent),
		GTK_DIALOG_MODAL,
		GTK_STOCK_OK,
		GTK_RESPONSE_ACCEPT,
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_REJECT,
		NULL);

	g_signal_connect(G_OBJECT(ret), "key-press-event", G_CALLBACK(defresponse), NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(ret), GTK_RESPONSE_ACCEPT);

	return ret;
}

GtkWidget *open_dialog(GtkWidget *parent, const char *message)
{
	GtkWidget *ret = gtk_file_chooser_dialog_new(
			message,
			GTK_WINDOW(parent),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);

	g_signal_connect(G_OBJECT(ret), "key-press-event", G_CALLBACK(defresponse), NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(ret), GTK_RESPONSE_ACCEPT);

	return ret;
}

GtkWidget *save_dialog(GtkWidget *parent, const char *message)
{
	GtkWidget *ret = gtk_file_chooser_dialog_new(
			message,
			GTK_WINDOW(parent),
			GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			NULL);

	g_signal_connect(G_OBJECT(ret), "key-press-event", G_CALLBACK(defresponse), NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(ret), GTK_RESPONSE_ACCEPT);

	return ret;
}

GtkWidget *save_query(GtkWidget *parent, const char *message, const char *docname)
{
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_WARNING,
		GTK_BUTTONS_NONE,
		message);

	gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dialog),
		_("<span weight=\"bold\" size=\"larger\">"
		"Save changes to document \"%s\" before closing?</span>\n\n"
		"If you close without saving, your changes will be discarded."),
		docname);

	GtkWidget *close_button;
	close_button = gtk_button_new_with_mnemonic(_("Close _without saving"));
	gtk_widget_show(close_button);
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), close_button, GTK_RESPONSE_NO);

	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_SAVE, GTK_RESPONSE_YES);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);


	return dialog;
}

void warn_dialog(GtkWidget *parent, const char *description, const char *message)
{
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_WARNING,
		GTK_BUTTONS_CLOSE,
		message);

	g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void error_dialog(GtkWidget *parent, const char *message)
{
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_CLOSE,
		message);

	g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
}
