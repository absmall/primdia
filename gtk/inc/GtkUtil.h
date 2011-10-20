#ifndef __GTKUTIL_H__
#define __GTKUTIL_H__

#include <config/common.h>

#include <gtk/gtk.h>

GtkWidget *input_dialog(GtkWidget *parent, const char *title);
GtkWidget *open_dialog(GtkWidget *parent, const char *title);
GtkWidget *save_dialog(GtkWidget *parent, const char *title);
GtkWidget *save_query(GtkWidget *parent, const char *title, const char *docname);
void warn_dialog(GtkWidget *parent, const char *description, const char *message);
void error_dialog(GtkWidget *parent, const char *message);

#endif /* __GTKUTIL_H__ */
