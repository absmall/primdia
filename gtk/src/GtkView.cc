#include <list>
#include <fstream>
#include <iostream>
#include <gdk/gdkkeysyms.h>

#include <Node.h>
#include <Tool.h>
#include <Type.h>
#include <Value.h>
#include <Binding.h>
#include <Tool.h>
#include <GtkUtil.h>
#include <GtkInit.h>
#include <GtkView.h>
#include <Attribute.h>
#include <GtkCanvas.h>
#include <Interface.h>
#include <GtkDocument.h>
#include <InterfaceAttribute.h>

using namespace std;

unsigned int GtkView::windowCount = 0;

gboolean GtkView::on_treeview_key(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	GtkView *gtkv = (GtkView *)user_data;

	if (event->keyval == GDK_KEY_Delete)
	{
		if (gtkv->selection != NULL)
		{
			Interface *i = gtkv->selection->getInterface();
			delete gtkv->selection->getInterface();
			gtkv->selection = NULL;
		}
	}

	return true;
}

void GtkView::bound_edited(GtkCellRendererToggle *cellRenderer, gchar *path_string, gpointer data)
{
	GtkTreeModel *model = GTK_TREE_MODEL(((GtkDocument *)((GtkView *)data)->document)->node_store);
	GtkTreeIter iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
	gboolean bound;

	gpointer attr;
	Attribute *attribute;
	GtkView *view = (GtkView *)data;

	if (gtk_tree_path_get_depth(path) != 2) return;

	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, GtkDocument::COL_SELECTIONBOUND, &bound, -1);
	gtk_tree_model_get (model, &iter, GtkDocument::COL_REFERENCE, &attr, -1);

	attribute = (Attribute *)attr;

	if (bound)
	{
		attribute->getBinding()->removeAttribute(attribute);
		view->document->update(Document::RemoveAttribute, attribute, NULL, NULL);
	} else {
		if (view->selection != NULL)
		{
			if (view->selection->getBinding()->mergeAttribute(attribute))
			{
				view->document->update(Document::AddBinding, attribute, NULL, view->selection->getBinding());
			}
		}
	}


	gtk_tree_path_free (path);

}

void GtkView::value_edited(GtkCellRendererText *cellRenderer, gchar *path_string, gchar *value, gpointer data)
{
	GtkTreeModel *model = GTK_TREE_MODEL(((GtkDocument *)((GtkView *)data)->document)->node_store);
	GtkTreeIter iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_string);

	gpointer attr;
	Attribute *attribute;
	GtkView *gtkv = (GtkView *)data;

	if (gtk_tree_path_get_depth(path) != 2) return;

	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, GtkDocument::COL_REFERENCE, &attr, -1);

	attribute = (Attribute *)attr;

	Value *v = attribute->getType()->parse(std::string(value));
	if (!attribute->getBinding()->setValue(gtkv->document, v))
	{
		GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, _("The binding could not be set. It overconstrains an object in the diagram."));
		g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
}

void GtkView::has_value_edited(GtkCellRendererToggle *cellRenderer, gchar *path_string, gpointer data)
{
	GtkTreeModel *model = GTK_TREE_MODEL(((GtkDocument *)((GtkView *)data)->document)->node_store);
	GtkTreeIter iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
	gboolean value;

	gpointer attr;
	Attribute *attribute;
	GtkView *view = (GtkView *)data;

	if (gtk_tree_path_get_depth(path) != 2) return;

	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, GtkDocument::COL_REFERENCE, &attr, -1);

	attribute = (Attribute *)attr;
	value = attribute->hasValue();

	if (value)
	{
		if (attribute->hasSetValue())
		{
			attribute->getBinding()->unsetValue(view->document, NULL);
			view->document->update(Document::UnsetValue, NULL, NULL, attribute->getBinding());
		} else {
			error_dialog(view->window, _("This value was calculated from other values, you cannot remove it directly. "
				"You must remove the values it was derived from."));
		}
	} else {
		setValue(attribute, view);
	}


	gtk_tree_path_free (path);

}

