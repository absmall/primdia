#include <stdlib.h>
#include <iostream>
#include <gconf/gconf-client.h>

#include <GtkTool.h>
#include <GtkView.h>
#include <Primitive.h>

using namespace std;

GConfClient *client;

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
	if (gconf_init(argc, argv, &err))
	{
		client = gconf_client_get_default();
	} else {
		cout << "Could not connect to gconf. Configuration settings are not accessible." << endl;
		g_error_free(err);
	}

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
