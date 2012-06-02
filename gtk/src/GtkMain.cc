#include <stdlib.h>
#include <iostream>
#include <gio/gio.h>

#include <GtkTool.h>
#include <GtkView.h>
#include <Primitive.h>

using namespace std;

GSettings *client;

int main(int argc, char *argv[])
{
	GError *err;

#if ENABLE_NLS
	setlocale(LC_ALL, "");
	textdomain (PACKAGE_NAME);
	bindtextdomain (PACKAGE_NAME, LOCALEDIR);
#endif

	try
	{
		Primitive::InitializeAll();
	} catch(string message) {
		cout << message << endl;
	}

	gtk_init(&argc, &argv);
    client = g_settings_new("org.dyndns.smeagle.primdia");

	GtkTool::Init();

	try
	{
		new GtkView();

		gtk_main();

	} catch(string message) {
		cout << message << endl;
	}

	return 0;
}