void GtkView::sets_value_edited(GtkCellRendererToggle *cellRenderer, gchar *path_string, gpointer data)
{
	GtkTreeModel *model = GTK_TREE_MODEL(((GtkDocument *)((GtkView *)data)->document)->node_store);
	GtkTreeIter iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_string);

	gboolean value;
	gpointer attr;
	Attribute *attribute;
	GtkView *view = (GtkView *)data;

	if (gtk_tree_path_get_depth(path) != 2) return;

	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, GtkDocument::COL_REFERENCE, &attr, -1);

	attribute = (Attribute *)attr;
	value = attribute->hasValue();

	if (attribute->hasSetValue())
	{
		attribute->getBinding()->unsetValue(view->document, NULL);
		view->document->update(Document::UnsetValue, NULL, NULL, attribute->getBinding());
	} else {
		if (attribute->getBinding()->isSet(attribute))
		{
			error_dialog(view->window, _("This value was calculated from other values, you cannot set it directly. "
				"You must remove the values it was derived from before setting it."));
		} else {
			setValue(attribute, view);
		}
	}


	gtk_tree_path_free (path);

}

gboolean set_value(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data)
{
	gpointer attr;
	Attribute *attribute;

	if (gtk_tree_path_get_depth(path) != 2) return FALSE;

	gtk_tree_model_get_iter (model, iter, path);
	gtk_tree_model_get (model, iter, GtkDocument::COL_REFERENCE, &attr, -1);

	attribute = (Attribute *)attr;

	std::string value;
	if (attribute->hasValue())
	{
		const Value *v = attribute->getBinding()->getValue();

		if (v != NULL)
		{
			value = v->getString();
		}
	}

	gtk_tree_store_set (GTK_TREE_STORE (model), iter, GtkDocument::COL_VALUE, value.c_str(), -1);
	gtk_tree_store_set (GTK_TREE_STORE (model), iter, GtkDocument::COL_SETSVALUE, attribute->hasSetValue(), -1);

	return FALSE;
}

gboolean bind_selection(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data)
{
	gboolean bound;

	gpointer attr;
	Attribute *attribute;
	GtkView *view = (GtkView *)data;

	gtk_tree_model_get_iter (model, iter, path);
	gtk_tree_model_get (model, iter, GtkDocument::COL_REFERENCE, &attr, -1);

	attribute = (Attribute *)attr;

	bound = (attribute->getBinding() == view->selection->getBinding());
	gtk_tree_store_set (GTK_TREE_STORE (model), iter, GtkDocument::COL_SELECTIONBOUND, bound, -1);

	return FALSE;
}

void GtkView::collapse(GtkTreeView *view, GtkTreeIter *iter, GtkTreePath *path, gpointer data)
{
	GtkView *gtkv = (GtkView *)data;
	Interface *node;
	GtkTreeModel *model = gtk_tree_view_get_model(view);

	gtk_tree_model_get_iter (model, iter, path);
	gtk_tree_model_get (model, iter, GtkDocument::COL_REFERENCE, &node, -1);

	foreach(i, node->attributes)
	{
		if (i->second == gtkv->selection)
		{
			gtkv->setSelection(GtkTreeIter(), NULL);
			gtkv->document->update(GtkDocument::Deselection, gtkv->selection, NULL, NULL);
		}
	}
}

void GtkView::expand(GtkTreeView *view, GtkTreeIter *iter, GtkTreePath *path, gpointer data)
{
	// If nothing is currently selected, select the first item in any tab that gets opened
	GtkView *gtkv = (GtkView *)data;

	if (gtkv->selection == NULL)
	{
		gtk_tree_path_down(path);
		gtk_tree_selection_select_path(gtk_tree_view_get_selection(view), path);
	}
}

gboolean GtkView::select_attributes(GtkTreeSelection *selection,
                                    GtkTreeModel *model,
                                    GtkTreePath *path,
                                    gboolean path_currently_selected,
                                    gpointer data)
{
	GValue value = {0};
	GtkTreeIter iter;
	GtkView *view = (GtkView *)data;

	if (gtk_tree_path_get_depth(path) != 2) return FALSE;

	gtk_tree_model_get_iter(model, &iter, path);

	gtk_tree_model_get_value(model, &iter, GtkDocument::COL_REFERENCE, &value);

	if (!path_currently_selected)
	{
		view->setSelection(iter, (InterfaceAttribute *)g_value_get_pointer(&value));

		gtk_tree_model_foreach(model, bind_selection, data);
	}

	g_value_unset(&value);

	return TRUE;
}

gboolean select_aggregate(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	return FALSE;
}

