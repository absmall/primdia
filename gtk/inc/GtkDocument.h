#ifndef __GTKDOCUMENT_H__
#define __GTKDOCUMENT_H__

#include <config/common.h>

/**
 * Contains gtk-specific parts of the document
 */

#include <gtk/gtk.h>
#include <gconf/gconf.h>

#include <Document.h>

class GtkDocument : public Document
{
public:
	enum UpdateCommands
	{
		Deselection = Document::MAX,
		ReloadTool,
		MAX
	};

	enum
	{
		COL_NAME = 0,
		COL_VALUE,
		COL_SETSVALUE,
		COL_SELECTIONBOUND,
		COL_REFERENCE,
		NUM_COLS
	};

	/**
	 * Default constructor
	 */
	GtkDocument();

	/**
	 * Default destructor
	 */
	~GtkDocument();

	/**
	 * Pass a message to the document that a particular type of update was performed
	 * so the views must be notified
	 * @param command What kind of update happened
	 * @param attribute A reference to an affected attribute
	 * @param node A reference to an affected node
	 * @param binding A reference to an affected binding
	 */
	void update(int command, Attribute *attribute, Interface *node, Binding *binding);

	/**
	 * Load a document.
	 * @param parent If a dialog needs to be shown, this will be its parent
	 */
	void load(GtkWidget *parent);

	/**
	 * Save a document using the current name. If the document has not been saved
	 * before, the user will be asked to enter a name.
	 * @param parent If a dialog needs to be shown, this will be its parent
	 */
	void save(GtkWidget *parent);

	/**
	 * Save a document. The user will be asked to enter a name.
	 * @param parent If a dialog needs to be shown, this will be its parent
	 */
	void saveAs(GtkWidget *parent);

	/**
	 * A gtk data store for representing the nodes of the document as a treeview.
	 * This will generally parallel the node list of the view's document.
	 */
	GtkTreeStore *node_store;
};

#endif /* __GTKDOCUMENT_H__ */
