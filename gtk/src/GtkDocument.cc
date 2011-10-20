#include <Value.h>
#include <Binding.h>
#include <GtkUtil.h>
#include <Interface.h>
#include <GtkDocument.h>
#include <InterfaceAttribute.h>

GtkDocument::GtkDocument():Document()
{
	node_store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_POINTER);
}

GtkDocument::~GtkDocument()
{
}

void GtkDocument::update(int command, Attribute *attribute, Interface *node, Binding *binding)
{
	GtkTreeIter iter, attr_iter;

	switch(command)
	{
		case Document::AddNode:
		{
			gtk_tree_store_append(node_store, &iter, NULL);

			gtk_tree_store_set(node_store, &iter,
				GtkDocument::COL_NAME, node->getName().c_str(),
				GtkDocument::COL_REFERENCE, node,
				-1);

			foreach(j, node->attributes)
			{
				std::string value;

				if (j->second->hasValue())
				{
					value = j->second->getBinding()->getValue()->getString();
				}

				gtk_tree_store_append(node_store, &attr_iter, &iter);
				gtk_tree_store_set(node_store, &attr_iter,
					GtkDocument::COL_NAME, j->first.c_str(),
					GtkDocument::COL_VALUE, value.c_str(),
					GtkDocument::COL_SETSVALUE, j->second->hasSetValue(),
					GtkDocument::COL_SELECTIONBOUND, false,
					GtkDocument::COL_REFERENCE, j->second,
					-1);
			}

		}
		break;
		default:
		break;
	}
	Document::update(command, attribute, node, binding);
}

void GtkDocument::save(GtkWidget *parent)
{
	try
	{
		Document::save();
	} catch(...) {
		saveAs(parent);
	}
}

void GtkDocument::load(GtkWidget *parent)
{
	GtkWidget *dialog = open_dialog(
			parent,
			"Please enter the name of the file");

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;

		try {
			filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		} catch( const std::string &exception ) {
			gtk_widget_destroy(dialog);
			throw exception;
		}
		Document::load(filename);
		g_free(filename);
	} else {
		gtk_widget_destroy(dialog);
		throw cancelled("No filename selected");
	}

	gtk_widget_destroy(dialog);
}

void GtkDocument::saveAs(GtkWidget *parent)
{
	GtkWidget *dialog = save_dialog(
			parent,
			"Please enter the name of the file");

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;

		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		Document::saveAs(filename);
		g_free(filename);
	} else {
		gtk_widget_destroy(dialog);
		throw cancelled("No filename selected");
	}

	gtk_widget_destroy(dialog);
}