gboolean GtkView::select_point(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	GtkView *view = (GtkView *)g_object_get_data(G_OBJECT(widget), "view");
	void (*notify)(Document *, double, double, void *) = (void (*)(Document *, double, double, void *))user_data;
	pair<int, void *> handler = view->canvas_mouse_handler.top();
	view->canvas_mouse_handler.pop();
	double x = event->x;
	double y = event->y;
	view->canvas->toCanvas(&x, &y);
	notify(view->document, x, y, handler.second);
	//view->document->update(Document::SetValue, NULL, NULL, (Binding *)handler.second);
	gtk_statusbar_pop(view->statusbar, gtk_statusbar_get_context_id(view->statusbar, "Retrieve Point"));
	view->settingValue = false;
	g_signal_handler_disconnect(view->canvas->getCairoWidget(), handler.first);

	GdkCursor *cursor = gdk_cursor_new_for_display(gtk_widget_get_display(GTK_WIDGET(view->canvas->getCairoWidget())), GDK_LEFT_PTR);
	gdk_window_set_cursor(gtk_widget_get_parent_window(GTK_WIDGET(view->canvas->getCairoWidget())), cursor);

	return TRUE;
}

void GtkView::newDocument(GtkMenuItem *m, gpointer user_data)
{
	new GtkView(new GtkDocument);
}

void GtkView::newView(GtkMenuItem *m, gpointer user_data)
{
	new GtkView(((GtkView *)user_data)->document);
}

void GtkView::getPoint(void (*notify)(Document *doc, double x, double y, void *data), void *data)
{
	settingValue = true;
	GdkCursor *cursor = gdk_cursor_new_for_display(gtk_widget_get_display(GTK_WIDGET(canvas->getCairoWidget())), GDK_CROSSHAIR);
	gdk_window_set_cursor(gtk_widget_get_parent_window(GTK_WIDGET(canvas->getCairoWidget())), cursor);
	gtk_statusbar_push(statusbar, gtk_statusbar_get_context_id(statusbar, "Retrieve Point"), _("Please click on the canvas to select a point"));
	canvas_mouse_handler.push(make_pair(g_signal_connect(G_OBJECT(canvas->getCairoWidget()), "button-press-event", G_CALLBACK(select_point), (void *)notify), data));
}

void on_open1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	try
	{
		((GtkDocument *)((GtkView *)user_data)->document)->load(((GtkView *)user_data)->window);
	} catch(...) {
		// The open failed
	}
}


void GtkView::on_save1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	try
	{
		((GtkDocument *)((GtkView *)user_data)->document)->save(((GtkView *)user_data)->window);
	} catch(...) {
		// The save failed
	}
}


void on_savetool_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkView *view = (GtkView *)user_data;
	GtkWidget *file = input_dialog(view->window, "Name Tool");

	GtkWidget *field = gtk_entry_new();
	gtk_widget_show(field);

	GtkWidget *label = gtk_label_new(_("Please enter tool name"));
	gtk_widget_show(label);

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(file))), label);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(file))), field);

	gint ret = gtk_dialog_run(GTK_DIALOG(file));
	if (ret == GTK_RESPONSE_ACCEPT)
	{
		Tool::save(std::string(gtk_entry_get_text(GTK_ENTRY(field))), view->document);

		view->document->update(GtkDocument::ReloadTool, NULL, NULL, NULL);
	}

	gtk_widget_destroy(file);
}


void GtkView::on_save_as1_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	try
	{
		((GtkDocument *)((GtkView *)user_data)->document)->saveAs(((GtkView *)user_data)->window);
	} catch(...) {
		// The save failed
	}
}


void on_cut1_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void on_copy1_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void on_paste1_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void on_delete1_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void on_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	const gchar *authors[] =
	{
		"Aaron Small",
		0
	};

	GtkAboutDialog *aboutDialog = GTK_ABOUT_DIALOG(gtk_about_dialog_new());
	gtk_about_dialog_set_program_name(aboutDialog, _(PACKAGE));
	gtk_about_dialog_set_version(aboutDialog, PACKAGE_VERSION);
	gtk_about_dialog_set_copyright(aboutDialog, "© 2011 Aaron Small");
	gtk_about_dialog_set_website(aboutDialog, "http://smeagle.dyndns.org/~absmall/primdia/");
	gtk_about_dialog_set_authors(aboutDialog, authors);

	gtk_dialog_run(GTK_DIALOG(aboutDialog));
	gtk_widget_destroy(GTK_WIDGET(aboutDialog));
}

void switch_view(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkView *view = (GtkView *)user_data;
	gtk_window_present(GTK_WINDOW(view->window));
}

