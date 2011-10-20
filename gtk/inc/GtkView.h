#ifndef __GTKVIEW_H__
#define __GTKVIEW_H__

#include <config/common.h>

#include <stack>
#include <utility>
#include <gtk/gtk.h>

#include <View.h>

class GtkView : public View
{
public:
	GtkView();
	GtkView(Document *document);
	~GtkView();

	static void newDocument(GtkMenuItem *m, gpointer user_data);
	static void newView(GtkMenuItem *m, gpointer user_data);

	// UI utility functions
	void getPoint(void (*notify)(Document *, double, double, void *), void *data);

	void updateWindowList();

	GtkCanvas *canvas;

	InterfaceAttribute *selection;
	GtkTreeIter selectionrow;

	GtkWidget *window;

private:
	static gboolean select_point(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
	static gboolean select_attributes(GtkTreeSelection *selection,
			GtkTreeModel *model,
			GtkTreePath *path,
			gboolean path_currently_selected,
			    gpointer data);
	static gboolean on_treeview_key(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
	static gboolean create_event(GtkToolButton *widget, gpointer data);
	static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
	static void on_save1_activate(GtkMenuItem *menuitem, gpointer user_data);
	static void on_save_as1_activate(GtkMenuItem *menuitem, gpointer user_data);
	static void value_edited(GtkCellRendererText *cellRenderer, gchar *arg1, gchar *arg2, gpointer user_data);
	static void has_value_edited(GtkCellRendererToggle *cellRenderer, gchar *path_string, gpointer data);
	static void bound_edited(GtkCellRendererToggle *cellRenderer, gchar *path_string, gpointer data);
	static void sets_value_edited(GtkCellRendererToggle *cellRenderer, gchar *path_string, gpointer data);
	static void collapse(GtkTreeView *view, GtkTreeIter *iter, GtkTreePath *path, gpointer data);
	static void expand(GtkTreeView *view, GtkTreeIter *iter, GtkTreePath *path, gpointer data);
	static void setValue(Attribute *attr, GtkView *view);
	static void clear_toolbar(GtkWidget *child, void *data);
	static void bind_event(GtkButton *button, gpointer data);
	static void destroy(GtkWidget *widget, gpointer data);

	void initialize();

	void update(int command, Attribute *attribute, Interface *node, Binding *binding);

	void setSelection(GtkTreeIter iter, InterfaceAttribute *ia);

	static unsigned int windowCount;
	GtkWidget *viewConnector;
	GtkMenu *windowMenu;
	GtkStatusbar *statusbar;
	GtkWidget *pane;
	GtkWidget *setvalue;
	GtkWidget *toolbar;
	GtkTooltips *tooltips;
	bool settingValue;
	std::stack<std::pair<int,void *> > canvas_mouse_handler;
};

#endif /* __GTKVIEW_H__ */
