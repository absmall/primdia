#include <string>
#include <dirent.h>
#include <iostream>
#include <sys/types.h>

#include <GtkTool.h>

GtkTool::GtkTool(const std::string &basename, const std::string &filename) : Tool(basename + std::string("tools/") + filename)
{
	icon = basename + std::string("images/") + filename + std::string(".png");
}

void GtkTool::Init()
{
	// Load tools
	try
	{
		const gchar *const *dirs = g_get_system_data_dirs();
		for(int i = 0; dirs[i]; i++)
		{
			processDirectory(std::string(dirs[i])+std::string("/")+std::string(PACKAGE_NAME)+std::string("/"));
		}
		processDirectory(std::string(g_get_user_data_dir())+std::string("/applications/")+std::string(PACKAGE_NAME)+std::string("/"));
	} catch(std::string message) {
		std::cout << message << std::endl;
	}
}

void GtkTool::processDirectory(std::string dir)
{
	struct dirent *de;
	DIR *td = opendir((dir+std::string("tools/")).c_str());
	if (td == NULL) return;

	while((de = readdir(td)) != NULL)
	{
		if (de->d_name[0] != '.')
		{
			new GtkTool(std::string(dir), std::string(de->d_name));
		}
	}

	closedir(td);
}