gboolean GtkView::create_event(GtkToolButton *widget, gpointer data)
{
	Tool *gtkt = (Tool *)data;
	GtkView *view = (GtkView *)g_object_get_data(G_OBJECT(widget), "view");
	view->document->addNode(gtkt);

	return FALSE;
}

void GtkView::setValue(Attribute *attr, GtkView *view)
{
	GdkCursor *cursor = gdk_cursor_new_for_display(gtk_widget_get_display(GTK_WIDGET(view->canvas->getCairoWidget())), GDK_LEFT_PTR);
	gdk_window_set_cursor(gtk_widget_get_parent_window(GTK_WIDGET(view->canvas->getCairoWidget())), cursor);

	if (view->settingValue)
	{
		// We must be cancelling
		gtk_statusbar_pop(view->statusbar, gtk_statusbar_get_context_id(view->statusbar, "Retrieve Point"));
		pair<int, void *> handler = view->canvas_mouse_handler.top();
		view->canvas_mouse_handler.pop();
		g_signal_handler_disconnect(view->canvas->getCairoWidget(), handler.first);
		view->settingValue = false;
	} else {
		attr->getType()->input(view, attr->getBinding());
	}
}

gboolean GtkView::delete_event(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
	GtkView *gtkv = (GtkView *)user_data;

	if (gtkv->document->isModified())
	{
		GtkWidget *dialog = save_query(gtkv->window, "Document modified", gtkv->document->getName().c_str());
		gint response = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);

		switch(response)
		{
			case GTK_RESPONSE_ACCEPT:
			case GTK_RESPONSE_YES:
			{
				try
				{
					((GtkDocument *)gtkv->document)->save(gtkv->window);
					return FALSE;
				} catch(...) {
					return TRUE;
				}
			}
			break;
			case GTK_RESPONSE_NO:
			{
				return FALSE;
			}
			break;
			default:
			{
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

void GtkView::destroy(GtkWidget *widget, gpointer data)
{
	gint width, height;
	GtkView *gtkv=(GtkView *)data;

    g_settings_set_int(client, "canvaswidth", gtk_paned_get_position(GTK_PANED(gtkv->pane)));

	width = gtk_widget_get_allocated_width(gtkv->window);
	height = gtk_widget_get_allocated_height(gtkv->window);

    g_settings_set_int(client, "width", width);
    g_settings_set_int(client, "height", height);

	delete gtkv;
}

GtkView::GtkView() : View(new GtkDocument)
{
	windowCount++;
	initialize();
}

GtkView::GtkView(Document *doc) : View(doc)
{
	windowCount++;
	initialize();
}

GtkView::~GtkView()
{
	delete canvas;
	windowCount--;
	if (windowCount == 0)
	{
		document->Clear();
		gtk_main_quit();
	}
}

void valid_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	Attribute *attr;
	gboolean bound;

	gtk_tree_model_get (model, iter, GtkDocument::COL_REFERENCE, &attr, -1);

	// Special rendering for interfaces
	if (gtk_tree_path_get_depth(gtk_tree_model_get_path(model, iter)) == 1)
	{
		g_object_set(renderer, "strikethrough", FALSE, NULL);
		g_object_set(renderer, "cell-background", "white", NULL);
		return;
	}

	gtk_tree_model_get (model, iter, GtkDocument::COL_SELECTIONBOUND, &bound, -1);
	if (bound)
	{
		g_object_set(renderer, "cell-background", "grey", NULL);
	} else {
		g_object_set(renderer, "cell-background", "white", NULL);
	}

	// Test if the value can be set
	gboolean can_set, has_set, has_value;
	has_value = attr->hasValue();
	has_set = attr->hasSetValue();

	if (has_value && !has_set)
	{
		can_set = false;
	} else if (has_set) {
		can_set = true;
	} else {
		if (attr->getBinding()->testValue())
		{
			can_set = true;
		} else {
			can_set = false;
		}
	}

	if (can_set)
	{
		g_object_set(renderer, "strikethrough", FALSE, NULL);
	} else {
		g_object_set(renderer, "strikethrough", TRUE, NULL);
	}
}

void draw_box(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	// Special rendering for interfaces
	if (gtk_tree_path_get_depth(gtk_tree_model_get_path(model, iter)) == 1)
	{
		g_object_set(renderer, "visible", FALSE, NULL);
		g_object_set(renderer, "cell-background", "white", NULL);
	} else {
		gboolean bound;
		gtk_tree_model_get (model, iter, GtkDocument::COL_SELECTIONBOUND, &bound, -1);

		if (bound)
		{
			g_object_set(renderer, "cell-background", "grey", NULL);
		} else {
			g_object_set(renderer, "cell-background", "white", NULL);
		}
		g_object_set(renderer, "visible", TRUE, NULL);
	}
}

void draw_value(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	GtkView *gtkv = (GtkView *)user_data;

	// Special rendering for interfaces
	if (gtk_tree_path_get_depth(gtk_tree_model_get_path(model, iter)) == 1)
	{
		g_object_set(renderer, "cell-background", "white", NULL);
	} else {
		gboolean bound;
		Attribute *attr;
		gtk_tree_model_get (model, iter, GtkDocument::COL_REFERENCE, &attr, -1);
		gtk_tree_model_get (model, iter, GtkDocument::COL_SELECTIONBOUND, &bound, -1);

		if (bound)
		{
			g_object_set(renderer, "cell-background", "grey", NULL);
		} else {
			g_object_set(renderer, "cell-background", "white", NULL);
		}

		gboolean can_set, has_set, has_value;
		has_value = attr->hasValue();
		has_set = attr->hasSetValue();

		if (has_value && !has_set)
		{
			can_set = false;
		} else if (has_set) {
			can_set = true;
		} else {
			if (attr->getBinding()->testValue())
			{
				attr->getBinding()->unsetValue(gtkv->document, NULL);
				can_set = true;
			} else {
				can_set = false;
			}
		}

		if (can_set)
		{
			g_object_set(renderer, "editable", TRUE, NULL);
		} else {
			g_object_set(renderer, "editable", FALSE, NULL);
		}
	}
}

void GtkView::initialize()
{
	GtkWidget *vbox1;
	GtkWidget *menubar1;
	GtkWidget *menuitem1;
	GtkWidget *menu1;
	GtkWidget *new_document;
	GtkWidget *new_view;
	GtkWidget *open1;
	GtkWidget *save1;
	GtkWidget *savetool;
	GtkWidget *save_as1;
	GtkWidget *menuitem2;
	GtkWidget *menu2;
	GtkWidget *cut1;
	GtkWidget *copy1;
	GtkWidget *paste1;
	GtkWidget *delete1;
	GtkWidget *menuitem4;
	GtkWidget *menu4;
	GtkWidget *about;
	GtkWidget *hbox1;
	GtkWidget *node_list;
	GtkWidget *scrollview;
	GtkAdjustment *hadj;
	GtkAdjustment *vadj;
	GtkAccelGroup *accel_group;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *node_list_selection;

	selection = NULL;
	settingValue = FALSE;

	accel_group = gtk_accel_group_new ();

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), _("Main window"));

	vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (window), vbox1);

	menubar1 = gtk_menu_bar_new ();
	gtk_widget_show (menubar1);
	gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

	menuitem1 = gtk_menu_item_new_with_mnemonic (_("_File"));
	gtk_widget_show (menuitem1);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

	menu1 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menu1);

	new_document = gtk_menu_item_new_with_mnemonic(_("New _Document"));
	gtk_widget_show (new_document);
	gtk_container_add (GTK_CONTAINER (menu1), new_document);

	new_view = gtk_menu_item_new_with_mnemonic(_("New _View"));
	gtk_widget_show (new_view);
	gtk_container_add (GTK_CONTAINER (menu1), new_view);

	open1 = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
	gtk_widget_show (open1);
	gtk_container_add (GTK_CONTAINER (menu1), open1);

	savetool = gtk_menu_item_new_with_mnemonic(_("Save _Tool"));
	gtk_widget_show(savetool);
	gtk_container_add(GTK_CONTAINER(menu1), savetool);

	save1 = gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
	gtk_widget_show (save1);
	gtk_container_add (GTK_CONTAINER (menu1), save1);

	save_as1 = gtk_image_menu_item_new_from_stock ("gtk-save-as", accel_group);
	gtk_widget_show (save_as1);
	gtk_container_add (GTK_CONTAINER (menu1), save_as1);

	menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Edit"));
	//gtk_widget_show (menuitem2);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem2);

	menu2 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), menu2);

	cut1 = gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
	gtk_widget_show (cut1);
	gtk_container_add (GTK_CONTAINER (menu2), cut1);

	copy1 = gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
	gtk_widget_show (copy1);
	gtk_container_add (GTK_CONTAINER (menu2), copy1);

	paste1 = gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
	gtk_widget_show (paste1);
	gtk_container_add (GTK_CONTAINER (menu2), paste1);

	delete1 = gtk_image_menu_item_new_from_stock ("gtk-delete", accel_group);
	gtk_widget_show (delete1);
	gtk_container_add (GTK_CONTAINER (menu2), delete1);

	viewConnector = gtk_menu_item_new_with_mnemonic (_("_View"));
	gtk_widget_show (viewConnector);
	gtk_container_add (GTK_CONTAINER (menubar1), viewConnector);

	windowMenu = GTK_MENU(gtk_menu_new ());
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (viewConnector), GTK_WIDGET(windowMenu));

	menuitem4 = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_widget_show (menuitem4);
	gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

	menu4 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menu4);

	about = gtk_menu_item_new_with_mnemonic (_("_About"));
	gtk_widget_show (about);
	gtk_container_add (GTK_CONTAINER (menu4), about);

	hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), GTK_WIDGET(hbox1), TRUE, TRUE, 0);

	toolbar = gtk_toolbar_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (toolbar), GTK_ORIENTATION_VERTICAL);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
	gtk_container_set_border_width (GTK_CONTAINER (toolbar), 5);
	gtk_widget_show(toolbar);
	gtk_container_add(GTK_CONTAINER (hbox1), toolbar);

	for(map<std::string,Tool *>::iterator i = document->toolBegin(); i != document->toolEnd(); i++)
	{
		Tool *gtkt = static_cast<Tool *>(i->second);
		GtkWidget *icon = gtk_image_new_from_file(gtkt->icon.c_str());
		gtk_widget_show(GTK_WIDGET(icon));
		GtkToolItem *toolitem = gtk_tool_button_new(icon, NULL);
		gtk_tool_item_set_tooltip_text(toolitem, i->second->description.c_str());
		gtk_tool_item_set_homogeneous(toolitem, true);
		gtk_tool_item_set_visible_vertical(toolitem, true);
		gtk_widget_show(GTK_WIDGET(toolitem));
		gtk_toolbar_insert (GTK_TOOLBAR (toolbar), toolitem, -1);
		g_object_set_data(G_OBJECT(toolitem), "view", this);
		g_signal_connect(G_OBJECT(toolitem), "clicked",
				G_CALLBACK(create_event), gtkt);
	}

	pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_show(GTK_WIDGET(pane));
	gtk_paned_set_position(GTK_PANED(pane), g_settings_get_int(client, "canvaswidth"));
	gtk_box_pack_start (GTK_BOX (hbox1), GTK_WIDGET(pane), TRUE, TRUE, 0);

	canvas = new GtkCanvas(this);
	gtk_widget_set_events(GTK_WIDGET(canvas->getCairoWidget()), GDK_BUTTON_PRESS_MASK);
	gtk_widget_show(GTK_WIDGET(canvas->getCairoWidget()));
	g_object_set_data(G_OBJECT(canvas->getCairoWidget()), "view", this);
	gtk_paned_pack1(GTK_PANED(pane), GTK_WIDGET(canvas->getCairoWidget()), TRUE, TRUE);

	// Now resize the window to match the last window that closed
	
	int width = g_settings_get_int(client, "width");
	int height = g_settings_get_int(client, "height");

	gtk_window_set_default_size(GTK_WINDOW(window), width, height);

	hadj = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 500.0, 1, 1, 50));
	vadj = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 65536.0, 1, 1, 10));

	scrollview = gtk_scrolled_window_new(hadj, vadj);
	gtk_widget_show(scrollview);

	node_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(((GtkDocument *)document)->node_store));
	node_list_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(node_list));
	gtk_tree_selection_set_select_function(node_list_selection, select_attributes, this, NULL);
	g_signal_connect ((gpointer) node_list, "row_collapsed", G_CALLBACK (collapse), (void *)this);
	g_signal_connect ((gpointer) node_list, "row_expanded", G_CALLBACK (expand), (void *)this);

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Name"));
	gtk_tree_view_append_column(GTK_TREE_VIEW(node_list), column);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(column, renderer, valid_data_func, this, NULL);
	gtk_tree_view_column_add_attribute(column, renderer, "text", GtkDocument::COL_NAME);

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Value"));
	gtk_tree_view_append_column(GTK_TREE_VIEW(node_list), column);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(column, renderer, draw_value, this, NULL);
	gtk_tree_view_column_add_attribute(column, renderer, "text", GtkDocument::COL_VALUE);
	g_signal_connect(renderer, "edited", G_CALLBACK(value_edited), this);

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Sets Value"));
	gtk_tree_view_append_column(GTK_TREE_VIEW(node_list), column);
	renderer = gtk_cell_renderer_toggle_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(column, renderer, draw_box, NULL, NULL);
	gtk_tree_view_column_add_attribute(column, renderer, "active", GtkDocument::COL_SETSVALUE);
	g_signal_connect(renderer, "toggled", G_CALLBACK(sets_value_edited), this);

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Bind to selection"));
	gtk_tree_view_append_column(GTK_TREE_VIEW(node_list), column);
	renderer = gtk_cell_renderer_toggle_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(column, renderer, draw_box, NULL, NULL);
	gtk_tree_view_column_add_attribute(column, renderer, "active", GtkDocument::COL_SELECTIONBOUND);
	g_signal_connect(renderer, "toggled", G_CALLBACK(bound_edited), this);

	gtk_widget_show(node_list);
	gtk_container_add(GTK_CONTAINER (pane), scrollview);
	gtk_container_add(GTK_CONTAINER (scrollview), node_list);

	statusbar = GTK_STATUSBAR(gtk_statusbar_new());
	gtk_widget_show (GTK_WIDGET(statusbar));
	gtk_box_pack_start (GTK_BOX (vbox1), GTK_WIDGET(statusbar), FALSE, FALSE, 0);

	g_signal_connect ((gpointer) new_document, "activate", G_CALLBACK (newDocument), (void *)this);
	g_signal_connect ((gpointer) new_view, "activate", G_CALLBACK (newView), (void *)this);
	g_signal_connect ((gpointer) open1, "activate", G_CALLBACK (on_open1_activate), (void *)this);
	g_signal_connect ((gpointer) save1, "activate", G_CALLBACK (on_save1_activate), (void *)this);
	g_signal_connect ((gpointer) savetool, "activate", G_CALLBACK (on_savetool_activate), (void *)this);
	g_signal_connect ((gpointer) save_as1, "activate", G_CALLBACK (on_save_as1_activate), (void *)this);
	g_signal_connect ((gpointer) cut1, "activate", G_CALLBACK (on_cut1_activate), (void *)this);
	g_signal_connect ((gpointer) copy1, "activate", G_CALLBACK (on_copy1_activate), (void *)this);
	g_signal_connect ((gpointer) paste1, "activate", G_CALLBACK (on_paste1_activate), (void *)this);
	g_signal_connect ((gpointer) delete1, "activate", G_CALLBACK (on_delete1_activate), (void *)this);
	g_signal_connect ((gpointer) about, "activate", G_CALLBACK (on_about_activate), (void *)this);

	g_signal_connect ((gpointer) node_list, "key-release-event", G_CALLBACK (on_treeview_key), (void *)this);

	g_signal_connect (G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), (void *)this);

	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), (void *)this);

	gtk_widget_show(window);
}

