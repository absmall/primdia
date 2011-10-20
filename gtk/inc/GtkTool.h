#ifndef __GTKTOOL_H__
#define __GTKTOOL_H__

#include <config/common.h>

#include <gtk/gtk.h>

#include <Tool.h>

class GtkTool : public Tool
{
public:
	GtkTool(const std::string &basename, const std::string &filename);
	static void Init();

	std::string icon;
private:
	static void processDirectory(std::string dir);
};

#endif /* __GTKTOOL_H__ */