void GtkView::updateWindowList()
{
	gtk_menu_detach(windowMenu);
	windowMenu = GTK_MENU(gtk_menu_new ());
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (viewConnector), GTK_WIDGET(windowMenu));
	foreach(i, Document::getDocuments())
	{
		GtkWidget *d = gtk_menu_item_new_with_label((*i)->getName().c_str());
		gtk_widget_show(d);
		GtkWidget *s = gtk_menu_new();
		gtk_widget_show(s);

		gtk_menu_shell_append(GTK_MENU_SHELL(windowMenu), d);
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(d), s);
		foreach(j, (*i)->getViews())
		{
			GtkWidget *si = gtk_menu_item_new_with_label((*j)->getName().c_str());
			gtk_widget_show(si);
			gtk_menu_shell_append(GTK_MENU_SHELL(s), si);
			g_signal_connect ((gpointer) si, "activate", G_CALLBACK (switch_view), *j);
		}
	}
}

void GtkView::clear_toolbar(GtkWidget *child, void *data)
{
	GtkView *gtkv = (GtkView *)data;

	gtk_container_remove(GTK_CONTAINER(gtkv->toolbar), child);
}

void GtkView::update(int command, Attribute *attribute, Interface *node, Binding *binding)
{
	GtkTreeIter iter, attr_iter;

	switch(command)
	{
		case Document::AddBinding:
		{
			GtkTreeModel *model = GTK_TREE_MODEL(
					((GtkDocument *)document)->node_store);

			gtk_tree_model_foreach(model, set_value, this);

			gtk_widget_queue_draw(
					GTK_WIDGET(canvas->getCairoWidget()));
		}
		break;
		case GtkDocument::Deselection:
		{
			gtk_widget_queue_draw(
					GTK_WIDGET(canvas->getCairoWidget()));
		}
		break;

		case Document::RemoveAttribute:
		{
			GtkTreeModel *model = GTK_TREE_MODEL(((GtkDocument *)document)->node_store);
			gtk_tree_model_foreach(model, bind_selection, this);
			gtk_widget_queue_draw(GTK_WIDGET(canvas->getCairoWidget()));
		}
		break;
		case Document::SetValue:
		case Document::UnsetValue:
		{
			GtkTreeModel *model = GTK_TREE_MODEL(((GtkDocument *)document)->node_store);
			gtk_tree_model_foreach(model, set_value, this);
			gtk_widget_queue_draw(GTK_WIDGET(canvas->getCairoWidget()));
		}
		break;
		case Document::AddNode:
		{
			gtk_widget_queue_draw(GTK_WIDGET(canvas->getCairoWidget()));
		}
		break;
		case Document::RemoveNode:
		{
			GtkTreeIter iter;
			gtk_tree_model_iter_parent(GTK_TREE_MODEL(((GtkDocument *)document)->node_store), &iter, &selectionrow);
			gtk_tree_store_remove(((GtkDocument *)document)->node_store, &iter);
			gtk_widget_queue_draw(GTK_WIDGET(canvas->getCairoWidget()));
		}
		break;
		case GtkDocument::ReloadTool:
		{
			document->Clear();
			document->Init();

			gtk_container_foreach(GTK_CONTAINER(toolbar), clear_toolbar, this);

			for(map<std::string,Tool *>::iterator i = document->toolBegin(); i != document->toolEnd(); i++)
			{
				Tool *gtkt = static_cast<Tool *>(i->second);
				GtkWidget *icon = gtk_image_new_from_file(gtkt->icon.c_str());
				gtk_widget_show(GTK_WIDGET(icon));
				GtkToolItem *toolitem = gtk_tool_button_new(icon, NULL);
				gtk_tool_item_set_tooltip_text(toolitem, i->second->description.c_str());
				gtk_tool_item_set_homogeneous(toolitem, true);
				gtk_tool_item_set_visible_vertical(toolitem, true);
				gtk_widget_show(GTK_WIDGET(toolitem));
				gtk_toolbar_insert (GTK_TOOLBAR (toolbar), toolitem, -1);
				g_object_set_data(G_OBJECT(toolitem), "view", this);
				g_signal_connect(G_OBJECT(toolitem), "clicked",
						G_CALLBACK(create_event), gtkt);
			}
		}
		break;
		default:
		{
			// An unknown command was received. Update everything. I should change this to an assert later.
			gtk_tree_store_clear(((GtkDocument *)document)->node_store);

			foreach(i, document->getNodes())
			{

				gtk_tree_store_append(((GtkDocument *)document)->node_store, &iter, NULL);

				gtk_tree_store_set(((GtkDocument *)document)->node_store, &iter,
					GtkDocument::COL_NAME, (*i)->getName().c_str(),
					GtkDocument::COL_REFERENCE, *i,
					-1);

				foreach(j, (*i)->attributes)
				{
					std::string value;

					if (j->second->hasValue())
					{
						value = j->second->getBinding()->getValue()->getString();
					}

					gtk_tree_store_append(((GtkDocument *)document)->node_store, &attr_iter, &iter);
					gtk_tree_store_set(((GtkDocument *)document)->node_store, &attr_iter,
						GtkDocument::COL_NAME, j->first.c_str(),
						GtkDocument::COL_VALUE, value.c_str(),
						GtkDocument::COL_SETSVALUE, j->second->hasSetValue(),
						GtkDocument::COL_SELECTIONBOUND, false,
						GtkDocument::COL_REFERENCE, j->second,
						-1);
				}
			}

			gtk_widget_queue_draw(GTK_WIDGET(canvas->getCairoWidget()));
		}
	}
}

void GtkView::setSelection(GtkTreeIter iter, InterfaceAttribute *ia)
{
	selection = ia;
	selectionrow = iter;
	gtk_widget_queue_draw(GTK_WIDGET(canvas->getCairoWidget()));
}
